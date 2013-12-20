#include "alarmtrapsettings.h"
#include "ui_alarmtrapsettings.h"
#include "sqldriver.h"
#include "global.h"
#include <QtSerialPort/QtSerialPort> // must be qt 5.1 or later





AlarmTrapSettings::AlarmTrapSettings(const QString &filename,
                                     const QString &id, const QString &verifyid,
                                      const QString &oldvalue , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlarmTrapSettings)
{
    ui->setupUi(this);   // must be first line. don't add anything before this line. otherwise report error.
    initalPairValue();
    set = new QSettings(filename,QSettings::IniFormat);
    m_verifyid = verifyid;
    setWindowTitle("添加布防时间段");
    ui->stackedWidget->currentWidget()->setEnabled(false);

    for(int i = 1 ; i < 8;i++)
        weeklist << ui->gbox_week->findChild<QCheckBox*>("cbox_w"+QString::number(i));

    for(int i = 1 ; i < 6;i++)
        conditionlist << ui->gbox_triggeraction->findChild<QCheckBox*>("cbox_condition"+QString::number(i));



//    readDataToWidget(id);


      foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
          ui->cbbox_msgcom->addItem(info.portName());
      }
      initalThisWidget(oldvalue);

}


void AlarmTrapSettings::initalThisWidget(const QString &value)
{
    QStringList tlist = value.split('?',QString::SkipEmptyParts);
    if(tlist.first().isEmpty())
        return;
    setCharToList(tlist.first().toInt(),weeklist);
    ui->tedt_starttime->setTime(QTime::fromString(tlist.at(1).section('~',0,0)));
    ui->tedt_endtime->setTime(QTime::fromString(tlist.at(1).section('~',1,1)));
    setCharToList(tlist.at(2).toInt(),conditionlist);
    if(tlist.size()>4)
    {
        QStringList allist = tlist.at(3).split('#',QString::SkipEmptyParts);
        foreach(const QString &s,allist)
        {
             readINIConfig(s.toInt());
        }
        ui->ledt_continue->setText(tlist.at(4));
    }
    else
    {
        ui->ledt_continue->setText(tlist.at(3));
    }



}

void AlarmTrapSettings::readDataToWidget(const QString &id)
{
//    QStringList list = SqlInstance::QuerySqlFromString(
//                "select * from alarm where verifyid == ? and sort_id == ?",
//                QStringList() << m_verifyid << id);
//    setCharToList(list.at(2).toInt(),weeklist);
//    ui->tedt_starttime->setTime(QTime::fromString(list.at(3),"hh:mm:ss"));
//    ui->tedt_endtime->setTime(QTime::fromString(list.at(4),"hh:mm:ss"));
//    ui->ledt_continue->setText(list.at(5));
//    setCharToList(list.at(6).toInt(),conditionlist);

}


