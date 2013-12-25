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





RecordTime::RecordTime(const QString &verifyid, const QString &init,
                      QWidget *parent)
    :QDialog(parent),
    ui(new Ui::record_time)
{
    m_verifyid = verifyid;
    ui->setupUi(this);
    for(int i = 1 ; i < 8;i++)
        weeklist << ui->gbox_week->findChild<QCheckBox*>("cbox_w"+QString::number(i));


    if(!init.isEmpty())
    {
        QStringList tlist = init.split('?',QString::SkipEmptyParts);
        if(tlist.isEmpty())
            return;

            setCharToList(tlist.first().toInt(),weeklist);
            QStringList l = tlist.last().split('~',QString::SkipEmptyParts);
            ui->tedt_starttime->setTime(QTime::fromString(l.first()));
            ui->tedt_endtime->setTime(QTime::fromString(l.last()));

    }

//    QStringList list = SqlInstance::QuerySqlFromString(
//                "select * from record where record_verifyid == ? and record_sort_id == ?",
//                QStringList() << m_verifyid << id);
//    setCharToList(list.at(2).toInt(),weeklist);
//    ui->tedt_starttime->setTime(QTime::fromString(list.at(3)));
//    ui->tedt_endtime->setTime(QTime::fromString(list.at(4)));

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


    m_iniConf << QString::number(w)
              << QString(ui->tedt_starttime->text() + "~" + ui->tedt_endtime->text());
    m_viewItem << wek << QString(ui->tedt_starttime->text() + " - " + ui->tedt_endtime->text());
    m_sqlItem << m_verifyid << QString::number(w) <<  ui->tedt_starttime->text() << ui->tedt_endtime->text() ;
}




camera_settings::camera_settings(int index, const QString &name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::camera_settings),
    m_inifname(qApp->applicationDirPath()+"/devs/"+name)
{
    ui->setupUi(this);
    this->setWindowTitle("摄像机设置");
    ui->edt_cameraName->setText(name);
    ui->tab_Widget->setCurrentIndex(index);
    if(!name.isEmpty())
    {


          set = new QSettings(m_inifname,QSettings::IniFormat);

        QStringList camera = SqlInstance::getRowFirst("camera_settings","camera_name",name);
        m_verifyid = camera.at(3);
        if(!camera.isEmpty())
        {
            ui->edt_user->setText(camera.at(4));
            ui->edt_passwd->setText(camera.at(5));
            QStringList l = SqlInstance::getColumnsList("hostinfo",QStringList() << "addrport",
                              "host_id",camera.at(2));
            if(!l.isEmpty())
            {
                ui->edt_ipaddress->setText("http://" + l.first().replace(",",":"));
            }
            ui->edt_info_user->setText(camera.at(6));
            ui->edt_info_tel->setText(camera.at(7));
            ui->edt_info_addr->setText(camera.at(8));
            ui->edt_info_commit->setText(camera.at(9));

        }
//         ui->chbox_enableAlarm->setChecked(!camera.at(12).compare("0") ? false:true );
//         ui->chbox_enablerecord->setChecked(!camera.at(13).compare("0") ? false:true );
        ui->chbox_enableAlarm->setChecked(set->value("Global/AlarmEnable").toBool());
        int type = set->value("RecVideo/RecType").toInt();
        ui->chbox_enablerecord->setChecked( type > 0 ? true : false);
        if(type > 1)
            ui->rdb_specialtime->setChecked(true);
    }
    setRecordTab();
    setAlarmTab();

//    ui->edt_info_user->setText(set->value("Info/User").toString());
//    ui->edt_info_tel->setText(set->value("Info/Tel").toString());
//    ui->edt_info_addr->setText(set->value("Info/Addr").toString());
//    ui->edt_info_commit->setText(set->value("Info/Commit").toString());
}

camera_settings::~camera_settings()
{
    delete ui;
}

void camera_settings::setRecordTab()
{
//     QStringList recordlist =  SqlInstance::getRowList("record","record_verifyid",m_verifyid);
//     if(recordlist.size() < 1)
//         return;
//     bool flag = false;
//     foreach( const QString &line,recordlist)
//     {
//         int row = ui->tablewidget_recordtime->rowCount();
//         ui->tablewidget_recordtime->insertRow(row);
//         QStringList record = line.split(',');

//         QString wek;

//         unsigned char c = record.at(2).toInt();
//         wek =  c == 254 ?  QString("每天"): getStringFromChar(c,weekNum.split(','));
//         ui->tablewidget_recordtime->setItem(row,0,new QTableWidgetItem(wek));
//         ui->tablewidget_recordtime->setItem(row,1,
//                                             new QTableWidgetItem(record.at(4) + " - " + record.at(5)));
//         if(!record.at(6).compare("1"))
//             flag = true;
//     }
//         ui->rdb_specialtime->setChecked(flag);

    if(set->value(RecordType).toInt() > 0)
        ui->chbox_enablerecord->setChecked(true);

    QString RecordSec = set->value(RecordTimeSec).toString();
    if(RecordSec.isEmpty())
        return;
    QStringList tlist = RecordSec.split('|',QString::SkipEmptyParts);
    foreach(const QString &s, tlist)
    {
        QString w;
        int c = s.section('?',0,0).toInt();
        w =  c == 254 ?  QString("每天"): getStringFromChar(c,weekNum.split(','));
//        QString t = s.section('?',1,1);

        int row = ui->tablewidget_recordtime->rowCount();
        ui->tablewidget_recordtime->insertRow(row);
        ui->tablewidget_recordtime->setItem(row,0,new QTableWidgetItem(w));
        ui->tablewidget_recordtime->setItem(row,1,new QTableWidgetItem(s.section('?',1,1)));


    }

}


