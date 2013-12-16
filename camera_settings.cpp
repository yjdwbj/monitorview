#include "camera_settings.h"
#include "ui_camera_settings.h"
#include "ui_record_time.h"
#include "lansearchcamera.h"
#include "sqldriver.h"
#include "setting_panel.h"
#include "viewframe.h"
#include "alarmtrapsettings.h"
class ViewFrame;
class CameraView;




RecordTime::RecordTime(const QString &verifyid, const QString &id,QWidget *parent)
    :QDialog(parent),
    ui(new Ui::record_time)
{
    m_verifyid = verifyid;
    ui->setupUi(this);
    for(int i = 1 ; i < 8;i++)
        weeklist << ui->gbox_week->findChild<QCheckBox*>("cbox_w"+QString::number(i));

    QStringList list = SqlInstance::QuerySqlFromString(
                "select * from alarm where verifyid == ? and sort_id == ?",
                QStringList() << m_verifyid << id);
    setCharToList(list.at(2).toInt(),weeklist);
    ui->tedt_starttime->setTime(QTime::fromString(list.at(3)));
    ui->tedt_endtime->setTime(QTime::fromString(list.at(4)));

}



void RecordTime::on_btn_weekSelectNone_clicked()
{
    foreach(QCheckBox *c,weeklist)
    {
        c->setChecked(false);
    }
}

void RecordTime::on_btn_weekSelectAll_clicked()
{
    foreach(QCheckBox *c,weeklist)
    {
        c->setChecked(true);
    }
}


void RecordTime::on_btn_accept_clicked()
{

    int w = getCharFromList(weeklist);

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

    m_viewItem << wek << QString(ui->tedt_starttime->text() + " - " + ui->tedt_endtime->text());
    m_sqlItem << m_verifyid << wek <<  ui->tedt_starttime->text() << ui->tedt_endtime->text();
}




camera_settings::camera_settings(int index, const QString &name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::camera_settings)
{
    ui->setupUi(this);

    this->setWindowTitle("摄像机设置");
    ui->edt_cameraName->setText(name);
    ui->tab_Widget->setCurrentIndex(index);
    if(!name.isEmpty())
    {


        QStringList camera = SqlInstance::getRowFirst("camera_settings","camera_name",name);

        m_verifyid = camera.at(3);
        if(!camera.isEmpty())
        {
            ui->edt_user->setText(camera.at(4));
            ui->edt_passwd->setText(camera.at(5));
            QStringList l = SqlInstance::getColumnsList("hostinfo",QStringList() << "address" << "port",
                              "host_id",camera.at(2));
            if(!l.isEmpty())
            {
                ui->edt_ipaddress->setText("http://" + l.first().replace(",",":"));
            }
        }
         ui->chbox_enableAlarm->setChecked(!camera.at(12).compare("0") ? false:true );
    }
    setRecordTab();
    setAlarmTab();
}

camera_settings::~camera_settings()
{
    delete ui;
    delete record;
}

void camera_settings::setRecordTab()
{
     QStringList recordlist =  SqlInstance::getRowList("record","verifyid",m_verifyid);
     if(recordlist.size() < 1)
         return;

     foreach( const QString &line,recordlist)
     {
         int row = ui->tablewidget_recordtime->rowCount();
         ui->tablewidget_recordtime->insertRow(row);
         QStringList record = line.split(',');

         QString wek;

         int c = record.at(3).toInt();
         wek =  c == 254 ?  QString("每天"): getStringFromChar(c,weekNum.split(','));
         ui->tableWidget_alarm->setItem(row,0,new QTableWidgetItem(wek));
         ui->tablewidget_recordtime->setItem(row,1,
                                             new QTableWidgetItem(record.at(4) + " - " + record.at(5)));


     }
}


void camera_settings::setAlarmTab()
{

   QStringList alarmlist =  SqlInstance::getRowList("alarm","verifyid",m_verifyid);
   if(alarmlist.size() <1)
       return;
   foreach(const QString &line,alarmlist)
   {
       int row = ui->tableWidget_alarm->rowCount();
       ui->tableWidget_alarm->insertRow(row);

       QStringList record = line.split(',');

       QString wek;

       int c = record.at(2).toInt();
       wek =  c == 254 ?  QString("每天"): getStringFromChar(c,weekNum.split(','));

       ui->tableWidget_alarm->setItem(row,0,new QTableWidgetItem(wek));

       ui->tableWidget_alarm->setItem(row,1,new QTableWidgetItem(record.at(3)));
       ui->tableWidget_alarm->setItem(row,2,new QTableWidgetItem(record.at(4)));

        c = record.at(6).toInt();
       ui->tableWidget_alarm->setItem(row,3,
                                      new QTableWidgetItem(getStringFromChar(c,alarmCondition.split(','))));

   }

}

QString camera_settings::getStringFromChar(unsigned char c, const QStringList &list)
{
    QStringList t;
    int n = list.size();
    foreach(const QString &str,list)
    {
        if (c & (1 << n--))
            t.append(str);
    }
    return t.join(',');
}


void camera_settings::on_btn_search_clicked()
{
    LanSearchCamera *lan = new LanSearchCamera;
    lan->exec();
    delete lan;
}




