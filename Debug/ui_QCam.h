/********************************************************************************
** Form generated from reading UI file 'QCam.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCAM_H
#define UI_QCAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QCam
{
public:

    void setupUi(QWidget *QCam)
    {
        if (QCam->objectName().isEmpty())
            QCam->setObjectName(QString::fromUtf8("QCam"));
        QCam->resize(400, 300);

        retranslateUi(QCam);

        QMetaObject::connectSlotsByName(QCam);
    } // setupUi

    void retranslateUi(QWidget *QCam)
    {
        QCam->setWindowTitle(QApplication::translate("QCam", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QCam: public Ui_QCam {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCAM_H
