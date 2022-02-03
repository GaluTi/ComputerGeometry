#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>
#include <QVector>
#include <algorithm>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QVector>
#include <cmath>
#include <string>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

struct TColor
{
  unsigned short R;
  unsigned short G;
  unsigned short B;

  TColor() { }
  TColor(unsigned short iR, unsigned short iG, unsigned short iB) {R = iR, G = iG, B = iB;}

  unsigned short Max(unsigned short a, unsigned short b) const
  {
      if(a < b)
          return b;
      else
          return a;
  }

  unsigned short Min(unsigned short a, unsigned short b) const
  {
      if(a > b)
          return b;
      else
          return a;
  }

  TColor operator+(const TColor& v) const
  {
    return TColor(std::min(R + v.R, 255), std::min(G + v.G, 255), std::min(B + v.B, 255));
  }

  TColor operator*(const double& f) const
  {
    return TColor(std::min(int(R * f), 255), std::min(int(G * f), 255), std::min(int(B * f), 255));
  }

  TColor apply_force(double lforce) const
  {
      unsigned short tR = std::min(255, int(lforce * R));
      unsigned short tG = std::min(255, int(lforce * G));
      unsigned short tB = std::min(255, int(lforce * B));
      return TColor(tR, tG, tB);
  }
};

struct TVec3f
{
  double x;
  double y;
  double z;

  TVec3f() { }
  TVec3f(double iX, double iY, double iZ) {x = iX; y = iY; z = iZ;}

  TVec3f operator+(const TVec3f& v) const
  {
    return TVec3f(x + v.x, y + v.y, z + v.z);
  }

  TVec3f operator-(const TVec3f& v) const
  {
    return TVec3f(x - v.x, y - v.y, z - v.z);
  }

  double operator*(const TVec3f& v) const
  {
    return x * v.x + y * v.y + z * v.z;
  }

  TVec3f crossProduct(const TVec3f& v) const
  {
      return TVec3f(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
  }

  TVec3f operator*(const double& f) const
  {
    return TVec3f(x * f, y * f, z * f);
  }

  double length() const
  {
    return sqrt(x * x + y * y + z * z);
  }

  void Normalize()
  {
    double l = length();
    if(l > 0)
    {
      x /= l;
      y /= l;
      z /= l;
    }
  }
};
inline TVec3f operator*(const double& f, const TVec3f& v)
{
  return v * f;
}

class TObject
{
protected:
   TColor color;
   double specular_exponent;
   QVector4D albedo;
   double ref;

public:
   TObject() {color = {0, 0, 0}; specular_exponent = 1; albedo = {1.0, 0.0, 0.0, 0.0}; ref = 1.0;}
   TObject(const TColor& col) {color = col; specular_exponent = 1; albedo = {1.0, 0.0, 0.0, 0.0}; ref = 1.0;}
   TObject(const TColor& col, const double& sp, const QVector4D& alb, const double& ri) {color = col; specular_exponent = sp; albedo = alb; ref = ri;}

   const TColor& get_color() const { return color; }
   const double& get_specular_exponent() const { return specular_exponent; }
   const QVector4D& get_albedo() const { return albedo; }
   const double& get_ref() const { return ref; }



   virtual bool IsIntersect(const TVec3f& ray_src, const TVec3f& ray_dir, double& tau) = 0;

   virtual bool IsIntersect(const TVec3f& ray_src, const TVec3f& ray_dir, double& tau, TVec3f& pt, TVec3f& nl) = 0;
};

class TSphere : public TObject
{

public:

    TVec3f center;
    double radius;

    TSphere(const TVec3f& c, const double& r, const TColor& col)
    {
        color = col;
        center = c;
        radius = r;
    }
    TSphere(const TVec3f& c, const double& r, const TColor& col, const double& sp, const QVector4D& alb, const double& ri)
    {
        color = col;
        specular_exponent = sp;
        albedo = alb;
        ref = ri;
        center = c;
        radius = r;
    }

