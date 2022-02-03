#include "drawwidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <iostream>
#include <QTextEdit>
#include <QPen>
#include <QImage>

const int circ_size = 10;
QVector2D p1(0, 0);
QVector2D p2(100, 0);
QVector2D p3(100, 100);

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    _trace = false;
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    if(_trace)
    {
        int W = width();
        int H = height();

        SimpleScene sc(W, H);
        QImage img(W, H, QImage::Format_RGB32);

        for(int y = 0; y < H; y++)
          for(int x = 0; x < W; x++)
          {
            TColor col = sc.get_pixel(x, y);
              img.setPixelColor(x, y, QColor(col.R, col.G, col.B));//QColor(x % 255, 100, 255 - y % 255));
          }
        p.drawImage(0, 0, img);
    }

    QPen pen(Qt::black);
    if(!_trace)
    {
        p.setPen(pen);
        for(int i = 0; i < this->GetPoints().size(); ++i)
        {
            QVector2D Pn = this->_points[i];
            p.drawArc(Pn.x() - circ_size/2, Pn.y() - circ_size/2, circ_size, circ_size, 0, 360 * 16);
        }
    }

    if(!this->GetConvexHull().isEmpty())
    {
        for(int i = 0; i < this->GetConvexHull().size()-1; ++i)
        {
            pen.setColor(Qt::black);
            if(i == 0)
                pen.setColor(Qt::red);
            else if(i == 1)
                pen.setColor(Qt::blue);
            p.setPen(pen);
            QVector2D Pn = this->_convex_hull[i];
            p.drawArc(Pn.x() - circ_size/2, Pn.y() - circ_size/2, circ_size, circ_size, 0, 360 * 16);
        }
        for(int i = 0; i < this->GetConvexHull().size()-1; ++i)
            p.drawLine(this->_convex_hull[i].toPointF(), this->_convex_hull[i+1].toPointF());
    }

    if(!this->GetTriangulation().isEmpty())
    {
        for (int i = 0; i < this->GetTriangulation().size(); ++i)
        {
            section s = this->GetTriangulation()[i];
            p.drawLine(s.a.toPointF(), s.b.toPointF());
        }
    }

    if(!this->GetPolygon1().isEmpty())
    {
        pen.setColor(Qt::red);
        p.setPen(pen);
        for(int i = 0; i < this->GetPolygon1().size() - 1; ++i)
        {
            p.drawText(this->GetPolygon1()[i].x(), this->GetPolygon1()[i].y() - 3/2*circ_size, QString::number(i+1));
            p.drawLine((this->GetPolygon1()[i]).toPointF(), (this->GetPolygon1()[i+1]).toPointF());
        }

        if(!this->GetPolygon2().isEmpty())
        {
            pen.setColor(Qt::blue);
            p.setPen(pen);
            for(int i = 0; i < this->GetPolygon2().size() - 1; ++i)
            {
                p.drawText(this->GetPolygon2()[i].x(), this->GetPolygon2()[i].y() - 3/2*circ_size, QString::number(i+1));
                p.drawLine((this->GetPolygon2()[i]).toPointF(), (this->GetPolygon2()[i+1]).toPointF());
            }

            if(!this->GetIntersection().isEmpty())
            {
                pen.setColor(Qt::magenta);
                p.setPen(pen);
                for(int i = 0; i < this->GetIntersection().size()-1; ++i)
                {
                    QVector2D Pn = this->GetIntersection()[i];
                    p.drawArc(Pn.x() - circ_size/2, Pn.y() - circ_size/2, circ_size, circ_size, 0, 360 * 16);
                    p.drawLine((this->GetIntersection()[i]).toPointF(), (this->GetIntersection()[i+1]).toPointF());
                }
            }

        }
    }

    if(!this->GetSpline().isEmpty())
    {
        pen.setColor(Qt::black);
        p.setPen(pen);
        for(int i = 0; i < this->GetSpline().size(); ++i)
        {
            QVector2D Pn = this->_spline[i];
            p.drawArc(Pn.x() - circ_size/2, Pn.y() - circ_size/2, circ_size, circ_size, 0, 360 * 16);
            p.drawText(Pn.x(), Pn.y() - 3/2*circ_size, QString::number(i+1));
        }
        if(!this->GetSplineLagrange().isEmpty())
        {
            pen.setColor(Qt::red);
            p.setPen(pen);
            for (int i = 0; i < this->GetSplineLagrange().size()-1; ++i)
                p.drawLine((this->GetSplineLagrange()[i]).toPointF(), (this->GetSplineLagrange()[i+1]).toPointF());
        }
        if(!this->GetSplineHermite().isEmpty())
        {
            pen.setColor(Qt::green);
            p.setPen(pen);
            for (int i = 0; i < this->GetSplineHermite().size()-1; ++i)
                p.drawLine((this->GetSplineHermite()[i]).toPointF(), (this->GetSplineHermite()[i+1]).toPointF());
            if(!this->GetDerivative().isEmpty())
            {
                pen.setColor(Qt::red);
                p.setPen(pen);
                for(int i = 0; i < this->GetDerivative().size(); ++i)
                {
                    QPointF p1 = (this->_spline[i]).toPointF();
                    QPointF p2 = (this->_spline[i] + this->_derivative[i]).toPointF();
                    p.drawLine(p1, p2);
                }
            }
        }
        if(!this->GetSplineCubic().isEmpty())
        {
            pen.setColor(Qt::blue);
            p.setPen(pen);
            for (int i = 0; i < this->GetSplineCubic().size()-1; ++i)
                p.drawLine((this->GetSplineCubic()[i]).toPointF(), (this->GetSplineCubic()[i+1]).toPointF());
            if(!this->GetDerivative().isEmpty())
            {
                pen.setColor(Qt::red);
                p.setPen(pen);
                for(int i = 0; i < this->GetDerivative().size(); ++i)
                {
                    QPointF p1 = (this->_spline[i]).toPointF();
                    QPointF p2 = (this->_spline[i] + this->_derivative[i]).toPointF();
                    p.drawLine(p1, p2);
                }
            }
        }
        if(!this->GetCurvesBezier().isEmpty())
        {
            pen.setColor(Qt::magenta);
            p.setPen(pen);
            for (int i = 0; i < this->GetCurvesBezier().size()-1; ++i)
                p.drawLine((this->GetCurvesBezier()[i]).toPointF(), (this->GetCurvesBezier()[i+1]).toPointF());
        }
    }
}