void AlarmTrapSettings::readINIConfig(int id)  // read ini to widgets
{
    initalPairValue();


    switch (id) {
    case E_Snd:
        if(getPairValue(A_Snd.Enable).toBool())
        {
            ui->tableWidget->item(0,0)->setCheckState(Qt::Checked);
            bool f = getPairValue(A_Snd.SndFrom).toBool();
             ui->rad_audio->setChecked(!f);
             ui->rad_audiofile->setChecked(f);
             if(!f)
             {
                ui->cbbox_audiotype->setCurrentText(getPairValue(A_Snd.Sound).toString());
             }
             else
             {
                 ui->ledt_audiofile->setText(getPairValue(A_Snd.Sound).toString());
             }
             ui->chbox_speech->setChecked(getPairValue(A_Snd.Speech).toBool());
             ui->cbbox_voice->setCurrentIndex(getPairValue(A_Snd.Voice).toInt());

        }
        break;
    case E_Video:
        if(getPairValue(A_SaveVideo).toBool())
        {
            ui->tableWidget->item(1,0)->setCheckState(Qt::Checked);
        }
        break;
    case E_Pic:
        if(getPairValue(A_SavePic.Enable).toBool())
        {
            ui->tableWidget->item(2,0)->setCheckState(Qt::Checked);
            ui->sbox_photocount->setValue(getPairValue(A_SavePic.Num).toInt());
            ui->sbox_Interval->setValue(getPairValue(A_SavePic.Interval).toInt());
        }

        break;
    case E_Ftp:
        if(getPairValue(A_Ftp.Enable).toBool())
        {
            ui->tableWidget->item(3,0)->setCheckState(Qt::Checked);
            ui->cbbox_ftpuploadtype->setCurrentIndex(getPairValue(A_Ftp.UploadType).toInt());
            ui->ledt_ftphost->setText(getPairValue(A_Ftp.Host).toString());
            ui->ledt_ftpport->setText(getPairValue(A_Ftp.Port).toString());
            ui->ledt_ftpuser->setText(getPairValue(A_Ftp.User).toString());
            ui->ledt_ftppasswd->setText(getPairValue(A_Ftp.Pwd).toString());
            ui->ledt_ftphomeDir->setText(getPairValue(A_Ftp.Dir).toString());
            ui->chbox_pasvmode->setChecked(getPairValue(A_Ftp.PASV).toBool());
        }

        break;
    case E_Email:
        if(getPairValue(A_Email.Enable).toBool())
        {
            ui->tableWidget->item(4,0)->setCheckState(Qt::Checked);
            ui->ledt_mailreceiver->setText(getPairValue(A_Email.Recv).toString());
            ui->cbbox_mailAttach->setCurrentIndex(getPairValue(A_Email.FileType).toInt());
            ui->cbbox_mailssl->setCurrentIndex(getPairValue(A_Email.SSL).toInt());
            ui->ledt_mailpasswd->setText(getPairValue(A_Email.Pwd).toString());
            ui->ledt_mailport->setText(getPairValue(A_Email.Port).toString());
            ui->ledt_mailsendbox->setText(getPairValue(A_Email.SendBox).toString());
            ui->ledt_mailsmtphost->setText(getPairValue(A_Email.SMTP).toString());
            ui->ledt_mailuser->setText(getPairValue(A_Email.User).toString());
        }
        break;
    case E_Msg:
        if(getPairValue(A_Msg.Enable).toBool())
        {
            ui->tableWidget->item(5,0)->setCheckState(Qt::Checked);
            ui->cbbox_msgcom->setCurrentIndex(getPairValue(A_Msg.ComPort).toInt());
            ui->ledt_msgphone->setText(getPairValue(A_Msg.TelNumber).toString());
            ui->chbox_msggap->setChecked(getPairValue(A_Msg.Interval).toBool());
            ui->sbox_msgtime->setValue(getPairValue(A_Msg.Second).toInt());
        }
        break;
    case E_DialUp:
        if(getPairValue(A_DialUp.Enable).toBool())
        {
            ui->tableWidget->item(6,0)->setCheckState(Qt::Checked);
            ui->cbbox_dialtype->setCurrentIndex(getPairValue(A_DialUp.DevType).toInt());
            ui->ledt_dialphone->setText(getPairValue(A_DialUp.TelNumer).toString());
        }
        break;
    case E_Skype:
        if(getPairValue(A_Skype.Enable).toBool())
        {
            ui->tableWidget->item(7,0)->setCheckState(Qt::Checked);
            ui->chbox_skyenable->setChecked(getPairValue(A_Skype.EnableMsg).toBool());
            ui->chbox_skycall->setChecked(getPairValue(A_Skype.EnableCall).toBool());
            ui->chbox_skysendmsg->setChecked(getPairValue(A_Skype.EnableMsg).toBool());
            ui->ledt_skyacct->setText(getPairValue(A_Skype.SkyAcct).toString());
            ui->txt_skymsg->setText(getPairValue(A_Skype.MsgTxt).toString());
            ui->ledt_skycallphone->setText(getPairValue(A_Skype.CallNumber).toString());
            ui->ledt_skyphonenumber->setText(getPairValue(A_Skype.TelNumber).toString());
        }

        break;
    case E_PTZ:
        if(getPairValue(A_PTZ.Enable).toBool())
        {
            ui->tableWidget->item(8,0)->setCheckState(Qt::Checked);
            ui->sbox_prepostionnum->setValue(getPairValue(A_PTZ.PTZ).toInt());
        }

        break;
    case E_Web:
        if(getPairValue(A_Web.Enable).toBool())
        {
            ui->tableWidget->item(9,0)->setCheckState(Qt::Checked);
            ui->ledt_weblink->setText(getPairValue(A_Web.Web).toString());
        }
        break;
    case E_App:
        if(getPairValue(A_App.Enable).toBool())
        {
            ui->tableWidget->item(10,0)->setCheckState(Qt::Checked);
            ui->ledt_appPath->setText(getPairValue(A_App.AppPath).toString());
            ui->ledt_apparg->setText( getPairValue(A_App.AppArg).toString());
            ui->chbox_apponce->setChecked(getPairValue(A_App.Once).toBool());
        }
        break;
    default:
        break;
    }










}

