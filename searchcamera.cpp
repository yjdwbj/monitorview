#include "searchcamera.h"
#include <QMetaObject>
#include <map>



static const QStringList headerlist  = QString("序号,名称,访问地址,机身编码,子网掩码,网关,主DNS服务器").split(",");



SearchCamera::SearchCamera(QDialog *parent)
    :QDialog(parent),
      m_viewlist(new ListView(headerlist)),
      signalmap(new QSignalMapper(this))
{
    const QStringList camera1 = QString("camera1,192.168.8.31,1111,255.255.255.0,192.168.115.1,8.8.8.8").split(",");
    const QStringList camera2 = QString("camera2,192.168.8.31,1111,255.255.255.0,192.168.115.1,8.8.8.8").split(",");
    const QStringList camera3 = QString("camera3,192.168.8.31,1111,255.255.255.0,192.168.115.1,8.8.8.8").split(",");
    const QStringList camera4 = QString("camera4,192.168.8.31,1111,255.255.255.0,192.168.115.1,8.8.8.8").split(",");
    m_viewlist->addNewLine(camera1);
    m_viewlist->addNewLine(camera2);
    m_viewlist->addNewLine(camera3);
    m_viewlist->addNewLine(camera4);
    m_viewlist->setFixedWidth(700);


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

