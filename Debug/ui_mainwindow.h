/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "QCMD.h"
#include "QCMDEdit.h"
#include "QMapImage.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btninit;
    QPushButton *btnclose;
    QPushButton *btnforward;
    QPushButton *btnleft;
    QPushButton *btnstop;
    QPushButton *btnright;
    QPushButton *btnbackward;
    QCMD *cmd;
    QCMDEdit *out;
    QMapImage *map;
    QPushButton *btnGo;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1293, 761);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        btninit = new QPushButton(centralwidget);
        btninit->setObjectName(QString::fromUtf8("btninit"));
        btninit->setGeometry(QRect(10, 10, 89, 25));
        btnclose = new QPushButton(centralwidget);
        btnclose->setObjectName(QString::fromUtf8("btnclose"));
        btnclose->setGeometry(QRect(110, 10, 89, 25));
        btnforward = new QPushButton(centralwidget);
        btnforward->setObjectName(QString::fromUtf8("btnforward"));
        btnforward->setGeometry(QRect(160, 60, 141, 71));
        btnleft = new QPushButton(centralwidget);
        btnleft->setObjectName(QString::fromUtf8("btnleft"));
        btnleft->setGeometry(QRect(10, 140, 141, 71));
        btnstop = new QPushButton(centralwidget);
        btnstop->setObjectName(QString::fromUtf8("btnstop"));
        btnstop->setGeometry(QRect(160, 140, 141, 71));
        btnright = new QPushButton(centralwidget);
        btnright->setObjectName(QString::fromUtf8("btnright"));
        btnright->setGeometry(QRect(310, 140, 141, 71));
        btnbackward = new QPushButton(centralwidget);
        btnbackward->setObjectName(QString::fromUtf8("btnbackward"));
        btnbackward->setGeometry(QRect(160, 220, 141, 71));
        cmd = new QCMD(centralwidget);
        cmd->setObjectName(QString::fromUtf8("cmd"));
        cmd->setGeometry(QRect(10, 300, 221, 181));
        out = new QCMDEdit(centralwidget);
        out->setObjectName(QString::fromUtf8("out"));
        out->setGeometry(QRect(240, 300, 211, 181));
        map = new QMapImage(centralwidget);
        map->setObjectName(QString::fromUtf8("map"));
        map->setGeometry(QRect(470, 0, 800, 800));
        btnGo = new QPushButton(centralwidget);
        btnGo->setObjectName(QString::fromUtf8("btnGo"));
        btnGo->setGeometry(QRect(10, 490, 121, 41));
        MainWindow->setCentralWidget(centralwidget);
        cmd->raise();
        btninit->raise();
        btnclose->raise();
        btnforward->raise();
        btnstop->raise();
        btnright->raise();
        btnbackward->raise();
        btnleft->raise();
        out->raise();
        map->raise();
        btnGo->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1293, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        btninit->setText(QApplication::translate("MainWindow", "Init", nullptr));
        btnclose->setText(QApplication::translate("MainWindow", "Close", nullptr));
        btnforward->setText(QApplication::translate("MainWindow", "Forward", nullptr));
        btnleft->setText(QApplication::translate("MainWindow", "Left", nullptr));
        btnstop->setText(QApplication::translate("MainWindow", "Stop", nullptr));
        btnright->setText(QApplication::translate("MainWindow", "Right", nullptr));
        btnbackward->setText(QApplication::translate("MainWindow", "Backward", nullptr));
        btnGo->setText(QApplication::translate("MainWindow", "Go", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