AlarmTrapSettings::~AlarmTrapSettings()
{
    delete ui;
}


void AlarmTrapSettings::initalPairValue()
{
//    A_SndLst << qMakePair(tr("A_Snd/Enable"),QVariant()) << qMakePair(tr("A_Snd/SndFrom"),QVariant())
//                << qMakePair(tr("A_Snd/Sound"),QVariant()) << qMakePair(tr("A_Snd/Speech"),QVariant())
//                   << qMakePair(tr("A_Snd/Msg"),QVariant()) << qMakePair(tr("A_Snd/Voice"),QVariant());
//    A_SaveVideoLst << qMakePair(tr("A_SaveVideo/Enable"),QVariant()) << qMakePair(tr("A_SavePic/Enable"),QVariant())
//                << qMakePair(tr("A_SavePic/Interval"),QVariant()) << qMakePair(tr("A_SavePic/Num"),QVariant());

//    A_FtpLst << qMakePair(tr("A_Ftp/UploadType"),QVariant()) << qMakePair(tr("A_Ftp/Host"),QVariant())
//                << qMakePair(tr("A_Ftp/Port"),QVariant()) << qMakePair(tr("A_Ftp/User"),QVariant())
//                   << qMakePair(tr("A_Ftp/Pwd"),QVariant()) << qMakePair(tr("A_Ftp/Dir"),QVariant())
//                      << qMakePair(tr("A_Ftp/PASV"),QVariant());

//    A_FtpLst << qMakePair(tr("A_Email/Recv"),QVariant()) << qMakePair(tr("A_Email/FileType"),QVariant())
//                << qMakePair(tr("A_Email/SMTP"),QVariant()) << qMakePair(tr("A_Email/Port"),QVariant())
//                   << qMakePair(tr("A_Email/Auth"),QVariant()) << qMakePair(tr("A_Email/SSL"),QVariant())
//                      << qMakePair(tr("A_Email/User"),QVariant()) << qMakePair(tr("A_Email/Pwd"),QVariant())
//                      <<   qMakePair(tr("A_Email/SendBox"),QVariant());




    QString sound;
    if(ui->rad_audio->isChecked())
        sound = ui->cbbox_audiotype->currentText();
    else
        sound = ui->ledt_audiofile->text();


    A_Snd= {qMakePair(tr("A_Snd/Enable"),
            QVariant(ui->tableWidget->item(E_Snd,0)->checkState()==Qt::Checked ? 1 : 0)),
            qMakePair(tr("A_Snd/SndFrom"),QVariant(ui->rad_audio->isChecked() ? 0 : 1)),
            qMakePair(tr("A_Snd/Sound"),QVariant(sound)),
            qMakePair(tr("A_Snd/Speech"),QVariant(ui->chbox_speech->isChecked())),
            qMakePair(tr("A_Snd/Msg"),QVariant(ui->txt_speechText->document()->toPlainText())),
            qMakePair(tr("A_Snd/Voice"),QVariant(ui->cbbox_voice->currentIndex()))};

    A_SaveVideo = qMakePair(tr("A_SaveVideo/Enable"),
    QVariant(ui->tableWidget->item(E_Video,0)->checkState()==Qt::Checked ? 1 : 0));

    A_SavePic= {
        qMakePair(tr("A_SavePic/Enable"),
        QVariant(ui->tableWidget->item(E_Pic,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_SavePic/Interval"),QVariant(ui->sbox_Interval->value())),
        qMakePair(tr("A_SavePic/Num"),QVariant(ui->sbox_photocount->value()))
    };

    A_Ftp = {
        qMakePair(tr("A_Ftp/Enable"),
        QVariant(ui->tableWidget->item(E_Ftp,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_Ftp/UploadType"),QVariant(ui->cbbox_ftpuploadtype->currentIndex())),
        qMakePair(tr("A_Ftp/Host"),QVariant(ui->ledt_ftphost->text())),
        qMakePair(tr("A_Ftp/Port"),QVariant(ui->ledt_ftpport->text())),
        qMakePair(tr("A_Ftp/User"),QVariant(ui->ledt_ftpuser->text())),
        qMakePair(tr("A_Ftp/Pwd"),QVariant(ui->ledt_ftppasswd->text())),
        qMakePair(tr("A_Ftp/Dir"),QVariant(ui->ledt_ftphomeDir->text())),
        qMakePair(tr("A_Ftp/PASV"),QVariant(ui->chbox_pasvmode->isChecked()))
    };

    A_Email = {
        qMakePair(tr("A_Email/Enable"),
        QVariant(ui->tableWidget->item(E_Email,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_Email/Recv"),QVariant(ui->ledt_mailreceiver->text())),
        qMakePair(tr("A_Email/Attachment"),QVariant(ui->cbbox_mailAttach->currentIndex())),
        qMakePair(tr("A_Email/SMTP"),QVariant(ui->ledt_mailsmtphost->text())),
        qMakePair(tr("A_Email/Port"),QVariant(ui->ledt_mailport->text())),
        qMakePair(tr("A_Email/Auth"),QVariant(ui->chbox_mailauth->isChecked())),
        qMakePair(tr("A_Email/SSL"),QVariant(ui->cbbox_mailssl->currentIndex())),
        qMakePair(tr("A_Email/User"),QVariant(ui->ledt_mailuser->text())),
        qMakePair(tr("A_Email/Pwd"),QVariant(ui->ledt_mailpasswd->text())),
        qMakePair(tr("A_Email/SendBox"),QVariant(ui->ledt_mailsendbox->text()))
    };

    A_Msg = {
        qMakePair(tr("A_Msg/Enable"),
        QVariant(ui->tableWidget->item(5,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_Msg/ComPort"),QVariant(ui->cbbox_msgcom->currentIndex())),
        qMakePair(tr("A_Msg/TelNumber"),QVariant(ui->ledt_msgphone->text())),
        qMakePair(tr("A_Msg/Interval"),QVariant(ui->chbox_msggap->isChecked())),
        qMakePair(tr("A_Msg/Second"),QVariant(ui->sbox_msgtime->value()))
    };

    A_DialUp = {
        qMakePair(tr("A_DialUp/Enable"),
        QVariant(ui->tableWidget->item(6,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_DialUp/DevType"),QVariant(ui->cbbox_dialtype->currentIndex())),
        qMakePair(tr("A_DialUp/TelNumer"),QVariant(ui->ledt_dialphone->text()))
    };

    A_Skype = {
        qMakePair(tr("A_Skype/Enable"),
        QVariant(ui->tableWidget->item(7,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_Skype/EnableMsg"),QVariant(ui->chbox_skyenable->isChecked())),
        qMakePair(tr("A_Skype/SkyAcct"),QVariant(ui->ledt_skyacct->text())),
        qMakePair(tr("A_Skype/MsgTxt"),QVariant(ui->txt_skymsg->document()->toPlainText())),
        qMakePair(tr("A_Skype/EnableSMS"),QVariant(ui->chbox_skysendmsg->isChecked())),
        qMakePair(tr("A_Skype/TelNumber"),QVariant(ui->ledt_skyphonenumber->text())),
        qMakePair(tr("A_Skype/EnableCall"),QVariant(ui->chbox_skycall->isChecked())),
        qMakePair(tr("A_Skype/CallNumber"),QVariant(ui->ledt_skycallphone->text()))
    };

    A_PTZ = {
        qMakePair(tr("A_PTZ/Enable"),
        QVariant(ui->tableWidget->item(8,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_PTZ/PTZnum"),QVariant(ui->sbox_prepostionnum->value()))
    };

    A_Web = {
        qMakePair(tr("A_Web/Enable"),
        QVariant(ui->tableWidget->item(9,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_Web/WebPage"),QVariant(ui->ledt_weblink->text()))
    };


    A_App = {
        qMakePair(tr("A_App/Enable"),
        QVariant(ui->tableWidget->item(10,0)->checkState()==Qt::Checked ? 1 : 0)),
        qMakePair(tr("A_App/AppPath"),QVariant(ui->ledt_appPath->text())),
        qMakePair(tr("A_App/AppArg"),QVariant(ui->ledt_apparg->text())),
        qMakePair(tr("A_App/Once"),QVariant(ui->chbox_apponce->text()))
    };

}





void AlarmTrapSettings::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(item->row());
    if(Qt::Unchecked == item->checkState())
        ui->stackedWidget->currentWidget()->setEnabled(false);
    else
    {
         ui->stackedWidget->currentWidget()->setEnabled(true);
    }

}

void AlarmTrapSettings::on_btn_weekSelectNone_clicked()
{
    foreach(QCheckBox *c , weeklist)
        c->setChecked(false);
}

void AlarmTrapSettings::on_btn_weekSelectAll_clicked()
{
    foreach(QCheckBox *c , weeklist)
        c->setChecked(true);
}

void AlarmTrapSettings::on_btn_conditionSelectNone_clicked()
{
    foreach(QCheckBox *c , conditionlist)
        c->setChecked(false);
}

void AlarmTrapSettings::on_btn_coditionSelectAll_clicked()
{
    foreach(QCheckBox *c , conditionlist)
        c->setChecked(true);
}

void AlarmTrapSettings::on_btn_openaudiofile_clicked()
{
    QString path = QFileDialog::getOpenFileName( this,"选择录像声音文件");
    if(path.isEmpty())
        return;
    ui->ledt_audiofile->setText(path);
}

void AlarmTrapSettings::on_rad_audiofile_toggled(bool checked)
{
    ui->ledt_audiofile->setEnabled(checked);
    ui->btn_openaudiofile->setEnabled(checked);
}

void AlarmTrapSettings::on_rad_audio_toggled(bool checked)
{
    ui->cbbox_audiotype->setEnabled(checked);
}

void AlarmTrapSettings::on_chbox_skyenable_toggled(bool checked)
{
    ui->ledt_skyacct->setEnabled(checked);
    ui->txt_skymsg->setEnabled(checked);
}



void AlarmTrapSettings::on_chbox_skysendmsg_toggled(bool checked)
{
     ui->ledt_skyphonenumber->setEnabled(checked);
     ui->txt_skymsg->setEnabled(checked);
}


void AlarmTrapSettings::writeSettings(int id)
{

    switch (id) {
    case E_Snd:
        setPairValue(A_Snd.Enable);
        setPairValue(A_Snd.Msg);
        setPairValue(A_Snd.SndFrom);
        setPairValue(A_Snd.Sound);
        setPairValue(A_Snd.Speech);
        setPairValue(A_Snd.Voice);
        break;
    case E_Video:
        setPairValue(A_SaveVideo);
        break;
    case E_Pic:
        setPairValue(A_SavePic.Enable);
        setPairValue(A_SavePic.Interval);
        setPairValue(A_SavePic.Num);
        break;

    case E_Ftp:
        setPairValue(A_Ftp.Enable);
        setPairValue(A_Ftp.Dir);
        setPairValue(A_Ftp.Host);
        setPairValue(A_Ftp.Port);
        setPairValue(A_Ftp.User);
        setPairValue(A_Ftp.Pwd);
        setPairValue(A_Ftp.UploadType);
        setPairValue(A_Ftp.PASV);
        break;
    case E_Email:
        setPairValue(A_Email.Enable);
        setPairValue(A_Email.FileType);
        setPairValue(A_Email.Port);
        setPairValue(A_Email.SMTP);
        setPairValue(A_Email.Auth);
        setPairValue(A_Email.User);
        setPairValue(A_Email.Pwd);
        setPairValue(A_Email.Recv);
        setPairValue(A_Email.SendBox);
        setPairValue(A_Email.SSL);
        break;
    case E_Msg:
        setPairValue(A_Msg.Enable);
        setPairValue(A_Msg.ComPort);
        setPairValue(A_Msg.TelNumber);
        setPairValue(A_Msg.Interval);
        setPairValue(A_Msg.Second);
        break;
    case E_DialUp:
        setPairValue(A_DialUp.Enable);
        setPairValue(A_DialUp.DevType);
        setPairValue(A_DialUp.TelNumer);
        break;

    case E_Skype:
        setPairValue(A_Skype.Enable);
        setPairValue(A_Skype.EnableMsg);
        setPairValue(A_Skype.SkyAcct);
        setPairValue(A_Skype.MsgTxt);
        setPairValue(A_Skype.EnableSMS);
        setPairValue(A_Skype.TelNumber);
        setPairValue(A_Skype.EnableCall);
        setPairValue(A_Skype.CallNumber);
        break;
    case E_PTZ:
        setPairValue(A_PTZ.Enable);
        setPairValue(A_PTZ.PTZ);
        break;
    case E_Web:
        setPairValue(A_Web.Enable);
        setPairValue(A_Web.Web);
        break;

    case E_App:
        setPairValue(A_App.Enable);
        setPairValue(A_App.AppPath);
        setPairValue(A_App.AppArg);
        setPairValue(A_App.Once);
        break;
    default:
        break;
    }

    set->sync();






}



void AlarmTrapSettings::on_pushButton_14_clicked()
{
    initalPairValue();
    int w = getCharFromList(weeklist);
//    int count = SqlInstance::getMaximumId("alarm","id");
//    SqlInstance::insertItem("alarm",QStringList() << QString::number(count+1) <<
//                            m_verifyid << w
//                            << ui->tedt_starttime->text() << ui->tedt_endtime->text()
//                            << ui->ledt_continue->text()
//                            << getCharFromList(conditionlist));
    for(int i = 0 ; i < ui->tableWidget->rowCount();i++)
    {
        QTableWidgetItem *item =    ui->tableWidget->item(i,0);
        if(item->isSelected())
        {

        }
    }


    QString wek;

    if(w == 254)   // 254 == 11111110
    {
        wek.append("每天");
    }
    else
    {
        QStringList t;
        foreach(const QCheckBox *s,weeklist)
        {
            if(s->isChecked())
            t.append(s->text().right(1));
        }
        wek  = t.join(',');
    }
    QStringList d;
    foreach(const QCheckBox *s,conditionlist)
    {
        if(s->isChecked())
        d.append(s->text());
    }

    int count = ui->tableWidget->rowCount();
    QStringList mlist = actionMapList.split(',');
    QString actions;
    for(int i = 0 ; i < count ;i++)
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Checked)
        {
            actions.append(mlist.at(i));
            writeSettings(i);
        }
    }
    m_iniArg  << QString::number(w) << ui->tedt_starttime->text()+"~"+ui->tedt_endtime->text()
    << QString::number(getCharFromList(conditionlist)) << actions << ui->ledt_continue->text();
     set->setValue(tr("Global/AlarmTimeSec"),QVariant(m_iniArg.join("?")));
     set->sync();

    m_alarmsql <<
                  m_verifyid << QString::number(w)
                  << ui->tedt_starttime->text() << ui->tedt_endtime->text()
                  << ui->ledt_continue->text()
                  << QString::number(getCharFromList(conditionlist));
    m_alarmlist << wek << ui->tedt_starttime->text() << ui->tedt_endtime->text() << d.join(',');




}





void AlarmTrapSettings::on_chbox_speech_toggled(bool checked)
{
   ui->txt_speechText->setEnabled(checked);
}

void AlarmTrapSettings::on_chbox_msggap_toggled(bool checked)
{
    ui->sbox_msgtime->setEnabled(checked);
}


