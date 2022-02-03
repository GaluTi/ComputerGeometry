#include "polygon_intersection.h"
#include "ui_polygon_intersection.h"

polygon_intersection::polygon_intersection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::polygon_intersection)
{
    ui->setupUi(this);
    connect(ui->draw_widget, SIGNAL(NewPoint()), this, SLOT(PushPoint()));
    //connect(ui->draw_widget, SIGNAL(MovePoint(QVector2D)), this, SLOT(UpdatePoint(QVector2D)));
}

polygon_intersection::~polygon_intersection()
{
    delete ui;
}

void polygon_intersection::PushPoint()
{
     QVector2D p = ui->draw_widget->GetPoints().back();
     this->_points.push_back(p);
}

/*void polygon_intersection::UpdatePoint(QVector2D new_point)
{
    int N = ui->draw_widget->GetPtNum();
    QVector2D Pn = ui->draw_widget->GetPoints()[N];
    for (int i = 0; i < this->_points.size(); ++i)
    {
        if(this->_points[i] == Pn)
        {
            this->_points[i] = new_point;
            break;
        }
    }
    for (int i = 0; i < this->_polygon1.size(); ++i)
    {
        if(this->_polygon1[i] == Pn)
        {
            this->_polygon1[i] = new_point;
            this->_polygon1 = BuildConvexHull(this->_polygon1)[0];
            ui->draw_widget->SetPolygon1(this->_polygon1);
            this->_points = _polygon1 + _polygon2;
            ui->draw_widget->SetPoints(this->_points);
            break;
        }
    }
    for (int i = 0; i < this->_polygon2.size(); ++i)
    {
        if(this->_polygon2[i] == Pn)
        {
            this->_polygon2[i] = new_point;
            this->_polygon2 = BuildConvexHull(this->_polygon2)[0];
            ui->draw_widget->SetPolygon2(this->_polygon2);
            this->_points = _polygon1 + _polygon2;
            ui->draw_widget->SetPoints(this->_points);
            break;
        }
    }

    Intersection();
    if(!_intersection.isEmpty())
    {
        this->_intersection = BuildConvexHull(this->_intersection)[0];
        ui->draw_widget->SetIntersection(this->_intersection);
        repaint();
    }

}*/

double crossProductSections(section s1, section s2)
{
    return (s1.b.x() - s1.a.x())*(s2.b.y() - s2.a.y()) - (s1.b.y() - s1.a.y())*(s2.b.x() - s2.a.x());
}

double crossProductVectors(QVector2D v1, QVector2D v2)
{
    return v1.x()*v2.y() - v1.y()*v2.x();
}

bool polygon_intersection::IsIntersect(section s1, section s2, QVector2D& intersect_point)
{
    /*QVector2D p = s1.a;
    QVector2D q = s2.a;
    QVector2D r = s1.b - s1.a;
    QVector2D s = s2.b - s2.a;
    double rs = crossProductVectors(r, s);
    double qpr = crossProductVectors(q-p, r);
    double qps = crossProductVectors(q-p, s);
    if(rs != 0)
    {
        double t = qps/rs;
        double u = qpr/rs;
        if(0 <= t && t <= 1 && 0 <= u && u <= 1)
        {
            intersect_point = q + u*s;
            return true;
        }
    }*/

    float p0_x = s1.a.x(), p0_y = s1.a.y(), p1_x = s1.b.x(), p1_y = s1.b.y(), p2_x = s2.a.x(), p2_y = s2.a.y(), p3_x = s2.b.x(), p3_y = s2.b.y();

    float s1_x, s1_y, s2_x, s2_y;
        s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
        s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

        float s, t;
        s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
        t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

        if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
        {
            intersect_point.setX(p0_x + (t * s1_x));
            intersect_point.setY(p0_y + (t * s1_y));
            return true;
        }

    return false;
}

bool polygon_intersection::InIntersection(QVector2D p)
{
    for(int i = 0; i < _intersection.size(); ++i)
        if(p == _intersection[i])
            return true;
    return false;
}

