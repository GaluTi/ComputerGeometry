#include "triangulation.h"
#include "ui_Triangulation.h"

#include<iostream>

triangulation::triangulation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::triangulation)
{
    ui->setupUi(this);
    connect(ui->draw_widget, SIGNAL(NewPoint()), this, SLOT(PushPoint()));
    connect(ui->draw_widget, SIGNAL(MovePoint(QVector2D)), this, SLOT(UpdatePoint(QVector2D)));
}

triangulation::~triangulation()
{
    delete ui;
}

void triangulation::PushPoint()
{
    QVector2D p = ui->draw_widget->GetPoints().back();
    for(int i = 0; i < _points.size(); ++i)
    {
        section s(_points[i], p);
        _sections.push_back(s);
    }
    _points.push_back(p);
    if(ui->Triangulation->checkState() == 2)
    {
        Triangulate();
        ui->draw_widget->SetTriangulation(_triangulation);
    }
}

void triangulation::UpdatePoint(QVector2D new_point)
{
    int N = ui->draw_widget->GetPtNum();
    QVector2D Pn = ui->draw_widget->GetPoints()[N];
    QVector2D oldP;
    for (int i = 0; i < this->_points.size(); ++i)
    {
        if(this->_points[i] == Pn)
        {
            oldP = _points[i];
            this->_points[i] = new_point;
            break;
        }
    }
    for(int i = 0; i < _sections.size(); ++i)
    {
        if(_sections[i].a == oldP)
            _sections[i].a = new_point;

        if(_sections[i].b == oldP)
            _sections[i].b = new_point;
    }
    if(ui->Triangulation->checkState() == 2)
    {
        Triangulate();
        ui->draw_widget->SetTriangulation(_triangulation);
    }
}

double crossProduct(QVector2D v1, QVector2D v2)
{
    return v1.x()*v2.y() - v1.y()*v2.x();
}

QVector<section> triangulation::Sort()
{
    std::sort(_sections.begin(), _sections.end(), [](section l, section r)
        {return l.length < r.length;});
    return _sections;
}

bool triangulation::IsIntersectTriangulation(section s)
{
    QVector2D r = s.b - s.a;
    QVector2D rt;
    QVector2D p = s.a;
    QVector2D q;
    for(int i = 0; i < _triangulation.size(); ++i)
    {
        rt = _triangulation[i].b - _triangulation[i].a;
        q = _triangulation[i].a;
        double r_rt = crossProduct(r, rt);
        double rqp = crossProduct(q-p, r);
        double qprt = crossProduct(q-p, rt);
        if(r_rt != 0)
        {
            double t = rqp/r_rt;
            double u = qprt/r_rt;
            if(0 < t && t < 1 && 0 < u && u < 1)
                return true;
        }
    }
    return false;
}

void triangulation::Triangulate()
{
    this->Sort();
    this->_triangulation.clear();
    for(int i = 0; i < _sections.size(); ++i)
    {
        section s = _sections[i];
        if(!IsIntersectTriangulation(s))
             _triangulation.push_back(s);
    }
}

void triangulation::on_Reset_clicked()
{
    _points.clear();
    _sections.clear();
    _triangulation.clear();
    ui->Triangulation->setCheckState(Qt::Unchecked);
    ui->draw_widget->Reset();
}


void triangulation::on_Triangulation_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearTriangulation();
        repaint();
    }
    else if(arg1 == 2)
    {
        Triangulate();
        ui->draw_widget->SetTriangulation(_triangulation);
        repaint();
    }
}

