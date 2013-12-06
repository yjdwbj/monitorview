#include "lansearchcamera.h"
#include "ui_lansearchcamera.h"

LanSearchCamera::LanSearchCamera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LanSearchCamera)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
}

LanSearchCamera::~LanSearchCamera()
{
    delete ui;
}

void LanSearchCamera::on_pushButton_2_clicked()
{
    accept();
}