bool polygon_intersection::InPolygon(polygon poly, QVector2D p)
{

    for(int i = 0; i < poly._vertices.size()-1; ++i)
    {
        section s1(poly._vertices[i], p);
        section s2(poly._vertices[i], poly._vertices[i+1]);
        if(crossProductSections(s1, s2) < 0)
            return false;
    }
    return true;
}

void:: polygon_intersection::Intersection()
{
    _intersection.clear();
    for(int i = 0; i < _polygons[0]._vertices.size() - 1; ++i)
    {
        if(InPolygon(_polygons[1], _polygons[0]._vertices[i]))
        {
            _intersection.push_back(_polygons[0]._vertices[i]);
            //std::cout << "Point polygon 1: " << i+1 << " in polygon 2\n";
        }
    }
    for(int i = 0; i < _polygons[1]._vertices.size() - 1; ++i)
    {
        if(InPolygon(_polygons[0], _polygons[1]._vertices[i]))
        {
            _intersection.push_back(_polygons[1]._vertices[i]);
            //std::cout << "Point polygon 2: " << i+1 << " in polygon 1\n";
        }
    }

    for(int i = 0; i < _polygons[0]._vertices.size() - 1; ++i)
    {
        int count = 0;
        section A(_polygons[0]._vertices[i], _polygons[0]._vertices[i+1]);
        QVector2D int_pt1;
        QVector2D int_pt2;
        for(int j = 0; j < _polygons[1]._vertices.size() - 1; ++j)
        {
            section B(_polygons[1]._vertices[i], _polygons[1]._vertices[i+1]);
            if(IsIntersect(A, B, int_pt2))
            {
                std::cout << std::endl;
                if(count == 0)
                    int_pt1 = int_pt2;
                std::cout << "Polygon1 segment " << i+1 << "-" << i+2 << " intersect polygon2 segment " << j+1 << "-" << j+2 << std::endl;
                std::cout << A.a.x() << " " << A.a.y() << "\n";
                std::cout << int_pt2.x() << " " << int_pt2.y() << "\n";
                std::cout << A.b.x() << " " << A.b.y() << "\n";
                count++;
            }
        }

        if(count == 1)
            _intersection.push_back(int_pt2);

        else if(count == 2)
        {
            _intersection.push_back(int_pt1);
            _intersection.push_back(int_pt2);
        }
    }
}

void polygon_intersection::on_NewPolygon_clicked()
{
    if(this->_polygons.isEmpty())
    {
        polygon poly(this->_points);
        this->_polygons.push_back(poly);
        this->_polygons[0]._vertices = BuildConvexHull(this->_polygons[0]._vertices)[0];
        ui->draw_widget->SetPolygon1(this->_polygons[0]._vertices);

        _points = this->_polygons[0]._vertices;
        ui->draw_widget->SetPoints(this->_points);
        num = this->_points.size();
        repaint();
    }
    else if(this->_polygons.size() == 1)
    {
        polygon poly;
        for(int i = num; i < _points.size(); ++i)
            poly._vertices.push_back(_points[i]);
        this->_polygons.push_back(poly);
        this->_polygons[1]._vertices = BuildConvexHull(this->_polygons[1]._vertices)[0];
        ui->draw_widget->SetPolygon2(this->_polygons[1]._vertices);

        this->_points = this->_polygons[0]._vertices + this->_polygons[1]._vertices;
        ui->draw_widget->SetPoints(this->_points);
        repaint();
    }
    else
        QMessageBox::warning(this, "Warning!", "You have already set two polygons!");
}

void polygon_intersection::on_Intersection_clicked()
{
    if(this->_polygons.size() == 1)
        QMessageBox::warning(this, "Warning!", "Set polygons!");
    else
    {
        Intersection();
        if(!_intersection.isEmpty())
        {
            this->_intersection = BuildConvexHull(this->_intersection)[0];
            ui->draw_widget->SetIntersection(this->_intersection);
            repaint();
        }
    }
}

void polygon_intersection::on_Reset_clicked()
{
    this->_points.clear();
    this->_polygons.clear();
    this->_intersection.clear();
    ui->draw_widget->Reset();
}


