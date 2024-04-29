#include "stdafx.h"
#include "KeenOn.h"
#include <math.h>
#include <QDebug>

// Geometrical parameters

WORD CRC16 (const BYTE *nData, WORD wLength);

KeenOn::KeenOn()
{
    cb.nCnt = 0;
    info.run.dt     = 1;    // sampling time
    memset(&info,0,sizeof(info));

    info.geo.GR = 4000;
    info.geo.d  = 0.3;
    info.geo.r  = 0.159/2.;
    info.bReady = FALSE;
    info.X.z    = RAD(0);
    info.Xo = info.X;
}

BOOL KeenOn::Init(char *pUART)
{
    return m_sx.Open(pUART,115200,onReceive, this);
}

void KeenOn::Close()
{
    m_sx.Close();
}

void KeenOn::onReceive(void *pArg)
{
    KeenOn *pKO = (KeenOn*)pArg;
    BYTE *p     = pKO->m_sx.GetBuffer();
    int n       = pKO->m_sx.GetLength();

    pKO->Parser(p,n);
}

void KeenOn::Parser(BYTE *p, int n)
{
    // buffering
    int m   = cb.nCnt+n;
    if (m>MAX_CBUF)
    {
        cb.nCnt = MAX_CBUF;
        memmove(cb.buf, &cb.buf[n], cb.nCnt-n);
        memmove(&cb.buf[cb.nCnt-n],p,n);
    }
    else
    {
        memmove(&cb.buf[cb.nCnt],p,n);
        cb.nCnt+=n;
    }

    // protocol
    BOOL bEncUpdated = FALSE;
    p   = cb.buf;
    for (int i=0;i<cb.nCnt-10;i++)
    if (p[i]==0xaa)     // find stx
    if (p[i+1]==0xaa)
    if (p[i+2]==0)
    if (p[i+3]==0xe0)
    {
        DWORD dw;
        BYTE cmd = cb.buf[i+4];

        // use length field or not
        // we will not use length field first..
        for (int j=i;j<cb.nCnt-1;j++)
        if (p[j]==0x55 && p[j+1]==0x55) // find etx
        {
            BYTE buf[5000];
            n   = j+2-i;
            n   = MIN(n,1000);
            memmove(buf,&cb.buf[i],n);

            // check CRC16
            WORD crc = CRC16(&buf[2],n-6);
            BYTE c1,c2;
            c1  = buf[n-4];
            c2  = buf[n-3];

            if ( (crc&0x00ff)==c1 && (crc>>8)==c2) ;
            else
            {
                // data corruption
                break;
            }

            // command
            switch(cmd){
            case 0x2e:  // unknowns.
            case 0x2b:
            case 0x30:
            case 0x31:
            case 0x41:  // hearbeat?..
                break;
            case 0x20:  info.battery    = buf[8];   break;
            case 0x2a:  info.us         = buf[8];   break;
            case 0x2d:  // acc
                memmove(&info.acc.x, &buf[8],2);
                memmove(&info.acc.y, &buf[10],2);
                memmove(&dw, &buf[12],4);   info.acc.tx    = ((float)dw)/1000.;
                memmove(&dw, &buf[16],4);   info.acc.ty    = ((float)dw)/1000.;
                break;
            case 0x2f:  // enc
                if (!bEncUpdated)
                {
                    info.enc.lo     = info.enc.l;
                    info.enc.ro     = info.enc.r;
                    info.enc.to     = info.enc.t;
                    bEncUpdated = TRUE;
                }
                memmove(&info.enc.l, &buf[8],4);
                memmove(&info.enc.r, &buf[12],4);                
                memmove(&dw, &buf[16],4);   info.enc.t   = ((float)dw)/1000.;
                break;
            case 0x32:
                memmove(&info.gyro.q, &buf[8],4);
                memmove(&info.gyro.ax, &buf[12],4);
                memmove(&info.gyro.ay, &buf[16],4);
                memmove(&dw, &buf[20],4);   info.gyro.t     = ((float)dw)/1000.;
                break;
            }

            // remove buffer and reset parser
            memmove(cb.buf, &cb.buf[j+2],cb.nCnt-(j+2));
            cb.nCnt-=(j+2);
            i   = -1;
            break;
        }
    }

    if (bEncUpdated)    EstPose();
}

// case 1) t>0 : smooth stop
// case 2) t=-1: keep speed with wl,wr.
void KeenOn::Move(int wl,int wr,float t)
{
    info.run.t  = 0;
    info.run.T  = t;
    info.run.l.wd   = wl;
    info.run.r.wd   = wr;
}

void KeenOn::Go(float v,float w)
{
    float wl = (v-info.geo.d*w/2)/info.geo.r;
    float wr = (v+info.geo.d*w/2)/info.geo.r;

    float K=4;
    Move(wl*K,wr*K,1);
    //qDebug()<<wl<<wr;
}

void KeenOn::Run()
{
    if (info.run.t>info.run.T)
    {
        // stop
        if (fabs(info.run.l.w)>0.1 || fabs(info.run.r.w)>0.1)
        {
            info.run.l.w    = 0;
            info.run.r.w    = 0;
            Send(0,0);
        }
        else
        {
            // KeenOn automatically stops by own damping.
        }
        return;
    }

    // deceleration at 90%
    if (info.run.T>0)
    if (info.run.t>info.run.T*0.9)
    {
        info.run.l.wd   = 0;
        info.run.r.wd   = 0;
    }

    // Smooth speed control
    // Remind that Send(,) uses integer speed.
    float Kp = 0.1;
    info.run.l.w    += (info.run.l.wd-info.run.l.w)*Kp;
    info.run.r.w    += (info.run.r.wd-info.run.r.w)*Kp;
    Send(info.run.l.w,info.run.r.w);

    info.run.t  += info.run.dt;
}