    virtual bool IsIntersect(const TVec3f& orig, const TVec3f& dir, double& t0)
    {
      TVec3f L = (orig - center);
      double A = dir*dir;
      double B = dir*L;
      double C = L*L - radius * radius;
      double D4 = B*B - A*C;
      if(D4 < 0)
        return false;

      double t1 = (-B - sqrt(D4))/A;
      double t2 = (-B + sqrt(D4))/A;
      if(t1 < 0 && t2 < 0)
        return false;
      else if(t1 < 0)
       t0 = t2;
      else
        t0 = t1;

      return true;
    }


    virtual bool IsIntersect(const TVec3f& ray_src, const TVec3f& ray_dir, double& tau, TVec3f& pt, TVec3f& nl)
    {
      if(IsIntersect(ray_src, ray_dir, tau))
      {
        pt = ray_src + tau * ray_dir;
        nl = pt - center;
        nl.Normalize();
        return true;
      }
      return false;
    }
};

class TTriangulation : public TObject
{

public:

    int f_i;
    QVector<TVec3f> verts;
    QVector<TVec3f> normals;
    QVector<QVector<int>> triangles;

    TTriangulation(const TColor& col) {color = col;}
    TTriangulation(const TColor& col, const double& sp, const QVector4D& alb, const double& ri, double x, double y, double z, double k)
    {
        color = col;
        specular_exponent = sp;
        albedo = alb;
        ref = ri;
        f_i = -1;

        std::ifstream input;
        input.open ("C:\\Users\\timo2\\Documents\\tasks\\Qt\\CompGem\\CompGem\\Piramida.obj", std::ifstream::in);
        if(input.fail())
        {
            std::cout << "Failed to open Piramida.obj" << std::endl;
            return;
        }
        std::string str;
        while (!input.eof())
        {
            std::getline(input, str);
            std::istringstream iss(str.c_str());
            char index;
            if (!str.compare(0, 2, "v "))
            {
                iss >> index;
                TVec3f v;
                iss >> v.x;
                iss >> v.y;
                iss >> v.z;
                v.x -= x;
                v.x *= k;
                v.y -= y;
                v.y *= k;
                v.z += z;
                v.x *= k;
                verts.push_back(v);
            }
            else if(!str.compare(0, 3, "vn "))
            {
                iss >> index;
                iss >> index;
                TVec3f vn;
                iss >> vn.x;
                iss >> vn.y;
                iss >> vn.z;
                normals.push_back(vn);
            }
            else if(!str.compare(0, 2, "f "))
            {
                std::cout << "New Triangle\n";
                QVector<int> f;
                QVector<int> vert;
                iss >> index;
                int t;
                int v;
                int n;
                int count = 0;

                /*iss >> v >> index >> n >>index >> n >> index;
                vert.push_back(v-1);
                iss >> v >> index >> n >>index >> n >> index;
                vert.push_back(v-1);
                iss >> v >> index >> n >>index >> n >> index;
                vert.push_back(v-1);*/

                iss >> v;
                vert.push_back(v-1);
                iss >> v;
                vert.push_back(v-1);
                iss >> v;
                vert.push_back(v-1);


                /*while (iss)
                {
                    char c;
                    iss >> v >> c;
                    vert.push_back(v-1);
                    iss >> t >> c;
                    iss >> n >> c;
                }*/

                //std::cout << "Times in while " << count << std::endl;

                for(int i = 2; i < vert.size(); ++i)
                {
                    f.clear();
                    f.push_back(vert[0]);
                    f.push_back(vert[i]);
                    f.push_back(vert[i-1]);
                    f.push_back(n-1);
                    triangles.push_back(f);
                }
            }
        }
        std::cout << verts.size() << std::endl << normals.size() << std::endl << triangles.size() << std::endl;
    }


