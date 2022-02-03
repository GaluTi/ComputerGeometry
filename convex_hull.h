#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <QWidget>
#include "drawwidget.h"

namespace Ui {
class convex_hull;
}

QVector<point> Sort(QVector<point> points);
double ScalarProduct(QVector2D v1, QVector2D v2);
int FindP2(QVector<point> points);
QVector<QVector<QVector2D>> BuildConvexHull(QVector<QVector2D> points);

class convex_hull : public QWidget
{
    Q_OBJECT

public:
    explicit convex_hull(QWidget *parent = nullptr);
    ~convex_hull();

    QVector<QVector2D> GetPoints() {return _points;}
    void UpdatePoint(int i, QVector2D new_point) {_points[i] = new_point;}
    void PushPoint(QVector2D new_point) {_points.push_back(new_point);}
    void ClearPoints() {_points.clear();}

private slots:
    void PushPoint();
    void UpdatePoint(QVector2D new_point);

    void on_ConvexHull_stateChanged(int arg1);

    void on_Reset_clicked();

private:
    Ui::convex_hull *ui;

    QVector<QVector2D> _points;
};

#endif // CONVEX_HULL_H
