#include "camera_settings.h"
#include "setting_trap_time.h"
#include "ui_camera_settings.h"
#include "lansearchcamera.h"
#include "sqldriver.h"


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


        QStringList camera = SqlInstance::GetRowList("camera_settings","camera_name",name);
        m_verifyid = camera.at(3);
        if(!camera.isEmpty())
        {
            ui->edt_user->setText(camera.at(4));
            ui->edt_passwd->setText(camera.at(5));
            QStringList l = SqlInstance::GetColumnsList("hostinfo",QStringList() << "address" << "port",
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
    SqlInstance::UpdateItem("camera_settings","camera_name",
                            ui->edt_cameraName->text(),"camera_verifyid",m_verifyid);
    SqlInstance::UpdateItem("camera_settings","camera_login_passwd",
                            ui->edt_passwd->text(),"camera_verifyid",m_verifyid);
    SqlInstance::UpdateItem("camera_settings","camera_login_name",
                            ui->edt_user->text(),"camera_verifyid",m_verifyid);
}
