#include "searchcamera.h"
#include <QMetaObject>
#include <map>
static const QStringList headerlist  = QString("序号,名称,访问地址,机身编码,子网掩码,网关,主DNS服务器").split(",");



HostView::HostView(QWidget *parent)
    :QTableWidget(parent)
{

    setShowGrid(false);
    setColumnCount(headerlist.count());
    setFixedWidth(700);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->hide();

    setHorizontalHeaderLabels(headerlist);

    const QStringList itemlist = QString("test,192.168.8.10:9999,1111,255.255.255.0,192.168.8.1,8.8.8.8").split(",");


    addNewLine(itemlist);
    addNewLine(itemlist);

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
        accept();
        break;
    case 4:
        reject();
        break;
    }


}
