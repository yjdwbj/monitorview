#ifndef ALARMTRAPSETTINGS_H
#define ALARMTRAPSETTINGS_H
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFileDialog>
#include <QSettings>
#include <QPair>

#include <QDialog>

namespace Ui {
class AlarmTrapSettings;
}

class AlarmTrapSettings : public QDialog
{
    Q_OBJECT
public:
    enum ActionEnum{
        E_Snd,
        E_Video,
        E_Pic,
        E_Ftp,
        E_Email,
        E_Msg,
        E_DialUp,
        E_Skype,
        E_PTZ,
        E_Web,
        E_App,
    };
    explicit AlarmTrapSettings(const QString &filename,
                               const QString &verifyid,const QString &id,
                               const QString &oldvalue=QString(), QWidget *parent = 0);
    ~AlarmTrapSettings();
    QStringList getAlarmList() const {return m_alarmlist;}
    QStringList getAlarmSql() const {return m_alarmsql;}
    QStringList getIniItem() const {return m_iniArg;}
    
private slots:
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_btn_weekSelectNone_clicked();

    void on_btn_weekSelectAll_clicked();

    void on_btn_conditionSelectNone_clicked();

    void on_btn_coditionSelectAll_clicked();

    void on_btn_openaudiofile_clicked();

    void on_rad_audiofile_toggled(bool checked);

    void on_rad_audio_toggled(bool checked);

    void on_chbox_skysendmsg_toggled(bool checked);


    void on_pushButton_14_clicked();

    void writeSettings(int id);

    void on_chbox_speech_toggled(bool checked);

    void on_chbox_msggap_toggled(bool checked);

    void on_chbox_skyenable_toggled(bool checked);

private:
    Ui::AlarmTrapSettings *ui;
    QList<QCheckBox*> weeklist;
    QList<QCheckBox*> conditionlist;

    QString m_verifyid;
    QStringList m_alarmlist;
    QStringList m_alarmsql;
    QStringList  m_iniArg;

    typedef QPair<QString,QVariant> pairvalue;

    struct {
        pairvalue Enable;
        pairvalue SndFrom;
        pairvalue Sound;
        pairvalue Speech;
        pairvalue Msg;
        pairvalue Voice;
    }A_Snd;

    struct {
        pairvalue  Enable;
        pairvalue  Interval;
        pairvalue  Num;

    }A_SavePic;

    struct{
        pairvalue Enable;
        pairvalue UploadType;
        pairvalue Host;
        pairvalue Port;
        pairvalue User;
        pairvalue Pwd;
        pairvalue Dir;
        pairvalue PASV;
    }A_Ftp;

    struct{
        pairvalue Enable;
        pairvalue Recv;
        pairvalue FileType;
        pairvalue SMTP;
        pairvalue Port;
        pairvalue Auth;
        pairvalue SSL;
        pairvalue User;
        pairvalue Pwd;
        pairvalue SendBox;
    }A_Email;

    struct{
        pairvalue Enable;
        pairvalue ComPort;
        pairvalue TelNumber;
        pairvalue Interval;
        pairvalue Second;
    }A_Msg;

    struct {
        pairvalue Enable;
        pairvalue DevType;
        pairvalue TelNumer;
    }A_DialUp;

    struct{
        pairvalue Enable;
        pairvalue EnableMsg;
        pairvalue SkyAcct;
        pairvalue MsgTxt;
        pairvalue EnableSMS;
        pairvalue TelNumber;
        pairvalue EnableCall;
        pairvalue CallNumber;
    }A_Skype;

    struct{
        pairvalue Enable;
        pairvalue AppPath;
        pairvalue AppArg;
        pairvalue Once;
    }A_App;

//    static QList<pairvalue> A_SndLst;
//     QList<pairvalue> A_SaveVideoLst;
//     QList<pairvalue> A_FtpLst;
//     QList<pairvalue> A_EmailLst;
//     QList<pairvalue> A_MsgLst;
//     QList<pairvalue> A_DialUpLst;
//     QList<pairvalue> A_SkypeLst;

    struct {
        pairvalue Enable;
        pairvalue PTZ;
    }A_PTZ;
    struct {
        pairvalue Enable;
        pairvalue Web;
    }A_Web;

     pairvalue A_SaveVideo;



//     QList<pairvalue> A_AppLst;

    QSettings *set;
    void initalPairValue();

    template <typename Tf, typename Ts>
    Q_INLINE_TEMPLATE void setPairValue(const QPair<Tf, Ts> &p) {set->setValue(p.first,p.second);}
    template <typename Tf, typename Ts>
    Q_INLINE_TEMPLATE QVariant getPairValue(const QPair<Tf, Ts> &p) {return set->value(p.first);}



//    unsigned char getCharFromList(const QList<QCheckBox*> &list);
//    void setCharToList(unsigned char c, QList<QCheckBox*> &list);
    void readDataToWidget(const QString &id);
    void readINIConfig(int id);
    void initalThisWidget(const QString &value);


};

#endif // ALARMTRAPSETTINGS_H
