#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "convex_hull.h"
#include "triangulation.h"
#include "polygon_intersection.h"
#include "tracing.h"
#include "spline.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    convex_hull* _convex_hull_win;
    triangulation* _triangulation_win;
    polygon_intersection* _polygon_intersection_win;
    tracing* _tracing_win;
    spline* _spline_win;

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
