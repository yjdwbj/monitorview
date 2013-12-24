#include "searchcamera.h"
#include "ui_viewcameralist.h"
#include "sqldriver.h"
#include <QMetaObject>
#include <map>




static const QStringList headerlist  = QString("序号,名称,访问地址,机身编码,子网掩码,网关,主DNS服务器").split(",");



SearchCamera::SearchCamera(QWidget *parent)
    :QDialog(parent),
      ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    setWindowTitle("批量添加局域网内摄像机");
    emulator_readfile();
    ui->label_2->setText(QString::number(ui->tableWidget->rowCount()));

}

void SearchCamera::emulator_readfile()
{
    QString listfile(qApp->applicationDirPath()+"/camera.list");
    QFile fd(listfile);
    if(!fd.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QByteArray ba = fd.readAll();
    fd.close();
    QStringList linelist(QString(ba).split(QRegExp("(\n|\r\n)"),QString::SkipEmptyParts));
    QStringList existlist = SqlInstance::getColumnList("camera_settings","camera_verifyid");
    bool e = true;
    foreach(const QString &s,linelist)
    {
        e = true;
        foreach(const QString &str,existlist)
        {
            if(s.contains(str))
            {
                e = false;
                break;
            }

        }
        if(e)
        addNewLine(s.split("|"));
    }
}

void SearchCamera::addNewLine(QStringList list)
{
    int count = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(count);
    int num =0;
    foreach(const QString &str, list)
    {
        QTableWidgetItem *i = new QTableWidgetItem(str);
        i->setCheckState(Qt::Checked);
        if(num==0)
        {
            ui->tableWidget->setItem(count,num++,i);
        }
        else
        ui->tableWidget->setItem(count,num++,new QTableWidgetItem(str));
    }
}

QStringList SearchCamera::getVaildCameraList() const
{
    int rcout = ui->tableWidget->rowCount();
    QStringList list;
    for(int i = 0 ; i < rcout ;i++ )
    {
        if(ui->tableWidget->item(i,0)->checkState() == Qt::Unchecked)
            continue;
        QStringList t;
        for(int j = 0 ; j < ui->tableWidget->columnCount();j++)
        {
            if(ui->tableWidget->item(i,j))
            t << ui->tableWidget->item(i,j)->text();
        }

        list << t.join(",");
        t.clear();
    }

    return list;
}


void SearchCamera::slot_SwitchCheckedAll()
{
    Switch_CheckBox(true);
}

void SearchCamera::slot_SwitchCheckedNone()
{
    Switch_CheckBox(false);
}

void SearchCamera::Switch_CheckBox(bool f)
{
    int count = ui->tableWidget->rowCount();
    for(int i = 0 ; i  < count;i++)
    {
    ui->tableWidget->item(i,0)->setCheckState(f ? Qt::Checked : Qt::Unchecked);
    }
}





void SearchCamera::on_pushButton_4_clicked()
{

}
