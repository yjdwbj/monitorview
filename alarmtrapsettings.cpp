#include "alarmtrapsettings.h"
#include "ui_alarmtrapsettings.h"
#include "sqldriver.h"
#include "global.h"
#include <QtAlgorithms>

AlarmTrapSettings::AlarmTrapSettings(const QString &verifyid, const QString &id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlarmTrapSettings)
{
    ui->setupUi(this);
    m_verifyid = verifyid;
    setWindowTitle("添加布防时间段");
    ui->stackedWidget->currentWidget()->setEnabled(false);
    ui->stackedWidget->currentWidget()->setEnabled(false);
    for(int i = 1 ; i < 8;i++)
        weeklist << ui->gbox_week->findChild<QCheckBox*>("cbox_w"+QString::number(i));

//    int count = weeklist.count();
//    int size = weeklist.size();



    for(int i = 1 ; i < 6;i++)
        conditionlist << ui->gbox_triggeraction->findChild<QCheckBox*>("cbox_condition"+QString::number(i));

    readDataToWidget(id);
}

void AlarmTrapSettings::readDataToWidget(const QString &id)
{
    QStringList list = SqlInstance::QuerySqlFromString(
                "select * from alarm where verifyid == ? and sort_id == ?",
                QStringList() << m_verifyid << id);
    setCharToList(list.at(2).toInt(),weeklist);
    ui->tedt_starttime->setTime(QTime::fromString(list.at(3),"hh:mm:ss"));
    ui->tedt_endtime->setTime(QTime::fromString(list.at(4),"hh:mm:ss"));
    ui->ledt_continue->setText(list.at(5));
    setCharToList(list.at(6).toInt(),conditionlist);


}


//void AlarmTrapSettings::setCharToList(unsigned char c, QList<QCheckBox *> &list)
//{

//    int n = list.size();
//    foreach(QCheckBox *b , list)
//    {
//        b->setChecked(c & (1 << n--));
//    }
//}

//unsigned char  AlarmTrapSettings::getCharFromList(const QList<QCheckBox*> &list)
//{
//    unsigned char c = 0;
//    int n = list.size();
//    foreach(QCheckBox *b , list)
//    {
//        c |= (b->isChecked() ? 1 : 0) << n--;
//    }
//    return c;
//}


AlarmTrapSettings::~AlarmTrapSettings()
{
    delete ui;
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

void AlarmTrapSettings::on_chbox_audioreadtxt_toggled(bool checked)
{
    ui->tedt_audioText->setEnabled(checked);
}

void AlarmTrapSettings::on_chbox_enablesky_toggled(bool checked)
{
    ui->ledt_skyacct->setEnabled(checked);
    ui->txt_skymsg->setEnabled(checked);
}

void AlarmTrapSettings::on_chbox_skysendmsg_toggled(bool checked)
{
     ui->ledt_skyphonenumber->setEnabled(checked);
     ui->txt_skymsg->setEnabled(checked);
}

void AlarmTrapSettings::on_chbox_enablecall_toggled(bool checked)
{
    ui->ledt_skycallphone->setEnabled(checked);
}

void AlarmTrapSettings::on_pushButton_14_clicked()
{
    int w = getCharFromList(weeklist);
//    int count = SqlInstance::getMaximumId("alarm","id");
//    SqlInstance::insertItem("alarm",QStringList() << QString::number(count+1) <<
//                            m_verifyid << w
//                            << ui->tedt_starttime->text() << ui->tedt_endtime->text()
//                            << ui->ledt_continue->text()
//                            << getCharFromList(conditionlist));


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

    m_alarmsql <<
                  m_verifyid << QString::number(w)
                  << ui->tedt_starttime->text() << ui->tedt_endtime->text()
                  << ui->ledt_continue->text()
                  << QString::number(getCharFromList(conditionlist));
    m_alarmlist << wek << ui->tedt_starttime->text() << ui->tedt_endtime->text() << d.join(',');


}




