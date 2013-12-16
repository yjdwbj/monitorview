#ifndef CAMERA_SETTINGS_H
#define CAMERA_SETTINGS_H

#include <QDialog>
#include "global.h"

namespace Ui {
class camera_settings;
class record_time;
}


class RecordTime : public QDialog
{
    Q_OBJECT

public:
    RecordTime(const QString &verifyid, const QString &id,QWidget *parent = 0 );
    ~RecordTime(){delete ui;}
    QStringList m_viewItem;
    QStringList m_sqlItem;
private slots:
    void on_btn_weekSelectNone_clicked();

    void on_btn_weekSelectAll_clicked();

    void on_btn_accept_clicked();

private:
    Ui::record_time *ui;
    QList<QCheckBox*> weeklist;
    QString m_verifyid;

//    void setToView();

};




//class RecordTime : public QDialog
//{
//    Q_OBJECT
//public:
//    explicit RecordTime(QWidget *parent = 0);
//    ~RecordTime(){}
//    QStringList getItemForSql() const { return m_sql;}
//    QString getItemToView() const { return m_itemToView;}
//private slots:
//    void slot_signalbyid(int);
////    void slot_choicebyid(int);
//private:
//    GroupCheckBox *cboxlist;
//    QString m_verifyid;
//    QStringList m_sql;
//    QString m_itemToView;
//    LabAndWidget *start_time;
//    LabAndWidget *end_time;
//};




class camera_settings : public QDialog
{
    Q_OBJECT
    
public:
    explicit camera_settings(int index=0,const QString &name="",QWidget *parent = 0);
    ~camera_settings();
    QString getCameraName() const {return m_cameraName;}
    QString getVerifyId() const {return m_verifyid;}
signals:
    void updateItemValue(QString);
    
private slots:
    void on_btn_search_clicked();

    void on_pushButton_15_clicked();



    void on_rdb_specialtime_toggled(bool checked);

    void on_btn_addtraptime_clicked();

    void on_btn_addrecordtime_clicked();

    void on_chbox_enableAlarm_toggled(bool checked);


    void on_btn_altertraptime_clicked();

    void on_btn_deletetraptime_clicked();

    void on_btn_cleartraptime_clicked();



    void on_chbox_enablerecord_toggled(bool checked);

private:
    Ui::camera_settings *ui;
    Ui::record_time *record;
    QString m_verifyid,m_cameraName;


    void setRecordTab();
    void setAlarmTab();
    QString getStringFromChar(unsigned char c, const QStringList &list);
};

#endif // CAMERA_SETTINGS_H