    bool IsIntersectTriangle(int fi, const TVec3f& orig, const TVec3f& dir, double& t0, TVec3f& pt, TVec3f& nl)
    {
        /*TVec3f v0 = verts[triangles[fi][0]];
        TVec3f v1 = verts[triangles[fi][1]];
        TVec3f v2 = verts[triangles[fi][2]];
        TVec3f v0v1 = v1 - v0;
        TVec3f v0v2 = v2 - v0;
        TVec3f N = v0v1.crossProduct(v0v2);
        //TVec3f N = normals[triangles[fi][3]];

        if (fabs(N*dir) < 1e-7)
            return false;

        double d = N*v0;
        double t = (N*orig + d)/(N*dir);
        if (t < 0)
            return false;

        TVec3f P = orig + t*dir;
        TVec3f C;

        TVec3f v1v0 = v1 - v0;
        TVec3f v0p = P - v0;
        C = v1v0.crossProduct(v0p);
        if (N*C < 0)
            return false;

        TVec3f v2v1 = v2 - v1;
        TVec3f v1p = P - v1;
        C = v2v1.crossProduct(v1p);
        if (N*C < 0)
            return false;

        TVec3f v2v0 = v0 - v2;
        TVec3f v2p = P - v2;
        C = v2v0.crossProduct(v2p);
        if (N*C < 0)
            return false;

        t0 = t;
        nl = N;
        N.Normalize();
        pt = P;
        return true;*/

        TVec3f v0 = verts[triangles[fi][0]];
        TVec3f v1 = verts[triangles[fi][1]];
        TVec3f v2 = verts[triangles[fi][2]];
        TVec3f v1v0 = v1 - v0;
        TVec3f v2v0 = v2 - v0;
        TVec3f dir_v2v0 = dir.crossProduct(v2v0);
        double det = v1v0*dir_v2v0;
        if(det < 1e-7)
            return false;

        TVec3f orig_v0 = orig - v0;
        double u = orig_v0*dir_v2v0;
        if(u < 0 || u > det)
            return false;

        TVec3f ve = orig_v0.crossProduct(v1v0);
        double v = dir*ve;
        if(v < 0 || u + v > det)
            return false;

        t0 = v2v0*ve * (1.0/det);
        nl = v1v0.crossProduct(v2v0);
        nl.Normalize();
        pt = orig + t0*dir;
        return t0 > 1e-7;

    }

    virtual bool IsIntersect(const TVec3f& orig, const TVec3f& dir, double& t0)
    {
        for(int i = 0; i < triangles.size(); ++i)
        {
            /*if(IsIntersectTriangle(i ,orig, dir, t0))
            {
                f_i = i;
                return true;
            }*/
        }
      return false;
    }


    virtual bool IsIntersect(const TVec3f& ray_src, const TVec3f& ray_dir, double& tau, TVec3f& pt, TVec3f& nl)
    {
        for(int i = 0; i < triangles.size(); ++i)
        {
              if(IsIntersectTriangle(i, ray_src, ray_dir, tau, pt, nl))
              {
                pt = ray_src + tau*ray_dir;
                //nl = normals[triangles[f_i][3]];
                TVec3f v0 = verts[triangles[i][0]];
                TVec3f v1 = verts[triangles[i][1]];
                TVec3f v2 = verts[triangles[i][2]];
                TVec3f v0v1 = v1 - v0;
                TVec3f v0v2 = v2 - v0;
                nl = v0v1.crossProduct(v0v2);
                nl.Normalize();
                return true;
              }
        }
        return false;
    }
};

struct Light
{
    Light(const TVec3f &p, const double &i) {position = p; intensity = i;}
    TVec3f position;
    double intensity;
};

class SimpleScene
{

protected:

  double Vw; //view point
  double Vh;
  double Dist;

  int CCx, CCy;

  int Cw;
  int Ch;

  double ambient_light;
  TColor ambient_color;

  QVector<Light> lights;

  double diffuse_light_intensity, specular_light_intensity;

