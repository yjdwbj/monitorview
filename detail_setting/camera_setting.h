#ifndef CAMERA_SETTING_H
#define CAMERA_SETTING_H

#include <QTabBar>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTabWidget>
#include "../global.h"
#include <QCheckBox>


class RecordTime : public QDialog
{
    Q_OBJECT
public:
    explicit RecordTime(QWidget *parent = 0);
    ~RecordTime(){}
private slots:
    void slot_signalbyid(int);
};


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
    QWidget *AppendInformation();

private slots:
    void slot_SearchCameraFromLan();
    void slot_MapSignal(int);

    void slot_Alarm_Widget(int);
    void slot_Btn_Dialog(int);
    void slot_RecordVideo_signals(int);

};

#endif // CAMERA_SETTING_H
