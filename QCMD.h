#ifndef QCMD_H
#define QCMD_H

#include <QWidget>
#include "QCMDEdit.h"

class QCMD : public QCMDEdit
{
    Q_OBJECT
public:
    explicit QCMD(QWidget *parent = nullptr);
    virtual void onRun(char *);
    QWidget *pParent;
signals:

};

#endif // QCMD_H
