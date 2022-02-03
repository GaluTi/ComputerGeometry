#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("CompGem");
    w.resize(300, 50);
    w.show();
    return a.exec();
}
