#include "camera_settings.h"
#include "setting_trap_time.h"
#include "ui_camera_settings.h"
#include "lansearchcamera.h"
#include "sqldriver.h"
#include "setting_panel.h"
#include "viewframe.h"
#include "alarmtrapsettings.h"
class ViewFrame;
class CameraView;


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
}

camera_settings::~camera_settings()
{
    delete ui;
}


void camera_settings::setRecordTab()
{

   QStringList alarmlist =  SqlInstance::getRowList("alarm","verifyid",m_verifyid);
   if(alarmlist.count() <1)
       return;
   foreach(const QString &line,alarmlist)
   {
       int row = ui->tableWidget_alarm->rowCount();
       ui->tableWidget_alarm->insertRow(row);

       QStringList record = line.split(',');

       QString wek;
       unsigned char c;
       memcpy(&c,record.at(1).toLocal8Bit().data(),1);
       wek =  c == 254 ?  QString("每天"): getStringFromChar(c,weekNum.split(','));

       ui->tableWidget_alarm->setItem(row,0,new QTableWidgetItem(wek));

       ui->tableWidget_alarm->setItem(row,1,new QTableWidgetItem(record.at(2)));
       ui->tableWidget_alarm->setItem(row,2,new QTableWidgetItem(record.at(3)));
       memcpy(&c,record.at(5).toLocal8Bit().data(),1);
       ui->tableWidget_alarm->setItem(row,3,
                                      new QTableWidgetItem(getStringFromChar(c,alarmCondition.split(','))));




   }

}

QString camera_settings::getStringFromChar(unsigned char c, const QStringList &list)
{
    QStringList t;
    int n = list.count();
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




void camera_settings::on_cbox_enablerecord_toggled(bool checked)
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



void camera_settings::on_btn_addtraptime_clicked()
{

//    AddTrapTimeDialog *a = new AddTrapTimeDialog;
//    a->exec();
//    delete a;
    AlarmTrapSettings *al = new AlarmTrapSettings(m_verifyid);
    if(al->exec())
    {
        int num = ui->tableWidget_alarm->rowCount();
        ui->tableWidget_alarm->insertRow(num);
        int col = 0;
        foreach(const QString &str,al->getAlarmList())
        {
            QTableWidgetItem *n = new QTableWidgetItem(str);
            ui->tableWidget_alarm->setItem(num,col++,n);
        }
    }

}

void camera_settings::on_btn_addrecordtime_clicked()
{
    RecordTime *d = new RecordTime;
    d->exec();
    delete d;

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
