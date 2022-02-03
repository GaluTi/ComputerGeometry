#include "convex_hull.h"
#include "ui_convex_hull.h"

convex_hull::convex_hull(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::convex_hull)
{
    ui->setupUi(this);
    connect(ui->draw_widget, SIGNAL(NewPoint()), this, SLOT(PushPoint()));
    connect(ui->draw_widget, SIGNAL(MovePoint(QVector2D)), this, SLOT(UpdatePoint(QVector2D)));
}

convex_hull::~convex_hull()
{
    delete ui;
}

void convex_hull::UpdatePoint(QVector2D new_point)
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
    if(ui->ConvexHull->checkState() == 2)
    {
        QVector<QVector2D> convex_hull = BuildConvexHull(this->_points)[0];
        ui->draw_widget->SetConvexHull(convex_hull);
    }
}

void convex_hull::PushPoint()
{
     QVector2D p = ui->draw_widget->GetPoints().back();
     this->_points.push_back(p);
     if(ui->ConvexHull->checkState() == 2)
     {
         QVector<QVector2D> convex_hull = BuildConvexHull(this->_points)[0];
         ui->draw_widget->SetConvexHull(convex_hull);
     }
}

QVector<point> Sort(QVector<point> points)
{
    std::sort(points.begin(), points.end(), [](point l, point r)
        {return l.GetPoint().y() > r.GetPoint().y();});

    std::sort(points.begin(), points.end(), [](point l, point r)
        {
            if(l.GetPoint().y() == r.GetPoint().y())
                return l.GetPoint().x() < r.GetPoint().x();
            else
                return l.GetPoint().y() > r.GetPoint().y();
        });

    return points;
}

double ScalarProduct(QVector2D v1, QVector2D v2)
{
    //QVector2D v1 = p2 - p1;
    //QVector2D v2 = p3 - p2;
    double sp = (v1.x()*v2.x() + v1.y()*v2.y())/(v1.length()*v2.length());
    return sp;
}

int FindP2(QVector<point> points)
{
    QVector2D p1 = points[0].GetPoint();
    QVector2D p0(p1.x()/2, p1.y());
    int n = 0;
    double delta = 1e+9;
    for(int i = 1; i < points.size(); ++i)
    {
        double d = acos(ScalarProduct(p1-p0, points[i].GetPoint()- p1));
        if(d < delta)
        {
            delta = d;
            n = i;
        }
    }
    return n;
}

QVector<QVector<QVector2D>> BuildConvexHull(QVector<QVector2D> points)
{
        QVector<point> color_points;
        for(int i = 0; i < points.size(); ++i)
        {
            point P(points[i]);
            color_points.push_back(P);
        }

        QVector<QVector2D> hull;
        color_points = Sort(color_points);
        hull.push_back(color_points[0].GetPoint());
        hull.push_back(color_points[FindP2(color_points)].GetPoint());
        color_points[FindP2(color_points)].Paint(true);

        while(!color_points[0].IsPainted())
        {
            point plast = hull.back();
            point pprev = hull[hull.size() - 2];
            int n = 0;
            double mncos = 1e+9;
            for(int i = 0; i < points.size(); ++i)
            {
                if(!color_points[i].IsPainted())
                {
                     if(acos(ScalarProduct(plast.GetPoint()-pprev.GetPoint(), color_points[i].GetPoint()-plast.GetPoint())) < mncos)
                     {
                         n = i;
                         double m = acos(ScalarProduct(plast.GetPoint()-pprev.GetPoint(), color_points[i].GetPoint()-plast.GetPoint()));
                         mncos = m;
                     }
                }
            }
            color_points[n].Paint(true);
            hull.push_back(color_points[n].GetPoint());
            if(n == 0)
                break;
        }

        QVector<QVector2D> not_hull;
        for(int i = 0; i < points.size(); ++i)
        {
            QVector2D P = points[i];
            int c = -1;
            for(int j = 0; j < hull.size()-1; ++j)
            {
                if(P == hull[j])
                {
                    c = j;
                    break;
                }
            }
            if(c == -1)
                not_hull.push_back(P);
        }

        QVector<QVector<QVector2D>> ans;
        ans.push_back(hull);
        ans.push_back(not_hull);
        return ans;
}

void convex_hull::on_ConvexHull_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->draw_widget->ClearConvexHull();
        ui->ConvexHull->setCheckState(Qt::Unchecked);
        repaint();
    }
    else if(arg1 == 2)
    {
        if(this->_points.size() < 3)
        {
            QMessageBox::warning(this, "Warning!", "Add Points!");
            ui->ConvexHull->setCheckState(Qt::Unchecked);
        }
        else
        {
            QVector<QVector2D> convex_hull = BuildConvexHull(this->_points)[0];
            ui->draw_widget->SetConvexHull(convex_hull);
            repaint();
        }
    }
}

void convex_hull::on_Reset_clicked()
{
    ui->draw_widget->Reset();
    this->_points.clear();
    ui->ConvexHull->setCheckState(Qt::Unchecked);
}

