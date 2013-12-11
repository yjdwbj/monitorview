#include "camera_settings.h"
#include "setting_trap_time.h"
#include "ui_camera_settings.h"
#include "lansearchcamera.h"
#include "sqldriver.h"
#include "setting_panel.h"
#include "viewframe.h"
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


        QStringList camera = SqlInstance::getRowList("camera_settings","camera_name",name);
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
    }

}

camera_settings::~camera_settings()
{
    delete ui;
}


void camera_settings::on_btn_search_clicked()
{
    LanSearchCamera *lan = new LanSearchCamera;
    lan->exec();
    delete lan;
}

void camera_settings::on_btn_add_clicked()
{
    AddTrapTimeDialog *a = new AddTrapTimeDialog;
    a->exec();
    delete a;
}

void camera_settings::on_btn_add_2_clicked()
{
    RecordTime *d = new RecordTime;
    d->exec();
    delete d;
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
        ui->tablewidget_traptime->setEnabled(checked);
        ui->btn_addtraptime->setEnabled(checked);
        ui->btn_altertraptime->setEnabled(checked);

}
