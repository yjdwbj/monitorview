#include "searchcamera.h"
#include "ui_viewcameralist.h"
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

    foreach(const QString &s,linelist)
    {
        addNewLine(s.split("|"));
    }
}

void SearchCamera::addNewLine(QStringList list)
{
    int count = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(count);
    int num =0;
    QCheckBox *box= new QCheckBox(QString::number(count+1));
    box->setChecked(true);
    ui->tableWidget->setCellWidget(count,num++,box);
    foreach(const QString &str, list)
    {
        ui->tableWidget->setItem(count,num++,new QTableWidgetItem(str));
    }
}

QStringList SearchCamera::getVaildCameraList() const
{
    int rcout = ui->tableWidget->rowCount();
    QStringList list;
    for(int i = 0 ; i < rcout ;i++ )
    {
        QCheckBox *box = qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,0));
        if(!box->isChecked())
            continue;
        QStringList t;
        for(int j = 1 ; j < ui->tableWidget->columnCount();j++)
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
        QCheckBox *box = qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,0));
        box->setChecked(f);
    }
}




