#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <QWidget>
#include "drawwidget.h"

namespace Ui {
class triangulation;
}

class triangulation : public QWidget
{
    Q_OBJECT

public:
    explicit triangulation(QWidget *parent = nullptr);
    ~triangulation();

    QVector<section> GetTriangulation() {return _triangulation;}
    void SetTriangulation(QVector<section> triangulation) {_triangulation = triangulation;}
    void ClearTriangulation() {_triangulation.clear();}

    QVector<section> GetSections() {return _sections;}
    void SetSections(QVector<section> sections) {_sections = sections;}
    void ClearSections() {_sections.clear();}

    QVector<QVector2D> GetPoints() {return _points;}
    void PushPoint(QVector2D new_point) {_points.push_back(new_point);}
    void ClearPoints() {_points.clear();}

    bool IsIntersectTriangulation(section s);
    QVector<QPair<QVector2D, QVector2D>> BuildTriangulation(QVector<QVector2D> points);

    QVector<section> Sort();
    void Triangulate();

private slots:
    void PushPoint();
    void UpdatePoint(QVector2D new_point);

    void on_Reset_clicked();

    void on_Triangulation_stateChanged(int arg1);

private:
    Ui::triangulation *ui;

    QVector<QVector2D> _points;
    QVector<section> _sections;
    QVector<section> _triangulation;
};

#endif // TRIANGULATION_H
