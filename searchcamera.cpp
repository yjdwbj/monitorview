#include "searchcamera.h"
#include <QMetaObject>
#include <map>
static const QStringList headerlist  = QString("序号,名称,访问地址,机身编码,子网掩码,网关,主DNS服务器").split(",");



HostView::HostView(QWidget *parent)
    :QTableWidget(parent)
{

    setContentsMargins(1,1,1,1);
    setShowGrid(false);
    setColumnCount(headerlist.count());
    setFixedWidth(700);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->hide();

    setHorizontalHeaderLabels(headerlist);

    const QStringList camera1 = QString("camera1,localhost:8090,1111,255.255.255.0,192.168.115.1,8.8.8.8").split(",");
    const QStringList camera2 = QString("camera2,localhost:8090,1111,255.255.255.0,192.168.115.1,8.8.8.8").split(",");
    const QStringList camera3 = QString("camera3,localhost:8090,1111,255.255.255.0,192.168.115.1,8.8.8.8").split(",");
    const QStringList camera4 = QString("camera4,localhost:8090,1111,255.255.255.0,192.168.115.1,8.8.8.8").split(",");
    addNewLine(camera1);
    addNewLine(camera2);
    addNewLine(camera3);
    addNewLine(camera4);

    show();
}

void HostView::addNewLine(const QStringList &list)
{

    int count = this->rowCount();
    insertRow(count);
    int num =0;
    QCheckBox *box= new QCheckBox(QString::number(count+1));
    box->setChecked(true);
    setCellWidget(count,num++,box);
    foreach(const QString &str, list)
    {
        setItem(count,num++,new QTableWidgetItem(str));
    }
}


QStringList HostView::getViewCountList() const
{
    int rcout = this->rowCount();
    QStringList list;
    for(int i = 0 ; i < rcout ;i++ )
    {
        QCheckBox *box = qobject_cast<QCheckBox *>(cellWidget(i,0));
        if(!box->isChecked())
            continue;
        QStringList t;
        for(int j = 1 ; j < headerlist.count();j++)
        {
            t << item(i,j)->text();
        }

        list << t.join(",");
        t.clear();
    }

    return list;
}

SearchCamera::SearchCamera(QDialog *parent)
    :QDialog(parent),
      m_viewlist(new HostView),
      signalmap(new QSignalMapper(this))
{
    QVBoxLayout *main_layout =  new QVBoxLayout;
    setWindowTitle("批量添加局域网内摄像机");
    const QStringList btnlist = QString("全选,全不选,刷新,提交,取消").split(",");
//    const QStringList slotslist =QString("slot_SwitchCheckedAll,slot_SwitchCheckedNone,slot_Reload,accept,reject").split(",");

    QHBoxLayout *lay_btn = new QHBoxLayout;
    for(int i = 0; i < btnlist.size();i++)
    {
        QPushButton *btn = new QPushButton(btnlist.at(i));
        signalmap->setMapping(btn,i);
        connect(btn,SIGNAL(clicked()),signalmap,SLOT(map()));
//        btn->setObjectName(str);
        lay_btn->addWidget(btn);
    }
    connect(signalmap,SIGNAL(mapped(int)),this,SLOT(slot_MapSignal(int)));
    main_layout->addWidget(m_viewlist);
    main_layout->addLayout(lay_btn);
    setLayout(main_layout);
    show();


}
void SearchCamera::slot_Reload()
{

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
    int count = m_viewlist->rowCount();
    for(int i = 0 ; i  < count;i++)
    {
        QCheckBox *box = qobject_cast<QCheckBox *>(m_viewlist->cellWidget(i,0));
        box->setChecked(f);
    }
}


void SearchCamera::slot_MapSignal(int id)
{
    switch(id)
    {
      case 0:
        slot_SwitchCheckedAll();
        break;
    case 1:
        slot_SwitchCheckedNone();
        break;
    case 2:
        slot_Reload();
        break;
    case 3:
        vaidlist = m_viewlist->getViewCountList();
        accept();
        break;
    case 4:
        reject();
        break;
    }


}