void KeenOn::Send(int l,int r)
{
    BYTE b[20];
    b[0] = 0xaa;
    b[1] = 0xaa;
    b[2] = 0;
    b[3] = 0x62;
    b[4] = 0x26;
    b[5] = 0;
    b[6] = 0x04;
    b[7] = 0;

    // 4bytes from b[6];
    b[8] = 0;
    b[9] = 0;
    b[10]= 0;
    b[11]= 0;

    if (l>0)	b[8] = 1;
    if (r>0)	b[10]= 1;
    b[9]	= fabs(l);
    b[11]	= fabs(r);

    WORD w = CRC16(&b[2], 10);
    b[12] = w &0x00ff;
    b[13] = w>>8 &0xff;

    // etx
    b[14]	= 0x55;
    b[15]	= 0x55;

    m_sx.Write(b,16);
}

void KeenOn::EstPose()
{
    double wl,wr;
    double dt   = info.enc.t-info.enc.to;
    if (dt<=0)  return;

    wl  = (info.enc.l-info.enc.lo)/dt;
    wr  = (info.enc.r-info.enc.ro)/dt;
    if (wl>0)
    {
        int a;
        a++;
    }
    wl  = wl/info.geo.GR/(2*PI);
    wr  = wr/info.geo.GR/(2*PI);

    double c,s,q;
    q   = info.X.z;
    c   = cos(q);
    s   = sin(q);

    double dx,dy,dq;
    dx  = info.geo.r/2.*c*(wl+wr);
    dy  = info.geo.r/2.*s*(wl+wr);
    dq  = info.geo.r/2.*(-wl+wr)/(info.geo.d/2.);

    info.Xo     = info.X;
    info.X      = info.Xo + dhVector(dx,dy,dq);
    info.speed.vel=(wl+wr)/2.0;
    info.speed.omega=(wr-wl)/(info.geo.d*2.0);
    //qDebug()<<info.X.x <<info.X.y <<DEG(info.X.z);
}


WORD CRC16 (const BYTE *nData, WORD wLength)
{
    static const WORD wCRCTable[] = {
    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

    BYTE nTemp;
    WORD wCRCWord = 0xFFFF;

    while (wLength--) {
        nTemp = *nData++ ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord ^= wCRCTable[nTemp];
    }
    return wCRCWord;
}

void KeenOn::InitROS(int argc, char **argv)
{
    tin.onReceive = onROSReceiveVel;
    tin.SetReceiver("cmd_vel");
    tin.Init(argc,argv,"peanut",this);

    //odom.bBroadcast = TRUE;
    odom.SetFPS(50);
    odom.SetSender("odom");
    odom.onLoop = onROSSendODOM;
    odom.Init(argc,argv,"peanut",this);

    // from odom to base_link
    hRobot.SetFPS(50);
    hRobot.TF("odom","base_footprint");
    hRobot.Init(argc,argv,"peanut","tf_odom",this, onROSTF);
}

//--------------------------------
//
//  ROS setting by QRos.h
//  by J. Yang in 2023.
//
//---------------------------------
using namespace geometry_msgs;
void KeenOn::onROSReceiveVel(geometry_msgs::Twist &msg, void *pArg)
{
    KeenOn *ko =(KeenOn*)pArg;
    qDebug()<<msg.linear.x << msg.angular.z;
    //ko->Go(msg.linear.x,msg.angular.z);
}

#include "3d/dhMat.h"
#include "3d/dhQuat.h"

void KeenOn::onROSSendODOM(void *pArg)
{
    KeenOn *ko =(KeenOn*)pArg;

    nav_msgs::Odometry odom;
    odom.header.stamp = ros::Time::now();
    odom.header.frame_id    = "odom";
    odom.child_frame_id     = "base_footprint";

    // odom position by vector,X.
    dhVector o  = ko->info.X;
    dhMat h;
    h   = h.Trans(o.x,o.y,0)*h.RotZ(o.z);
    dhQuat q    = h.Q();

    odom.pose.pose.position.x   = o.x;
    odom.pose.pose.position.y   = o.y;
    odom.pose.pose.position.z   = 0;
    odom.pose.pose.orientation.x    = q.x;
    odom.pose.pose.orientation.y    = q.y;
    odom.pose.pose.orientation.z    = q.z;
    odom.pose.pose.orientation.w    = q.q;

    // odom twist by difference of X-Xo
    dhVector d  = ko->info.X-ko->info.Xo;
    float vel = ko->info.speed.vel;
    float omega = ko->info.speed.omega;
    odom.twist.twist.linear.x   = vel;
    odom.twist.twist.linear.y   = 0;
    odom.twist.twist.linear.z   = 0;
    odom.twist.twist.angular.x  = 0;
    odom.twist.twist.angular.y  = 0;
    odom.twist.twist.angular.z  = omega;

    ko->odom.Send(odom);
}

void KeenOn::onROSTF(void *pArg)
{
    KeenOn *ko = (KeenOn*)pArg;
    dhVector o  = ko->info.X;
    dhMat h;
    h   = h.Trans(o.x,o.y,0)*h.RotZ(o.z);
    ko->hRobot.Send(h);
}
