#include "alarmtrapsettings.h"
#include "ui_alarmtrapsettings.h"
#include "sqldriver.h"

AlarmTrapSettings::AlarmTrapSettings(const QString &verifyid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlarmTrapSettings)
{
    ui->setupUi(this);
    m_verifyid = verifyid;
    setWindowTitle("添加布防时间段");
    ui->stackedWidget->currentWidget()->setEnabled(false);ui->stackedWidget->currentWidget()->setEnabled(false);
    weeklist << ui->cbox_w1 << ui->cbox_w2 << ui->cbox_w2 << ui->cbox_w3 << ui->cbox_w4
                << ui->cbox_w5 << ui->cbox_w6 << ui->cbox_w7;
    conditionlist << ui->cbox_codition1 << ui->cbox_condition2 << ui->cbox_condition3
                     << ui->cbox_condition4 << ui->cbox_condition5;


    readDataToWidget();
}

void AlarmTrapSettings::readDataToWidget()
{
    QStringList list = SqlInstance::getRowFirst("alarm","verifyid",m_verifyid);
    setCharToList(list.at(1),weeklist);
    ui->tedt_starttime->setTime(QTime::fromString(list.at(2)));
    ui->tedt_endtime->setTime(QTime::fromString(list.at(3)));
    ui->ledt_continue->setText(list.at(4));
    setCharToList(list.at(5),conditionlist);


}


void AlarmTrapSettings::setCharToList(const QString &str, QList<QCheckBox *> &list)
{
    char c = 0;
    memcpy(&c,str.toLocal8Bit().data(),1);
    int n = list.count();
    foreach(QCheckBox *b , list)
    {
        b->setChecked(c & (1 << n--));
    }
}

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
    QString w = getCharFromList(weeklist);
    SqlInstance::insertItem("alarm",QStringList() <<
                            m_verifyid << w
                            << ui->tedt_starttime->text() << ui->tedt_endtime->text()
                            << ui->ledt_continue->text()
                            << getCharFromList(conditionlist));
    QString wek;
    unsigned char c;
    memcpy(&c,w.toLocal8Bit().data(),1);
    if(c == 254)
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


    m_alarmlist << wek << ui->tedt_starttime->text() << ui->tedt_endtime->text() << d.join(',');


}

QString AlarmTrapSettings::getCharFromList(const QList<QCheckBox*> &list)
{
    char c = 0;
    int n = list.count();
    foreach(QCheckBox *b , list)
    {
        c |= (b->isChecked() ? 1 : 0) << n--;
    }
    return QString::fromLocal8Bit(&c);
}