void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    bool p_catch = false;
    if(event->button() == Qt::LeftButton)
    {
        QVector2D Pn(event->x(), event->y());
        for(int i = 0; i < this->GetPoints().size(); ++i)
        {
            if((this->_points[i]-Pn).length() < circ_size/2)
            {
                _mouseMovePointFlag = true;
                _ptNum = i;
                p_catch = true;
            }
        }
        if(!p_catch)
        {
            if(_polygon1.isEmpty() || _polygon2.isEmpty())
            {
                PushPoint(Pn);
                emit NewPoint();
                if(!_trace)
                    repaint();
            }
        }
    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
  _mouseMovePointFlag = false;
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(_mouseMovePointFlag)
    {
      QVector2D curPos(event->x(), event->y());
      emit MovePoint(curPos);
      this->UpdatePoint(GetPtNum(), curPos);
      if(!_trace)
        repaint();
    }
}

void DrawWidget::Reset()
{
    this->ClearPoints();
    this->ClearConvexHull();
    this->ClearTriangulation();
    this->ClearPolygon1();
    this->ClearPolygon2();
    this->ClearIntersection();
    this->ClearSpline();
    this->ClearSplineLagrange();
    this->ClearSplineHermite();
    this->ClearDerivative();
    this->ClearSplineCubic();
    this->ClearCurvesBezier();
    repaint();
}