  std::vector<TObject*> objects;

public:
  SimpleScene(int W, int H) : Cw(W), Ch(H)
  {
    Vh = 1;
    Vw = double (Cw * Vh) / Ch; 1.0;

    Dist = 1;

    CCx = W / 2;
    CCy = H / 2;

    ambient_color = TColor(50, 200, 220);
    ambient_light = 0.0;
    diffuse_light_intensity = 0.0;
    specular_light_intensity = 0.0;

    Light L1 = {{-50, 50, -100}, 0.7};
    Light L2 = {{-30, 10, -100}, 0.8};
    Light L3 = {{200, 20, -100}, 1.0};
    lights.push_back(L1);
    //lights.push_back(L2);
    lights.push_back(L3);

        //objects.push_back(new TSphere(TVec3f(3, 5, 30), 6, TColor(0, 0, 0), 200, QVector4D(1.0, 2.0, 0.9, 0.0), 0));
    //objects.push_back(new TSphere(TVec3f(-3, 2, 15), 3, TColor(0, 200, 0), 250.0, QVector4D(0.8, 0.5, 0.01, 0.0), 0));
    objects.push_back(new TSphere(TVec3f(7, -3, 24), 4, TColor(200, 0, 0), 200, QVector4D(1.0, 0.8, 0.05, 0.0), 0));
    //objects.push_back(new TSphere(TVec3f(-18, 4, 40), 12, TColor(255, 255, 1), 0, QVector4D(0.8, 0.2, 0.0, 0.0), 0));
    objects.push_back(new TSphere(TVec3f(3, 5, 30), 6, TColor(0, 0, 0), 200, QVector4D(1.0, 2.0, 0.9, 0.0), 0));
    objects.push_back(new TSphere(TVec3f(8, -3.5, 16), 2.5, TColor(0, 0, 0), 100, QVector4D(1.0, 0.1, 0.0, 0.95), 1.15));
    //objects.push_back(new TSphere(TVec3f(-18, 4, 40), 15, TColor(255, 255, 1), 200, QVector4D(1.0, 2.0, 0.0, 0.0), 0));
    //objects.push_back(new TTriangulation(TColor(250, 250, 0), 200, QVector4D(1.0, 1.0, 0.0, 0.0), 1.0, 0, 0, 8, 10));
    //objects.push_back(new TTriangulation(TColor(0, 0, 0), 200, QVector4D(1.0, 0.0, 0.9, 0.0), 1.0, -2, -0.5, 6, 1.0));
    //objects.push_back(new TTriangulation(TColor(200, 0, 200), 200, QVector4D(1.0, 0.85, 0.0, 0.0), 1.0, 1, 0.5, 6, 1.0));
        //objects.push_back(new TSphere(TVec3f(8, -2, 15), 2, TColor(0, 0, 0), 100, QVector4D(1.0, 0.1, 0.0, 0.95), 1.05));
  }

  TVec3f reflect(const TVec3f &l, const TVec3f &n)
  {
      return l - n*2.0*(l*n);
  }

  TVec3f refract(const TVec3f &l, const TVec3f &n, const double &ref)
  {
      double cos_alpha = -1*l*n/(l.length()*n.length());
      double n1 = 1.0;
      double n2 = ref;
      TVec3f N = n;
      if (cos_alpha < 0.0)
      {
          cos_alpha = -cos_alpha;
          n1 = ref;
          n2 = 1.0;
          N = -1*n;
      }
      double n1_to_n2 = n1/n2;
      double cos_beta_squared = 1.0 - n1_to_n2*n1_to_n2*(1.0 - cos_alpha*cos_alpha);
      return cos_beta_squared < 0.0 ? TVec3f(0,0,0) : l*n1_to_n2 + n*(n1_to_n2 * cos_alpha - sqrt(cos_beta_squared));
  }

