#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "KeenOn.h"
#include <QTimer>
#include "QRos.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc, char** argv,QWidget *parent = nullptr);
    ~MainWindow();

public:
    KeenOn  ko;

    QRos<nav_msgs::OccupancyGrid>   map;
    static void onROSReceiveMap(nav_msgs::OccupancyGrid  &msg, void *);
    void    SetTarget(float x,float y,float q=0);

    // the current position
    QRos<nav_msgs::Odometry> pos;
    static void onROSPose(nav_msgs::Odometry &msg, void *);

    // TF listener
    QTFListen<geometry_msgs::TransformStamped> tflisten;

    // Local map
    QRos<nav_msgs::OccupancyGrid> lmap;
    static void onROSLocalMap(nav_msgs::OccupancyGrid &msg, void *);

    //TF sender
    QTF hgRobot; // from global map to global base_footprint
    static void onRosGlobalPose(void*);

    void Save();
    void SaveMap(BYTE*,int w,int h, char *psz);
    struct
    {
        float wl,wr;
    } speed;

private slots:
    void on_btninit_clicked();
    void on_btnclose_clicked();
    void on_btnleft_clicked();
    void on_btnstop_clicked();
    void on_btnright_clicked();
    void on_btnbackward_clicked();
    void on_btnforward_clicked();
    void onTimer();
    void onUpdateMap(QImage *);
    void on_btnGo_clicked();
    void onTarget(float,float);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
