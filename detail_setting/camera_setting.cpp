#include "camera_setting.h"

CameraSetting::CameraSetting(const QString &name,QWidget *parent)
    :QDialog(parent),
      mainTab(new QTabWidget)
{

    QGridLayout *main_layout = new QGridLayout();
    LabAndLineEdit *cameraName = new LabAndLineEdit(tr("名称:"),"",name);
    mainTab->addTab(ConnectInformation("http://192.168.8.31"),"连接信息");
    mainTab->addTab(OtherInformation(),"其它信息");
    mainTab->addTab(Alarm(),"报警");

    main_layout->addWidget(cameraName,0,0,1,2);
    main_layout->addWidget(mainTab,1,0,1,2);
    QPushButton *btn_Ok = new QPushButton("提交");
    connect(btn_Ok,SIGNAL(clicked()),SLOT(accept()));
    QPushButton *btn_Cancul = new QPushButton("取消");
    connect(btn_Cancul,SIGNAL(clicked()),SLOT(reject()));
    main_layout->addWidget(btn_Ok,2,0);
    main_layout->addWidget(btn_Cancul,2,1);

    this->setLayout(main_layout);
}


QWidget *CameraSetting::ConnectInformation(const QString &url)
{

    QGridLayout *grid_layout = new QGridLayout;
    LabAndLineEdit *accessAddr =
            new LabAndLineEdit(tr("访问地址:"),
            "比如地址：端口http://192.168.1.1:8080,域名http://www.example.com",url);
    QPushButton *btn_search = new QPushButton("查找");
    connect(btn_search,SIGNAL(clicked()),SLOT(slot_SearchCameraFromLan()));
    LabAndLineEdit *watchID = new LabAndLineEdit("观看帐号:","","admin");
    LabAndLineEdit *watchPasswd = new LabAndLineEdit("观看密码:","","*****");
    grid_layout->addWidget(accessAddr,0,0);
    grid_layout->addWidget(btn_search,0,1);
    grid_layout->addWidget(watchID,1,0,1,2);
    grid_layout->addWidget(watchPasswd,2,0,1,2);
    QWidget *w = new QWidget;
    w->setLayout(grid_layout);
    return w;
}

QWidget *CameraSetting::OtherInformation()
{
    QVBoxLayout *main_layout = new QVBoxLayout;

    QCheckBox *supportYunTai = new QCheckBox("支持云台");
    QCheckBox *reflectionImage = new QCheckBox("倒置图像");
    main_layout->addWidget(supportYunTai);
    main_layout->addWidget(reflectionImage);
    main_layout->addStretch();
    QWidget *w = new QWidget;
    w->setLayout(main_layout);
    return w;
}

QWidget *CameraSetting::Alarm()
{
    QVBoxLayout *main_layout = new QVBoxLayout;

    QCheckBox *enable_alarm = new QCheckBox("启用报警");

    LabAndLineEdit *alarm_group = new LabAndLineEdit("报警组:");
    QGroupBox *gbox_trapTime = new QGroupBox("布防时间段");
    QGridLayout *lay_trapTime = new QGridLayout(gbox_trapTime);
    QString headLabels("星期,时间,触发方式,报警动作");

    ListView *lview = new ListView(headLabels.split(","));
    lay_trapTime->addWidget(lview,0,0,1,4);
    QString btn_list("添加,修改,删除,清除");
    QSignalMapper *signalmap = new QSignalMapper;
    int i = 0;
    foreach(const QString &str,btn_list.split(","))
    {
        QPushButton *btn = new QPushButton(str);
        lay_trapTime->addWidget(btn,1,i);
        signalmap->setMapping(btn,i++);
         connect(btn,SIGNAL(clicked()),signalmap,SLOT(map()));
    }
     connect(signalmap,SIGNAL(mapped(int)),this,SLOT(slot_MapSignal(int)));

     main_layout->addWidget(enable_alarm);
     main_layout->addWidget(gbox_trapTime);
     main_layout->addStretch();
     QWidget *w = new QWidget ;
     w->setLayout(main_layout);
     return w;

}


void CameraSetting::slot_MapSignal(int id)
{

}

void CameraSetting::slot_SearchCameraFromLan()
{

}
