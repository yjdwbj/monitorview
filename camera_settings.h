#ifndef CAMERA_SETTINGS_H
#define CAMERA_SETTINGS_H

#include <QDialog>

namespace Ui {
class camera_settings;
}

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

    void on_cbox_enablerecord_toggled(bool checked);

    void on_rdb_specialtime_toggled(bool checked);

    void on_btn_addtraptime_clicked();

    void on_btn_addrecordtime_clicked();

    void on_chbox_enableAlarm_toggled(bool checked);


private:
    Ui::camera_settings *ui;
    QString m_verifyid,m_cameraName;


    void setRecordTab();
    QString getStringFromChar(unsigned char c, const QStringList &list);
};

#endif // CAMERA_SETTINGS_H