void camera_settings::setAlarmTab()
{

//   QStringList alarmlist =  SqlInstance::getRowList("alarm","verifyid",m_verifyid);
//   if(alarmlist.size() <1)
//       return;
//   foreach(const QString &line,alarmlist)
//   {
//       int row = ui->tableWidget_alarm->rowCount();
//       ui->tableWidget_alarm->insertRow(row);

//       QStringList record = line.split(',');

//       QString wek;

//       unsigned char c = record.at(2).toInt();
//       wek =  c == 254 ?  QString("每天"): getStringFromChar(c,weekNum.split(','));

//       ui->tableWidget_alarm->setItem(row,0,new QTableWidgetItem(wek));

//       ui->tableWidget_alarm->setItem(row,1,new QTableWidgetItem(record.at(3)));
//       ui->tableWidget_alarm->setItem(row,2,new QTableWidgetItem(record.at(4)));

//        c = record.at(6).toInt();
//       ui->tableWidget_alarm->setItem(row,3,
//                                      new QTableWidgetItem(getStringFromChar(c,alarmCondition.split(','))));

//   }
    QString iniValue = set->value(AlarmTimeSec).toString();
    ui->chbox_enableAlarm->setChecked(set->value(EnableAlarm).toBool());

    QStringList inilist = iniValue.split('|',QString::SkipEmptyParts);
    if(inilist.isEmpty())
        return;
    foreach(const QString &str,inilist)
    {
        QStringList tmlist = str.split('?',QString::SkipEmptyParts);
        int row = ui->tableWidget_alarm->rowCount();
        ui->tableWidget_alarm->insertRow(row);

        QString wek;

        unsigned char c = tmlist.at(0).toInt();
        wek =  c == 254 ?  QString("每天"): getStringFromChar(c,weekNum.split(','));
        ui->tableWidget_alarm->setItem(row,0,new QTableWidgetItem(wek));
        ui->tableWidget_alarm->setItem(row,1,
                                       new QTableWidgetItem(tmlist.at(1).section('~',0,0)));
        ui->tableWidget_alarm->setItem(row,2,
                                       new QTableWidgetItem(tmlist.at(1).section('~',1,1)));
        c = tmlist.at(2).toInt();
        ui->tableWidget_alarm->setItem(row,3,
        new QTableWidgetItem(getStringFromChar(c,alarmCondition.split(','))));

    }

    ui->edt_group->setText(set->value(AlarmGroup).toString());




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

    SqlInstance::updateItem("camera_settings","camera_loginpwd",
                            ui->edt_passwd->text(),"camera_verifyid",m_verifyid);

    SqlInstance::updateItem("camera_settings","camera_loginacct",
                            ui->edt_user->text(),"camera_verifyid",m_verifyid);  

    SqlInstance::updateItem("camera_settings","camera_addr",
                            ui->edt_info_addr->text(),"camera_verifyid",m_verifyid);

    SqlInstance::updateItem("camera_settings","camera_user",
                            ui->edt_info_user->text(),"camera_verifyid",m_verifyid);

    SqlInstance::updateItem("camera_settings","camera_commit",
                            ui->edt_info_commit->text(),"camera_verifyid",m_verifyid);

    SqlInstance::updateItem("camera_settings","camera_tel",
                            ui->edt_info_tel->text(),"camera_verifyid",m_verifyid);

    set->setValue("Global/EnableAlarm",ui->chbox_enableAlarm->isChecked());
    set->setValue(AlarmGroup,ui->edt_group->text());


    int type = 0;
    if(ui->chbox_enablerecord->isChecked())
        type = 1;
    if(ui->rdb_specialtime->isChecked() && ui->chbox_enablerecord->isChecked() )
        type = 2;

//    set->setValue("Global/RecordTimeSec",ui->chbox_enablerecord->isChecked()? 1 : 0);
//    set->setValue("Info/User",ui->edt_info_user->text());
//    set->setValue("Info/Tel",ui->edt_info_tel->text());
//    set->setValue("Info/Addr",ui->edt_info_addr->text());
//    set->setValue("Info/Commit",ui->edt_info_commit->text());

    set->setValue("RecVideo/RecType",type);
    set->sync();




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
    on_rdb_specialtime_toggled(checked);
}