void camera_settings::on_pushButton_15_clicked()
{

    m_cameraName = ui->edt_cameraName->text();
    SqlInstance::updateItem("camera_settings","camera_name",
                            m_cameraName,"camera_verifyid",m_verifyid);
    SqlInstance::updateItem("camera_settings","camera_login_passwd",
                            ui->edt_passwd->text(),"camera_verifyid",m_verifyid);
    SqlInstance::updateItem("camera_settings","camera_login_name",
                            ui->edt_user->text(),"camera_verifyid",m_verifyid);
    SqlInstance::updateItem("camera_settings","camera_alarm",
                            ui->chbox_enableAlarm->isChecked() ? "1" : "0",
                            "camera_verifyid",m_verifyid);
    SqlInstance::updateItem("camera_settings","camera_record",
                            ui->chbox_enablerecord->isChecked() ? "1" : "0",
                            "camera_verifyid",m_verifyid);



    QWidgetList aw = QApplication::allWidgets();
    int i = 0;
    foreach(QWidget *w,aw)
    {
        if(!w->objectName().compare("ViewFrame"))
        {
            ViewFrame *v = (ViewFrame*)w;
            v->updateItem(m_verifyid,m_cameraName);
            i++;
        }else if(!w->objectName().compare("ViewCamera"))
        {
            CameraView *c = (CameraView*)w;
            c->updateItem(m_verifyid,m_cameraName);
            i++;
        }
        if(i>1)
            break;
    }

}




void camera_settings::on_chbox_enablerecord_toggled(bool checked)
{
    ui->rdb_alltimerecord->setEnabled(checked);
    ui->rdb_specialtime->setEnabled(checked);
}




void camera_settings::on_rdb_specialtime_toggled(bool checked)
{
     ui->tablewidget_recordtime->setEnabled(checked);
     ui->btn_addrecordtime->setEnabled(checked);
     ui->btn_alterrecordtime->setEnabled(checked);
     ui->btn_clearrecordtime->setEnabled(checked);
     ui->btn_deletetrecordtime->setEnabled(checked);
}



void camera_settings::on_btn_addtraptime_clicked() // add new item
{


    AlarmTrapSettings *al = new AlarmTrapSettings(m_verifyid,
                                                  QString::number(ui->tableWidget_alarm->currentRow()));
    if(al->exec())
    {
        int num = ui->tableWidget_alarm->rowCount();
        ui->tableWidget_alarm->insertRow(num);
        int col = 0;
        QStringList llist = al->getAlarmSql();
        QString id = QString::number(SqlInstance::getMaximumId("alarm","id")+1);
        llist.prepend(id);
        llist.append(QString::number(num));
        SqlInstance::insertItem("alarm",llist);
        foreach(const QString &str,al->getAlarmList())
        {
            QTableWidgetItem *n = new QTableWidgetItem(str);
            ui->tableWidget_alarm->setItem(num,col++,n);
        }
    }

}

void camera_settings::on_btn_altertraptime_clicked() // alter item
{
    AlarmTrapSettings *al = new AlarmTrapSettings(m_verifyid,
                                                  QString::number(ui->tableWidget_alarm->currentRow()));
    if(al->exec())
    {
        int num = ui->tableWidget_alarm->currentRow();
        QString nstr = QString::number(num);
        QStringList llist =  al->getAlarmSql();


        QStringList idl = SqlInstance::QuerySqlFromString("select id from alarm where verifyid == ? and sort_id == ?",
                                                          QStringList () << m_verifyid << nstr);
        llist.prepend(idl.first());
        llist.append(nstr);

        SqlInstance::deleteRecordByCondition("alarm","id",idl.first());
        SqlInstance::insertItem("alarm",llist);
        int col = 0;
        foreach(const QString &str,al->getAlarmList())
        {
            QTableWidgetItem *n = new QTableWidgetItem(str);
            ui->tableWidget_alarm->setItem(num,col++,n);
        }
    }
}


void camera_settings::on_btn_deletetraptime_clicked()   // delete item
{
    int row = ui->tableWidget_alarm->currentRow();
    QStringList idl = SqlInstance::QuerySqlFromString("select id from alarm where verifyid == ? and sort_id == ?",
                                                      QStringList () << m_verifyid << QString::number(row));
    SqlInstance::deleteRecordByCondition("alarm","id",idl.first());
    ui->tableWidget_alarm->removeRow(row);
}



void camera_settings::on_btn_cleartraptime_clicked()   // clear all item
{
    ui->tableWidget_alarm->setCurrentItem(ui->tableWidget_alarm->item(0,0));
    int row ;
    while( (row = ui->tableWidget_alarm->currentRow()) >= 0)
    {
        QStringList idl = SqlInstance::QuerySqlFromString("select id from alarm where verifyid == ? and sort_id == ?",
                                                          QStringList () << m_verifyid << QString::number(row));
        SqlInstance::deleteRecordByCondition("alarm","id",idl.first());
        ui->tableWidget_alarm->removeRow(row);
    }
}

void camera_settings::on_btn_addrecordtime_clicked()
{
    RecordTime *record = new RecordTime(m_verifyid,
                                       QString::number(ui->tablewidget_recordtime->currentRow()));
    if(record->exec())
    {
        int num = ui->tablewidget_recordtime->rowCount();
        ui->tablewidget_recordtime->insertRow(num);
        int col = 0;
        QStringList llist = record->m_sqlItem;
        QString id = QString::number(SqlInstance::getMaximumId("record","record_id")+1);
        llist.prepend(id);
        llist.append(QString::number(num));
        SqlInstance::insertItem("record",llist);
        foreach(const QString &str,record->m_viewItem)
        {
            QTableWidgetItem *n = new QTableWidgetItem(str);
            ui->tablewidget_recordtime->setItem(num,col++,n);
        }

    }

}



void camera_settings::on_chbox_enableAlarm_toggled(bool checked)
{
    ui->tableWidget_alarm->setEnabled(checked);
    ui->btn_addtraptime->setEnabled(checked);
    ui->btn_addtraptime->setEnabled(checked);
    ui->btn_cleartraptime->setEnabled(checked);
    ui->btn_deletetraptime->setEnabled(checked);
    ui->btn_altertraptime->setEnabled(checked);
    ui->edt_group->setEnabled(checked);
}
