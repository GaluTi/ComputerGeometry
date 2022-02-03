#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->Task->currentText() == "Convex Hull")
    {
        this->_convex_hull_win = new convex_hull();
        this->_convex_hull_win->setWindowTitle("Convex Hull");
        this->_convex_hull_win->show();
    }
    else if(ui->Task->currentText() == "Triangulation")
    {
        this->_triangulation_win = new triangulation();
        this->_triangulation_win->setWindowTitle("Triangulation");
        this->_triangulation_win->show();
    }
    else if(ui->Task->currentText() == "Polygon Intersection")
    {
        this->_polygon_intersection_win = new polygon_intersection();
        this->_polygon_intersection_win->setWindowTitle("Polygon Intersection");
        this->_polygon_intersection_win->show();
    }
    else if(ui->Task->currentText() == "Tracing")
    {
        this->_tracing_win = new tracing();
        this->_tracing_win->setWindowTitle("Tracing");
        this->_tracing_win->resize(1280, 720);
        this->_tracing_win->show();
    }
    else if(ui->Task->currentText() == "Spline")
    {
        this->_spline_win = new spline();
        this->_spline_win->setWindowTitle("Spline");
        this->_spline_win->show();
    }
}

