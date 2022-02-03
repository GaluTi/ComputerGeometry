#ifndef POLYGON_INTERSECTION_H
#define POLYGON_INTERSECTION_H

#include <QWidget>
#include "drawwidget.h"
#include "convex_hull.h"

namespace Ui {
class polygon_intersection;
}

class polygon_intersection : public QWidget
{
    Q_OBJECT

public:
    explicit polygon_intersection(QWidget *parent = nullptr);
    ~polygon_intersection();

    QVector<QVector2D> GetPoints() {return _points;}
    void SetPoints(QVector<QVector2D> points) {_points = points;}
    void ClearPoints() {_points.clear();}

    QVector<polygon> GetPolygons() {return _polygons;}
    void SetPolygons(QVector<polygon> polygons) {_polygons = polygons;}
    void ClearPolygons() {_polygons.clear();}

    bool IsIntersect(section s1, section s2, QVector2D& intersect_point);
    bool InIntersection(QVector2D p);
    bool InPolygon(polygon poly, QVector2D p);
    void Intersection();

    int num;
private slots:
    void PushPoint();
    //void UpdatePoint(QVector2D new_point);

    void on_NewPolygon_clicked();
    void on_Intersection_clicked();
    void on_Reset_clicked();

private:
    Ui::polygon_intersection *ui;

    QVector<QVector2D> _points;
    QVector<polygon> _polygons;
    /*QVector<QVector2D> _polygon1;
    QVector<QVector2D> _polygon2;*/
    QVector<QVector2D> _intersection;
};

#endif // POLYGON_INTERSECTION_H
