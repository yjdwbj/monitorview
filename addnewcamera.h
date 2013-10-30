#ifndef ADDNEWCAMERA_H
#define ADDNEWCAMERA_H
#include "global.h"
#include <QDialog>

class SetCameraType : public QDialog
{
    Q_OBJECT
public:
    explicit SetCameraType(QDialog *parent =0);
    ~SetCameraType(){}
};

class AddNewCamera : public QWidget
{
    Q_OBJECT
public:
    explicit AddNewCamera(QWidget *parent=0);
    ~AddNewCamera(){}
};


#endif // ADDNEWCAMERA_H
