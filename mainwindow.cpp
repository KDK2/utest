#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ros/ros.h"

MainWindow::MainWindow(int argc, char **argv,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<QImage*>("QImage*");

    ui->setupUi(this);
    ui->cmd->pParent    = this;

    QTimer *tmr = new QTimer(this);
    connect(tmr, SIGNAL(timeout()), this, SLOT(onTimer()));
    tmr->start(50);

    speed.wl    = 0;
    speed.wr    = 0;

    ko.InitROS(argc,argv);  //<--for ROS

    map.onReceive = onROSReceiveMap;
    map.SetReceiver("map");
    map.Init(argc,argv,"peanut)",this);

    pos.onReceive   = onROSPose;
    pos.SetFPS(100);
    pos.SetReceiver("odom");
    pos.Init(argc,argv,"peanut_pos",this);

    lmap.onReceive  = onROSLocalMap;
    lmap.SetFPS(100);
    lmap.SetReceiver("/costmap_node/costmap/costmap");
    lmap.Init(argc,argv,"peanut_local",this);

    connect(ui->map, SIGNAL(onTarget(float,float)), this, SLOT(onTarget(float,float)));

    tflisten.SetFPS(100);
    tflisten.TF("map,odom,base_footprint");
    tflisten.Init(argc,argv,"ddd","tfodom",this);

    hgRobot.SetFPS(100);
    hgRobot.TF("gmap","gbase_footprint");
    hgRobot.Init(argc,argv,"pe","tf_global",this, onRosGlobalPose);
}

#include <tf2_ros/transform_listener.h>

void MainWindow::onROSReceiveMap(nav_msgs::OccupancyGrid &msg, void *pArg)
{
    int w = msg.info.width;
    int h = msg.info.height;

    if (w!=4000 || h!=4000) return;

    MainWindow *pMF = (MainWindow*)pArg;

    QImage *p   = new QImage(w,h,QImage::Format_Grayscale8);
    memmove(p->bits(),msg.data.data(),w*h);
    QMetaObject::invokeMethod(pMF,"onUpdateMap",Qt::QueuedConnection,Q_ARG(QImage*,p));
    //pMF->UpdateMap((BYTE*)msg.data.data(l),w,h);
}

void MainWindow::onUpdateMap(QImage *p)
{
    ui->map->UpdateMap((BYTE*)p->bits(),p->width()*p->height());
    delete p;
}

MainWindow::~MainWindow()
{
    pos.Close();
    ko.Close();
    delete ui;
}

void MainWindow::on_btninit_clicked()
{
    ko.Init("/dev/ttyUSB0");
}

void MainWindow::on_btnclose_clicked()
{
    ko.Close();
}

void MainWindow::on_btnforward_clicked()
{
    ko.Move(15,15,10000);
}

void MainWindow::on_btnbackward_clicked()
{
    ko.Move(-15,-15,10000);
}

void MainWindow::on_btnleft_clicked()
{
    ko.Move(-15,15,10000);
}

void MainWindow::on_btnright_clicked()
{
    ko.Move(15,-15,10000);
}

void MainWindow::on_btnstop_clicked()
{
    ko.Move(0,0,1.0);   // smooth stop
    //ko.Move(0,0,0.0); // sudden stop
}

void MainWindow::onTimer()
{
    ko.Run();

    // Display
    if (ko.m_sx.IsConnected())
    {
        dhVector o  = ko.info.X;
        ui->out->PutLine("%4.2f,%4.2f,(%4.1f)", o.x,o.y,DEG(o.z));
    }
}

#include "vShell.h"
void MainWindow::Save()
{
    QImage *p   = &ui->map->info.map;

    int w   = p->width();
    int h   = p->height();

    ui->map->SaveMap(ui->map->info.map.bits(),w,h,"map/map");
}

void MainWindow::on_btnGo_clicked()
{
    if (ui->map->info.bLock==false)
    {
        ui->map->Lock();
        ui->btnGo->setText("Press Map.");
    }
}

#include "QAct.h"
void MainWindow::onTarget(float x,float y)
{
    ui->map->Unlock();
    ui->btnGo->setText("Go");

    SetTarget(x,y);

    ui->out->PutLine("Go %4.2f,%4.2f",x,y);
}

void MainWindow::SetTarget(float x, float y, float d)
{
    dhMat h;
    h   = h.RotZ(RAD(d));
    dhQuat q(h);

    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x    = x;
    goal.target_pose.pose.position.y    = y;
    goal.target_pose.pose.position.z    = 0;
    goal.target_pose.pose.orientation.x = q.x;
    goal.target_pose.pose.orientation.y = q.y;
    goal.target_pose.pose.orientation.z = q.z;
    goal.target_pose.pose.orientation.w = q.q;
}
#include <QDebug>
using namespace nav_msgs;
void MainWindow::onROSPose(nav_msgs::Odometry &msg, void *pArg)
{
    //when send odom to base_footprint by KeenOn
    if (msg.child_frame_id.compare("base_footprint")==0)
    {
        MainWindow *pMF = (MainWindow*)pArg;

        dhMat h = pMF->tflisten.Get();
        dhVector o  = h.O();
        dhVector rpy= h.RPY();

        pMF->ui->map->info.o.Vector(o.x,-o.y,DEG(-rpy.z));
        pMF->update();//call repaint
    }
}

void MainWindow::onRosGlobalPose(void *pArg)
{
    MainWindow* pMF = (MainWindow*)pArg;
    dhMat h = pMF->tflisten.Get();
    pMF->hgRobot.Send(h);
}

void MainWindow::onROSLocalMap(nav_msgs::OccupancyGrid &msg, void *pArg)
{
    MainWindow *pMF = (MainWindow*)pArg;
    int w   = msg.info.width;
    int h   = msg.info.height;

    int cx  = pMF->ui->map->info.lmap.width();
    int cy  = pMF->ui->map->info.lmap.height();

    if (cx!=w || cy!=h)
        pMF->ui->map->info.lmap = QImage(w,h,QImage::Format_Grayscale8);

    // flpped image
    BYTE* pt    = pMF->ui->map->info.lmap.bits();
    pt  +=w*(h-1);
    for (int i=0;i<h;i++)
    {
        memmove(pt,&msg.data[w*i],w);
        pt-=w;
    }
    //memmove(pMF->ui->map->info.lmap.bits(),&msg.data[0], w*h);

    float x = msg.info.origin.position.x;
    float y = msg.info.origin.position.y;
    dhQuat q;
    q.x     = msg.info.origin.orientation.x;
    q.y     = msg.info.origin.orientation.y;
    q.z     = msg.info.origin.orientation.z;
    q.q     = msg.info.origin.orientation.w;
    dhMat H(q);
    dhVector rpy    = H.RPY();
    pMF->ui->map->info.lo.Vector(x,y,DEG(rpy.z));

    pMF->ui->map->update();
}
