#include "QCMD.h"
#include "vToken.h"

QCMD::QCMD(QWidget *parent) : QCMDEdit(parent)
{
}

#include "mainwindow.h"
void QCMD::onRun(char *p)
{
    vToken tok(p);
    tok.SetSeparator(", \n");

    if (tok.GetSize()==0)   return;

    MainWindow *pMF = (MainWindow*)pParent;
    vString stCmd = (char*)tok[0];

    // Command
    // s: stop
    // f or f,speed: forward
    // b or b,speed: backward
    // l or l,speed: left
    // r or r,speed: right
    // ex) f
    //     f,3

    // default speed
    float v = 5;
    float t = 10000;

    switch(tok.GetSize()){
    case 1:
        if (stCmd=="f")    pMF->ko.Move(v,v,t);
        if (stCmd=="b")    pMF->ko.Move(-v,-v,t);
        if (stCmd=="l")    pMF->ko.Move(-v,v,t);
        if (stCmd=="r")    pMF->ko.Move(v,-v,t);
        if (stCmd=="s")    pMF->ko.Move(0,0,1);
        if (stCmd=="save") pMF->Save();
        break;
    case 2:
        v   = tok.f(1);
        if (stCmd=="f")    pMF->ko.Move(v,v,t);
        if (stCmd=="b")    pMF->ko.Move(-v,-v,t);
        if (stCmd=="l")    pMF->ko.Move(-v,v,t);
        if (stCmd=="r")    pMF->ko.Move(v,-v,t);
        if (stCmd=="s")    pMF->ko.Move(0,0,1);
        break;

    case 3:
        if (stCmd=="vw")
        {
            float v = tok.f(1);
            float w = tok.f(2);

            // Warning!!: v = m/s, w=rad/s
            if (fabs(v)<1 && fabs(w)<1)
            pMF->ko.Go(v,w);
        }
        break;
    case 4:
        if (stCmd=="go")
            pMF->SetTarget(tok.f(1),tok.f(2),tok.f(3));
        break;
    }
}