  TColor ray_trace(const TVec3f &orig, const TVec3f &dir, int k)
  {
      bool intersect = false;
      double t0;
      TVec3f N;//нормаль
      TVec3f N1;
      TVec3f P;//точка
      TVec3f P1;
      TColor Col;//цвет
      int num;

      for(size_t q = 0; q < objects.size(); ++q)
      {
        double tT;
        TVec3f tN;//возвращаем сюда вектор нормали
        TVec3f tP;//возвращаем сюда точку пересечения
        TColor tCol;

        if(objects[q]->IsIntersect(orig, dir, tT, tP, tN))
        {
          if(!intersect || tT < t0)
          {
            t0 = tT;
            N = tN;
            P = tP;
            Col = objects[q]->get_color();
            num = q;
          }
          intersect = true;
        }
      }

      if (k > 1 || !intersect)
            return ambient_color;


      double ttt;
      TVec3f reflect_dir = reflect(dir, N);
      reflect_dir.Normalize();
      TVec3f reflect_orig = reflect_dir*N < 0 ? P - N*1e-3 : P + N*1e-3;
      TColor reflect_color = ray_trace(reflect_orig, reflect_dir, k + 1);
      /*TColor reflect_color;
      bool refl = false;
      for(int j = 0; j < objects.size(); ++j)
      {
          if(j != num)
          {
              if (objects[j]->IsIntersect(reflect_orig, reflect_dir, ttt))
              {
                 reflect_color = objects[j]->get_color();
                 refl = true;
              }
          }
      }
      if(!refl)
          reflect_color = ambient_color;*/

      TVec3f refract_dir = refract(dir, N, objects[num]->get_ref());
      refract_dir.Normalize();
      TVec3f refract_orig = refract_dir*N < 0 ? P - N*1e-3 : P + N*1e-3;
      TColor refract_color;
      bool refract = false;
      for(int j = 0; j < objects.size(); ++j)
      {
          if(j != num)
          {
              if (objects[j]->IsIntersect(refract_orig, refract_dir, ttt))
              {
                 refract_color = objects[j]->get_color();
                 refract = true;
              }
          }
      }
      if(!refract)
          refract_color = ambient_color;

      double dli = diffuse_light_intensity;
      double sli = specular_light_intensity;
      for(size_t i = 0; i < lights.size(); ++i)
      {
          TVec3f ldir = lights[i].position - P;
          ldir.Normalize();
          TVec3f shadow_orig = ldir*N < 0 ? P - N*1e-3 : P + N*1e-3;
          bool shad = false;

          /*for(int j = 0; j < objects.size()-1; ++j)
          {
              if(j != num)
              {
                  if (objects[j]->IsIntersect(shadow_orig, ldir, ttt))
                  {
                     shad = true;
                  }
              }
          }
          if(shad == true)
              continue;*/


          double f = ldir * N;
          if(f > 0)
             dli  += f * lights[i].intensity;
          if(((-1)*reflect((-1)*ldir, N))*dir > 0)
             sli += powf(((-1)*reflect((-1)*ldir, N))*dir, objects[num]->get_specular_exponent())*lights[i].intensity;
      }

      //TColor C1 = objects[num]->get_color()*dli*objects[num]->get_albedo().x();
      //TColor C1 = objects[num]->get_color()*dli*objects[num]->get_albedo().x() + TColor(1.0, 1.0, 1.0)*sli*objects[num]->get_albedo().y();
      //TColor C1 = objects[num]->get_color()*dli*objects[num]->get_albedo().x() + TColor(1.0, 1.0, 1.0)*sli*objects[num]->get_albedo().y() + reflect_color*objects[num]->get_albedo().z();
      //TColor C1 = objects[num]->get_color()*dli*objects[num]->get_albedo().x() + TColor(1.0, 1.0, 1.0)*sli*objects[num]->get_albedo().y() + refract_color*objects[num]->get_albedo().w();
      TColor C1 = objects[num]->get_color()*dli*objects[num]->get_albedo().x() + TColor(1.0, 1.0, 1.0)*sli*objects[num]->get_albedo().y() + reflect_color*objects[num]->get_albedo().z() + refract_color*objects[num]->get_albedo().w();
      return C1;
  }

  TColor get_pixel(int x, int y)
  {
     double rX = Vw * (x - CCx) / Cw;
     double rY = Vh * (CCy - y) / Ch;

     TVec3f orig(rX, rY, Dist);
     TVec3f dir(rX, rY, Dist);

     return ray_trace(orig, dir, 0);
  }
};

#endif // GEOMETRY_H