void camera_settings::on_rdb_specialtime_toggled(bool checked)
{
     ui->tablewidget_recordtime->setEnabled(ui->rdb_specialtime->isChecked() & checked);
     ui->btn_addrecordtime->setEnabled(ui->rdb_specialtime->isChecked() & checked);
     ui->btn_alterrecordtime->setEnabled(ui->rdb_specialtime->isChecked() & checked);
     ui->btn_clearrecordtime->setEnabled(ui->rdb_specialtime->isChecked() & checked);
     ui->btn_deletetrecordtime->setEnabled(ui->rdb_specialtime->isChecked() & checked);
}



void camera_settings::on_btn_addtraptime_clicked() // add new item
{


    AlarmTrapSettings *al = new AlarmTrapSettings(m_inifname,m_verifyid,
                                                  QString::number(ui->tableWidget_alarm->currentRow()));
    if(!al->exec()) // here is reverse because AlarmTrapSettings overwrite accept() */
    {
        int num = ui->tableWidget_alarm->rowCount();
        ui->tableWidget_alarm->insertRow(num);
        int col = 0;
//        QStringList llist = al->getAlarmSql();
//        QString id = QString::number(SqlInstance::getMaximumId("alarm","id")+1);
//        llist.prepend(id);
//        llist.append(QString::number(num));
//        SqlInstance::insertItem("alarm",llist);
         QString v = set->value(AlarmTimeSec).toString();
         QStringList vlist = v.split('|',QString::SkipEmptyParts);
         vlist.append(al->getIniItem().join('?'));
         set->setValue(AlarmTimeSec,vlist);
         set->sync();
        foreach(const QString &str,al->getAlarmList())
        {
            QTableWidgetItem *n = new QTableWidgetItem(str);
            ui->tableWidget_alarm->setItem(num,col++,n);
        }


    }

}

