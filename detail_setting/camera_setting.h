#ifndef CAMERA_SETTING_H
#define CAMERA_SETTING_H

#include <QTabBar>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTabWidget>
#include "../global.h"
#include <QCheckBox>

class CameraSetting : public QDialog
{
    Q_OBJECT
public:
    explicit CameraSetting(const QString &name, QWidget *parent=0);
    ~CameraSetting(){}
private:
    QTabWidget *mainTab;

    QWidget *ConnectInformation(const QString &);
    QWidget *OtherInformation();
    QWidget *Alarm();
    QWidget *RecordVideo();

private slots:
    void slot_SearchCameraFromLan();
    void slot_MapSignal(int);

};

#endif // CAMERA_SETTING_H
