#ifndef ALARMTRAPSETTINGS_H
#define ALARMTRAPSETTINGS_H
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFileDialog>

#include <QDialog>

namespace Ui {
class AlarmTrapSettings;
}

class AlarmTrapSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit AlarmTrapSettings(const QString &verifyid,QWidget *parent = 0);
    ~AlarmTrapSettings();
    QStringList getAlarmList() const {return m_alarmlist;}
    
private slots:
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_btn_weekSelectNone_clicked();

    void on_btn_weekSelectAll_clicked();

    void on_btn_conditionSelectNone_clicked();

    void on_btn_coditionSelectAll_clicked();

    void on_btn_openaudiofile_clicked();

    void on_rad_audiofile_toggled(bool checked);

    void on_rad_audio_toggled(bool checked);

    void on_chbox_audioreadtxt_toggled(bool checked);

    void on_chbox_enablesky_toggled(bool checked);

    void on_chbox_skysendmsg_toggled(bool checked);

    void on_chbox_enablecall_toggled(bool checked);

    void on_pushButton_14_clicked();

private:
    Ui::AlarmTrapSettings *ui;
    QList<QCheckBox*> weeklist;
    QList<QCheckBox*> conditionlist;

    QString m_verifyid;
    QStringList m_alarmlist;

    QString getCharFromList(const QList<QCheckBox*> &list);
    void setCharToList(const QString &str, QList<QCheckBox*> &list);
    void readDataToWidget();

};

#endif // ALARMTRAPSETTINGS_H
