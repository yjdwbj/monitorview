#include "camera_settings.h"
#include "setting_trap_time.h"
#include "ui_camera_settings.h"
#include "lansearchcamera.h"


camera_settings::camera_settings(const QString &name,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::camera_settings)
{
    ui->setupUi(this);
    this->setWindowTitle("摄像机设置");
    ui->edt_cameraName->setText(name);


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
