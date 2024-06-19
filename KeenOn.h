#ifndef KEENON_H
#define KEENON_H

#include "vSerial.h"
#include "QRos.h"
#include "QTF2.h"
#include "3d/dhVector.h"
#include "std_msgs/Bool.h"

#define MAX_CBUF    10000
#define MAX(a,b)    ((a)>(b) ? (a):(b))
#define MIN(a,b)    ((a)<(b) ? (a):(b))

/*
    KeenOn Hacked Protocol
    aa,aa, ..... 55,55.
    by J. Yang in 2023.
*/

class KeenOn
{
public:
    KeenOn();
public:
    BOOL    Init(char *pUART);
    void    Close();
    void    Move(int wl,int wr,float t);    // it needs Run() and sampling time, dt.
    void    Go(float v,float w);
    void    InitROS(int argc,char **argv);

public:
    // ROS section
    QRos<geometry_msgs::Twist>  tin;    // in
    QRos<nav_msgs::Odometry>    odom;   // out
    QTF hRobot; // from odom to base_link
    QTF hInit;  // from map to odom;
    QTF hScan;  // from base_link to laser_sensor_link
    QTF hPose;

    static void onROSReceiveVel(geometry_msgs::Twist &msg, void*);
    static void onROSSendODOM(void*);
    static void onROSTF(void*);

public:
    // serial communication
    vSerial     m_sx;
    struct
    {
        BYTE        buf[MAX_CBUF];
        int nCnt;
    }cb;

    struct _info
    {
        int     battery;
        int     us;

        struct
        {
            int     x,y;
            float   tx,ty;
        } acc;

        struct
        {
            int     l,r;    // enc current
            int     lo,ro;  // enc old;
            float   t,to;
        } enc;

        struct
        {
            float   q,ax,ay;
            float   t;
        } gyro;

        struct
        {
            float   t,T,dt;
            struct
            {
                float w,wd;
            } l,r;
        } run;

        struct
        {

            float   GR; // gear ratio of wheel
            float   d;  // distance between two wheels
            float   r;  // radius of  wheel
        } geo;

        struct
        {
            float   vel;
            float   omega;
        } speed;

        dhVector X,Xo;
        bool    bReady;
    } info;

    void    Parser(BYTE *,int);
    void    Send(int l,int r);
    void    Run();
    void    EstPose();

    // callback functions.
    static void onReceive(void *);

    bool m_bCmd;
};

#endif // KEENON_H
