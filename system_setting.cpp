#include "system_setting.h"
#include "sqldriver.h"
#include <QTextEdit>
#include <QFontDialog>
#include <QFileDialog>
#include <QSettings>
#include "ui_systemdialog.h"
#include <QStandardItem>


SystemSetting::SystemSetting(QWidget *parent)
    :QDialog(parent),
    ui(new Ui::systemdialog),
    set(new QSettings(qApp->applicationDirPath()+tr("/options.ini"),QSettings::IniFormat))
{
    ui->setupUi(this);

    tw = ui->store_listwidget;


    this->setWindowTitle("系统设置");

    this->setObjectName("SystemSettings");



    QString g("global/");
//    set.setValue(g+"Layout",QString());
    ui->auto_chkautorun->setChecked(set->value(g+"AutoTurn").toBool());
//    set.setValue(g+"AutoTurn",ui->auto_chkautorun->isChecked());
    ui->view_chkstate->setChecked(set->value(g+"WindowsState").toBool());
    ui->store_record->setValue(set->value(g+"SaveRecDay").toInt());
    ui->store_alarm->setValue(set->value(g+"SaveAlarmDay").toInt());
    ui->store_len->setValue(set->value(g+"RecSectionTime").toInt());
    ui->other_edtproxy->setText(set->value(g+"ProxyIP").toString());
    ui->auto_chktray->setChecked( set->value(g+"AutoTray").toBool());
    ui->view_timstamp->setChecked( set->value(g+"ShowTimeStamp").toBool());
    ui->other_sboxalarm->setValue(set->value(g+"ErrAlarm_Time").toInt());
    ui->other_chklosesnd->setChecked(set->value(g+"AlarmSnd").toBool());
    ui->auto_chkconnect->setChecked(set->value(g+"AutoStartDev").toBool());
    ui->other_sboxpage->setValue(set->value(g+"AutoTurnPage").toInt());
//    set.setValue(g+"AutoDecRateNum",QVariant());
//    set.setValue(g+"VideoCache",QVariant());
//    set.setValue(g+"ShowKey",QVariant());
    ui->view_chbtitle->setChecked(set->value(g+"Paint_ShowTitle").toBool());
    ui->view_chkstate->setChecked(set->value(g+"Paint_ShowStatus").toBool());
    ui->view_chkfixed->setChecked(set->value(g+"VideoFixRatio").toBool());
    ui->other_chkupdate->setChecked(set->value(g+"CheckUpate").toBool());
    ui->view_cname->setChecked(set->value(g+"ShowCameraName").toBool());
    ui->view_chklow->setChecked(set->value(g+"RealTimeVideo").toBool());
    int n = 1;

    set->beginGroup(datafolderGroup);
    QStringList dirlist = set->childKeys();

    foreach(const QString &s,dirlist)
    {
      tw->addItem(set->value(s).toString());
    }

    set->endGroup();
}


void SystemSetting::accept()
{
    close();
}

void SystemSetting::closeEvent(QCloseEvent *e)
{
    if(!tw->count())
    {
        QMessageBox::warning(this,"提示","请选择一个存放报警与录像记录的目录");
        e->ignore();
    }
    else
    e->accept();
}

void SystemSetting::on_store_add_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"选择录像存储目录");
    if(path.isEmpty())
        return;
      tw->addItem(path);
}

void SystemSetting::on_auto_chkautorun_toggled(bool checked)
{
    ui->auto_edtpass->setEnabled(checked);
    ui->auto_edtuser->setEnabled(checked);
    ui->auto_chktray->setEnabled(checked);
}

void SystemSetting::on_view_btnfont_clicked()
{
    bool f = false;
    QFont font = QFontDialog::getFont(&f);
    if(f)
    {
        ui->view_edtfont->setFont(font);
    }
}

void SystemSetting::on_pushButton_3_clicked()
{


    QString g("global/");
//    set.setValue(g+"Layout",QString());
    set->setValue(g+"AutoTurn",ui->auto_chkautorun->isChecked());
    set->setValue(g+"WindowsState",ui->view_chkstate->isChecked());
    set->setValue(g+"SaveRecDay",ui->store_record->value());
    set->setValue(g+"SaveAlarmDay",ui->store_alarm->value());
    set->setValue(g+"RecSectionTime",ui->store_len->value());
    set->setValue(g+"ProxyIP",ui->other_edtproxy->text());
    set->setValue(g+"AutoTray",ui->auto_chktray->isChecked());
    set->setValue(g+"ShowTimeStamp",ui->view_timstamp->isChecked());
    set->setValue(g+"ErrAlarm_Time",ui->other_sboxalarm->value());
    set->setValue(g+"AlarmSnd",ui->other_chklosesnd->isChecked());
    set->setValue(g+"AutoStartDev",ui->auto_chkconnect->isChecked());
    set->setValue(g+"AutoTurnPage",ui->other_sboxpage->value());
//    set.setValue(g+"AutoDecRateNum",QVariant());
//    set.setValue(g+"VideoCache",QVariant());
//    set.setValue(g+"ShowKey",QVariant());
    set->setValue(g+"Paint_ShowTitle",ui->view_chbtitle->isChecked());
    set->setValue(g+"Paint_ShowStatus",ui->view_chkstate->isChecked());
    set->setValue(g+"VideoFixRatio",ui->view_chkfixed->isChecked());
    set->setValue(g+"CheckUpate",ui->other_chkupdate->isChecked());
//    set.setValue(g+"Screen",QVariant());
    set->setValue(g+"ShowCameraName",ui->view_cname->isChecked());
    set->setValue(g+"RealTimeVideo",ui->view_chklow->isChecked());

    QString datafolder("DataFolder/");
    set->beginGroup(datafolder);
    QStringList dirlist = set->childKeys();
    foreach(const QString &s,dirlist)
    {
        set->remove(s);   // remove old values.
    }
    set->sync();
    int c = tw->count();
    for(int i = 0 ; i < c;i++)
    {
        set->setValue(QString::number(i),tw->item(i)->text());
    }
    set->endGroup();
    set->sync();

}



void SystemSetting::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(item->row());
}

void SystemSetting::on_store_del_clicked()
{
    tw->takeItem(tw->currentRow());
}