void camera_settings::on_btn_altertraptime_clicked() // alter item
{
     QString oldAlarm = set->value(AlarmTimeSec).toString();
     QStringList tlist = oldAlarm.split('|',QString::SkipEmptyParts);
     if(tlist.isEmpty())
         return;
     while(tlist.size() < ui->tableWidget_alarm->rowCount())
         tlist.append("");

    AlarmTrapSettings *al = new AlarmTrapSettings(m_inifname,m_verifyid,
                                                  QString::number(ui->tableWidget_alarm->currentRow()),
                                                  tlist.at(ui->tableWidget_alarm->currentRow()));
    if(!al->exec())
    {
        int num = ui->tableWidget_alarm->currentRow();
//        QString nstr = QString::number(num);
//        QStringList llist =  al->getAlarmSql();


//        QStringList idl = SqlInstance::QuerySqlFromString("select id from alarm where verifyid == ? and sort_id == ?",
//                                                          QStringList () << m_verifyid << nstr);
//        llist.prepend(idl.first());
//        llist.append(nstr);

//        SqlInstance::deleteRecordByCondition("alarm","id",idl.first());
//        SqlInstance::insertItem("alarm",llist);


        tlist.replace(num,al->getIniItem().join('?'));
        set->setValue(AlarmTimeSec,tlist.join('|'));
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

    QString val =set->value(AlarmTimeSec).toString();
    QStringList tlist = val.split('|',QString::SkipEmptyParts);
    if(tlist.isEmpty()|| tlist.size() < row)
    return;
    tlist.removeAt(row);
    set->setValue(AlarmTimeSec,tlist.join('|'));
    set->sync();

//    QStringList idl = SqlInstance::QuerySqlFromString("select id from alarm where verifyid == ? and sort_id == ?",
//                                                      QStringList () << m_verifyid << QString::number(row));
//    SqlInstance::deleteRecordByCondition("alarm","id",idl.first());
    ui->tableWidget_alarm->removeRow(row);
}



void camera_settings::on_btn_cleartraptime_clicked()   // clear all item
{
    ui->tableWidget_alarm->setCurrentItem(ui->tableWidget_alarm->item(0,0));
    int row ;
    while( (row = ui->tableWidget_alarm->currentRow()) >= 0)
    {
//        QStringList idl = SqlInstance::QuerySqlFromString("select id from alarm where verifyid == ? and sort_id == ?",
//                                                          QStringList () << m_verifyid << QString::number(row));
//        SqlInstance::deleteRecordByCondition("alarm","id",idl.first());
        ui->tableWidget_alarm->removeRow(row);
    }
    set->setValue(AlarmTimeSec,QString());
    set->sync();
}

void camera_settings::on_btn_addrecordtime_clicked() // add record time
{


    RecordTime *record = new RecordTime(m_verifyid,
                                       getRecordIniConfig());
    if(record->exec())
    {
        int num = ui->tablewidget_recordtime->rowCount();
        ui->tablewidget_recordtime->insertRow(num);
        int col = 0;
        QString v = set->value(RecordTimeSec).toString();
        QStringList vlist = v.split('|',QString::SkipEmptyParts);
        vlist.append(record->m_iniConf.join('?'));
        set->setValue(RecordTimeSec,vlist.join('|'));
        set->sync();
//        QStringList llist = record->m_sqlItem;
//        QString id = QString::number(SqlInstance::getMaximumId("record","record_id")+1);
//        llist.prepend(id);
//        llist.append(QString::number(num));
//        llist.append("1");
//        SqlInstance::insertItem("record",llist);
        foreach(const QString &str,record->m_viewItem)
        {
            QTableWidgetItem *n = new QTableWidgetItem(str);
            ui->tablewidget_recordtime->setItem(num,col++,n);
        }

    }

}

QString camera_settings::getRecordIniConfig()
{
    /* get one row from  ini config to  ui->tablewidget_recordtime.*/
    QString init = set->value(RecordTimeSec).toString();
    QString v=QString();
    if(!init.isEmpty())
    {
        QStringList l = init.split('|',QString::SkipEmptyParts);
        if(!l.isEmpty())
        {
            int n = ui->tablewidget_recordtime->currentRow();
            if(n < 0)
                n = 0;
            v = l.at(n);
        }
    }
    return v;
}


void camera_settings::on_btn_alterrecordtime_clicked() // alter record time
{



    RecordTime *record = new RecordTime(m_verifyid,
                                       getRecordIniConfig());
    if(record->exec())
    {
        int num = ui->tablewidget_recordtime->currentRow();
        QString nstr = QString::number(num);
//        QStringList llist =  record->m_sqlItem;
//        QStringList idl = SqlInstance::QuerySqlFromString("select record_id from record where record_verifyid == ? and record_sort_id == ?",
//                                                          QStringList () << m_verifyid << nstr);
//        llist.prepend(idl.first());
//        llist.append(nstr);
//        llist.append("1");

//        SqlInstance::deleteRecordByCondition("record","record_id",idl.first());
//        SqlInstance::insertItem("record",llist);
        QStringList vlist = set->value(RecordTimeSec).toString().split('|',QString::SkipEmptyParts);
        if(vlist.isEmpty())
            return;
        vlist.replace(ui->tablewidget_recordtime->currentRow(),record->m_iniConf.join('?'));
        set->setValue(RecordTimeSec,vlist.join('|'));
        set->sync();
        int col = 0;
        foreach(const QString &str,record->m_viewItem)
        {
            QTableWidgetItem *n = new QTableWidgetItem(str);
            ui->tablewidget_recordtime->setItem(num,col++,n);
        }
    }
}

void camera_settings::on_btn_deletetrecordtime_clicked() // delete record time
{
    int row = ui->tablewidget_recordtime->currentRow();

    QStringList vlist = set->value(RecordTimeSec).toString().split('|');
    if(vlist.isEmpty())
        return;
    vlist.removeAt(row);
    set->setValue(RecordTimeSec,vlist.join('|'));
    set->sync();
//    QStringList idl = SqlInstance::QuerySqlFromString("select record_id from record where record_verifyid == ? and record_sort_id == ?",
//                                                      QStringList () << m_verifyid << QString::number(row));

//    SqlInstance::deleteRecordByCondition("record","record_id",idl.first());
    ui->tablewidget_recordtime->removeRow(row);
}

void camera_settings::on_btn_clearrecordtime_clicked()    // clear record time
{
    ui->tablewidget_recordtime->setCurrentItem(ui->tablewidget_recordtime->item(0,0));
    int row ;
    while( (row = ui->tablewidget_recordtime->currentRow()) >= 0)
    {
//        QStringList idl = SqlInstance::QuerySqlFromString("select record_id from record where record_verifyid == ? and record_sort_id == ?",
//                                                          QStringList () << m_verifyid << QString::number(row));
//        SqlInstance::deleteRecordByCondition("record","record_id",idl.first());
        ui->tablewidget_recordtime->removeRow(row);
    }
    set->setValue(RecordTimeSec,QVariant());
    set->sync();
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



void camera_settings::on_tableWidget_alarm_doubleClicked(const QModelIndex &index)
{
    on_btn_altertraptime_clicked();
}

void camera_settings::on_tablewidget_recordtime_doubleClicked(const QModelIndex &index)
{
    on_btn_alterrecordtime_clicked();
}
