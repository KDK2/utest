/********************************************************************************
** Form generated from reading UI file 'QMapImage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMAPIMAGE_H
#define UI_QMAPIMAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMapImage
{
public:

    void setupUi(QWidget *QMapImage)
    {
        if (QMapImage->objectName().isEmpty())
            QMapImage->setObjectName(QString::fromUtf8("QMapImage"));
        QMapImage->resize(400, 300);

        retranslateUi(QMapImage);

        QMetaObject::connectSlotsByName(QMapImage);
    } // setupUi

    void retranslateUi(QWidget *QMapImage)
    {
        QMapImage->setWindowTitle(QApplication::translate("QMapImage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QMapImage: public Ui_QMapImage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMAPIMAGE_H
