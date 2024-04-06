/********************************************************************************
** Form generated from reading UI file 'QCMDEdit.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCMDEDIT_H
#define UI_QCMDEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_QCmdEdit
{
public:

    void setupUi(QTextEdit *QCmdEdit)
    {
        if (QCmdEdit->objectName().isEmpty())
            QCmdEdit->setObjectName(QString::fromUtf8("QCmdEdit"));
        QCmdEdit->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QCmdEdit->sizePolicy().hasHeightForWidth());
        QCmdEdit->setSizePolicy(sizePolicy);
        QCmdEdit->setMaximumSize(QSize(400, 300));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu Light"));
        font.setPointSize(9);
        QCmdEdit->setFont(font);

        retranslateUi(QCmdEdit);

        QMetaObject::connectSlotsByName(QCmdEdit);
    } // setupUi

    void retranslateUi(QTextEdit *QCmdEdit)
    {
        QCmdEdit->setWindowTitle(QApplication::translate("QCmdEdit", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QCmdEdit: public Ui_QCmdEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCMDEDIT_H
