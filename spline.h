#ifndef SPLINE_H
#define SPLINE_H

#include <QWidget>
#include "drawwidget.h"

namespace Ui {
class spline;
}

class spline : public QWidget
{
    Q_OBJECT

public:
    explicit spline(QWidget *parent = nullptr);
    ~spline();

    QVector<QVector2D> GetSpline() {return _spline;}
    QVector2D Lagrange(double t);
    QVector<QVector2D> Derivative();
    QVector2D Hermite(double t);
    QVector<QVector2D> Gauss(QVector<QVector<double>> S, QVector<QVector2D> P, int condition);
    QVector2D Cubic(double t, int condition);
    double fact(int n);
    double Cnk(int n, int k);
    QVector2D Bezier (double t);

private slots:
    void PushPoint();
    void UpdatePoint(QVector2D new_point);

    void on_Lagrange_stateChanged(int arg1);
    void on_Hermite_stateChanged(int arg1);
    void on_HermiteDerivative_stateChanged(int arg1);
    void on_Cubic_stateChanged(int arg1);
    void on_Bezier_stateChanged(int arg1);

    void on_Reset_clicked();

    void on_CubicC1_stateChanged(int arg1);

    void on_CubicC2_stateChanged(int arg1);

private:
    Ui::spline *ui;

    QVector<QVector2D> _spline;
};

#endif // SPLINE_H
