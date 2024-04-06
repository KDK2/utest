QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Robot Library
INCLUDEPATH+=../../lib
LIBS+= -lrt

LIBS+= -L"$$PWD/../../lib/amGL/jpeg" -ljpeg
LIBS+= -L"$$PWD/../../lib/amGL/png" -lpng
LIBS+= -L"$$PWD/../../lib/amGL/zlib" -lz

# ROS
INCLUDEPATH+=/opt/ros/noetic/include    # --> QRos.h
LIBS+=-L/opt/ros/noetic/lib
LIBS+= -lrostime -lroscpp -ltf2_ros -lrosconsole -lroscpp_serialization
LIBS+= -lxmlrpcpp -lactionlib -ltf2 -lrosconsole_backend_interface
LIBS+= -lrosconsole_log4cxx -lcpp_common
LIBS+= -lactionlib -lboost_thread


QMAKE_LFLAGS += -no-pie

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../lib/3d/dhMat.cpp \
    ../../lib/3d/dhQuat.cpp \
    ../../lib/3d/dhVector.cpp \
    ../../lib/Elapsed.cpp \
    ../../lib/GTimer.cpp \
    ../../lib/KDTp.cpp \
    ../../lib/PTRList.cpp \
    ../../lib/QCMDEdit.cpp \
    ../../lib/QCam.cpp \
    ../../lib/SMMgr.cpp \
    ../../lib/ShMem.cpp \
    ../../lib/afx.cpp \
    ../../lib/vMutex.cpp \
    ../../lib/vParser.cpp \
    ../../lib/vSerial.cpp \
    ../../lib/vShell.cpp \
    ../../lib/vString.cpp \
    ../../lib/vToken.cpp \
    ../../lib/amGL/CImage.cpp \
    KeenOn.cpp \
    QCMD.cpp \
    QMapImage.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../lib/3d/dhMat.h \
    ../../lib/3d/dhQuat.h \
    ../../lib/3d/dhVector.h \
    ../../lib/Elapsed.h \
    ../../lib/GTimer.h \
    ../../lib/KDTp.h \
    ../../lib/NetInfo.h \
    ../../lib/PTRList.h \
    ../../lib/QAct.h \
    ../../lib/QCMDEdit.h \
    ../../lib/QCam.h \
    ../../lib/QRos.h \
    ../../lib/QTF.h \
    ../../lib/QTF2.h \
    ../../lib/SMMgr.h \
    ../../lib/ShMem.h \
    ../../lib/stdafx.h \
    ../../lib/vArray.h \
    ../../lib/vMutex.h \
    ../../lib/vParser.h \
    ../../lib/vSerial.h \
    ../../lib/vShell.h \
    ../../lib/vString.h \
    ../../lib/vToken.h \
    ../../lib/xRGB.h \
    ../../lib/amGL/CImage.h \
    KeenOn.h \
    QCMD.h \
    QMapImage.h \
    mainwindow.h

FORMS += \
    ../../lib/QCMDEdit.ui \
    ../../lib/QCam.ui \
    QMapImage.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
