#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include <QVector>
#include <QVector2D>
#include <QPointF>
#include <QPair>
#include <algorithm>
#include <math.h>
#include <QString>
#include <QMessageBox>
#include "geometry.h"

struct section
{
public:
    QVector2D a;
    QVector2D b;
    double length;
    section() {}
    section(QVector2D A, QVector2D B) {a = A; b = B; length = (a-b).length();}

};

class polygon
{
public:
    QVector<QVector2D> _vertices;
    polygon() {}
    polygon (QVector<QVector2D> vertices)
    {
        _vertices = vertices;
    }

};

class point
{
 public:
    point() {}
    point(QVector2D r) {
        _painted = false;
        _r = r;
    }
    point(float a, float b) {
        _painted = false;
        _r = {a, b};
    }

    QVector2D GetPoint(){return _r;}
    void SetPoint(QVector2D r) {_r = r;}

    bool IsPainted() {return _painted;}
    void Paint(bool flag) {_painted = flag;}

private:
    QVector2D _r;
    bool _painted;
};

class Triangle
{

public:
    Triangle() {}
    Triangle(QVector2D a, QVector2D b, QVector2D c)
    {
        _a = a; _b = b; _c = c;
        float zx = (_b.y()-_a.y())*_c.lengthSquared() + (_a.y()-_c.y())*_b.lengthSquared() + (_c.y()-_b.y())*_a.lengthSquared();
        float zy = (_b.x()-_a.x())*_c.lengthSquared() + (_a.x()-_c.x())*_b.lengthSquared() + (_c.x()-_b.x())*_a.lengthSquared();
        float z = 2*((_c.x()-_b.x())*(_b.y()-_a.y()) - (_b.x()-_a.x())*(_c.y()-_b.y()));
        _o = {zx/z, -zy/z};
        _r = (_o-_a).length();
    }

    QVector2D GetA() {return _a;}
    QVector2D GetB() {return _b;}
    QVector2D GetC() {return _c;}
    QVector2D GetO() {return _o;}
    double GetR() {return _r;}

    double Area()
    {
        double AB = (_a-_b).length();
        double AC = (_a-_c).length();
        double BC = (_b-_c).length();
        double p = (AB + AC + BC)/2;
        return sqrt(p*(p-AB)*(p-AC)*(p-BC));
    }
    bool InTriangle(QVector2D p)
    {
        Triangle abp(_a, _b, p);
        Triangle apc(_a, p, _c);
        Triangle bpc(_b, p, _c);
        double AB = (_a-_b).length();
        double AC = (_a-_c).length();
        double BC = (_b-_c).length();
        double hp = (AB + AC + BC)/2;
        double S1 = abp.Area() + apc.Area();
        double S2 = abp.Area() + bpc.Area();
        double S3 = apc.Area() + bpc.Area();
        double S0 = sqrt(hp*(hp-AB)*(hp-AC)*(hp-BC));
        if(S1 > S0 || S2 > S0 || S3 > S0)
            return false;
        else
            return true;
    }
    bool HaveEdge(QVector2D a, QVector2D b)
    {
        if((_a == a || _b == a || _c == a) && (_a == b || _b == b || _c == b))
            return true;
        return false;
    }

    Triangle operator=(const Triangle& other) {return Triangle(other._a, other._b, other._c);}

private:
    QVector2D _a;
    QVector2D _b;
    QVector2D _c;
    QVector2D _o;
    double _r;
};

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = nullptr);

    void SetTrace(bool flag) {_trace = flag;}
    bool IsTracing() {return _trace;}

    QVector<QVector2D> GetPoints() {return _points;}
    void SetPoints(QVector<QVector2D> points) {_points = points;}
    void UpdatePoint(int i, QVector2D new_point) {_points[i].setX(new_point.x()); _points[i].setY(new_point.y());}
    void PushPoint(QVector2D new_point) {_points.push_back(new_point);}
    void ClearPoints() {_points.clear();}

    QVector<QVector2D> GetConvexHull() {return _convex_hull;}
    void SetConvexHull(QVector<QVector2D> convex_hull) {_convex_hull = convex_hull;}
    void ClearConvexHull() {_convex_hull.clear();}

    QVector<section> GetTriangulation() {return _triangulation;}
    void SetTriangulation(QVector<section> triangulation) {_triangulation = triangulation;}
    void ClearTriangulation() {_triangulation.clear();}

    QVector<QVector2D> GetPolygon1() {return _polygon1;}
    void SetPolygon1(QVector<QVector2D> polygon1) {_polygon1 = polygon1;}
    void ClearPolygon1() {_polygon1.clear();}

    QVector<QVector2D> GetPolygon2() {return _polygon2;}
    void SetPolygon2(QVector<QVector2D> polygon2) {_polygon2 = polygon2;}
    void ClearPolygon2() {_polygon2.clear();}

    QVector<QVector2D> GetIntersection() {return _intersection;}
    void SetIntersection(QVector<QVector2D> intersection) {_intersection = intersection;}
    void ClearIntersection() {_intersection.clear();}

    QVector<QVector2D> GetSpline() {return _spline;}
    void SetSpline(QVector<QVector2D> spline) {_spline = spline;}
    void ClearSpline() {_spline.clear();}

    QVector<QVector2D> GetSplineLagrange() {return _sp_lagrange;}
    void SetSplineLagrange(QVector<QVector2D> sp_lagrange) {_sp_lagrange = sp_lagrange;}
    void ClearSplineLagrange() {_sp_lagrange.clear();}

    QVector<QVector2D> GetSplineHermite() {return _sp_hermite;}
    void SetSplineHermite(QVector<QVector2D> sp_hermite) {_sp_hermite = sp_hermite;}
    void ClearSplineHermite() {_sp_hermite.clear();}

    QVector<QVector2D> GetDerivative() {return _derivative;}
    void SetDerivative(QVector<QVector2D> derivative) {_derivative = derivative;}
    void ClearDerivative() {_derivative.clear();}

    QVector<QVector2D> GetSplineCubic() {return _sp_cubic;}
    void SetSplineCubic(QVector<QVector2D> sp_cubic) {_sp_cubic = sp_cubic;}
    void ClearSplineCubic() {_sp_cubic.clear();}

    QVector<QVector2D> GetCurvesBezier() {return _cr_bezier;}
    void SetCurvesBezier(QVector<QVector2D> cr_bezier) {_cr_bezier = cr_bezier;}
    void ClearCurvesBezier() {_cr_bezier.clear();}

    int GetPtNum() {return _ptNum;}

    void paintEvent(QPaintEvent *event);
    void paintTriangulation(QVector<Triangle> triang);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void Reset();

signals:
    void NewPoint();
    void MovePoint(QVector2D new_point);

private:
    bool _trace;
    QVector<QVector2D> _points;
    QVector<QVector2D> _convex_hull;
    QVector<section> _triangulation;
    QVector<QVector2D> _polygon1;
    QVector<QVector2D> _polygon2;
    QVector<QVector2D> _intersection;
    QVector<QVector2D> _spline;
    QVector<QVector2D> _sp_lagrange;
    QVector<QVector2D> _sp_hermite;
    QVector<QVector2D> _derivative;
    QVector<QVector2D> _sp_cubic;
    QVector<QVector2D> _cr_bezier;
    bool _mouseMovePointFlag = false;
    int _ptNum;
};

#endif // DRAWWIDGET_H
