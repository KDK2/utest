#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argc,argv);        //<--for ROS
    w.show();
    return a.exec();
}
