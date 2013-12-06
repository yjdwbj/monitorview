#include "camera_setting.h"
#include "setting_trap_time.h"
#include "lansearchcamera.h"



CameraSetting::CameraSetting(const QString &name,QWidget *parent)
    :QDialog(parent),
      mainTab(new QTabWidget)
{

    setWindowModality(Qt::ApplicationModal);

    setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QPushButton {height: 20px;width:50px; background: gray;border-radius: 2px;"
                  "border-style: inset;"
                  "border-width: 1px;"
                  "}"
                  "QPushButton::hover{ height: 20px;width: 50px; background: white;"
                  "border-radius: 2px;border-style: inset;"
                  "border-width: 1px;"
                  "}");
//    background-image: url(:/lcy/images/large_button.png);
//    background: #d4d4d4;
//    border-radius: 10px;
//    background-origin: margin;
//    border-width: 2px;
//    background-position: buttom center;
//    setStyleSheet("QPushButton {"
//                  "background-image: url(:/lcy/images/large_button.png);"
//                  "background-position: top center;}"
//                  "background: #d4d4d4;"
//                  "background-origin: margin;"
//                  "border-radius: 2px;"
//                  "border-width: 0px;"
//                  "border-style: outset;"
//                  "QPushButton::hover {"
//                  "background-image: url(:/lcy/images/large_button.png);"
//                  "background-position: buttom center;"
//                  "background: #d4d4d4;"
//                  "border-radius: 2px;"
//                  "border-width: 0px;"
//                  "border-style: outset;}");

    QVBoxLayout *main_layout = new QVBoxLayout();
    LabAndWidget *cameraName = new LabAndWidget(tr("名称:"),new QLineEdit(name));
    mainTab->addTab(ConnectInformation("http://192.168.8.31"),"连接信息");
    mainTab->addTab(OtherInformation(),"其它信息");
    mainTab->addTab(Alarm(),"报警");
    mainTab->addTab(RecordVideo(),"录像");
    mainTab->addTab(AppendInformation(),"附加信息");

    main_layout->addWidget(cameraName);
    main_layout->addWidget(mainTab);
    GroupBtnWidget *btn_dialog = new GroupBtnWidget(yesorno.split(","));
    connect(btn_dialog,SIGNAL(SignalById(int)),SLOT(slot_Btn_Dialog(int)));
    main_layout->addWidget(btn_dialog);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setFixedWidth(440);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);

    this->setLayout(main_layout);
}

void CameraSetting::slot_Btn_Dialog(int id)
{
//    id ? reject() : accept();
}


QWidget *CameraSetting::ConnectInformation(const QString &url)
{

    QHBoxLayout *lay_addr = new QHBoxLayout;
    LabAndWidget *accessAddr =
            new LabAndWidget(tr("访问地址:"),new QLineEdit(url),
            LabAndWidget::Horizontal,"比如地址：端口http://192.168.1.1:8080,域名http://www.example.com");
    QLineEdit *edit = (QLineEdit*)accessAddr->getTwoObj();
    edit->setFixedWidth(240);
    QLabel *lab_commit= new QLabel("    比如地址：端口http://192.168.1.1:8080,默认端口80\n    域名http://www.example.com");
    QPushButton *btn_search = new QPushButton("查找");
    connect(btn_search,SIGNAL(clicked()),SLOT(slot_SearchCameraFromLan()));
    LabAndWidget *watchID = new LabAndWidget("观看帐号:",new QLineEdit("admin"));
    LabAndWidget *watchPasswd = new LabAndWidget("观看密码:",new QLineEdit("******"));

    lay_addr->setSpacing(1);
    lay_addr->addWidget(accessAddr);
    lay_addr->addWidget(btn_search);

    QVBoxLayout *lay_main = new QVBoxLayout;
    lay_main->addLayout(lay_addr);
    lay_main->addWidget(lab_commit);
    lay_main->addWidget(watchID);
    lay_main->addWidget(watchPasswd);
    lay_main->addStretch();

    QWidget *w = new QWidget;
    w->setLayout(lay_main);
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

    LabAndWidget *alarm_group = new LabAndWidget("报警组:",new QLineEdit);
    QGroupBox *gbox_trapTime = new QGroupBox("布防时间段");
    QVBoxLayout *lay_trapTime = new QVBoxLayout(gbox_trapTime);
    QString headLabels("星期,时间,触发方式,报警动作");

    ListView *lview = new ListView(headLabels.split(","));
    lay_trapTime->addWidget(lview);
    QString btn_list("添加,修改,删除,清除");
    GroupBtnWidget *gbtn = new GroupBtnWidget(btn_list.split(","));
    connect(gbtn,SIGNAL(SignalById(int)),SLOT(slot_Alarm_Widget(int)));
    lay_trapTime->addWidget(gbtn);
     main_layout->addWidget(enable_alarm);
     main_layout->addWidget(alarm_group);
     main_layout->addWidget(gbox_trapTime);
     main_layout->addStretch();
     QWidget *w = new QWidget ;
     w->setLayout(main_layout);
     return w;

}

void CameraSetting::slot_Alarm_Widget(int id)
{
    switch(id)
    {
    case 0:
        AddTrapTimeDialog *dlg = new AddTrapTimeDialog;
        dlg->exec();
        break;
    }
}

QWidget* CameraSetting::RecordVideo()
{
    QVBoxLayout *main_layout = new QVBoxLayout;
    QCheckBox *enable_record = new QCheckBox("启用录像");
    QRadioButton *allTime_record = new QRadioButton("所有时间进行录制");
    QRadioButton *blowTime_record = new QRadioButton("只在下列时间段进行录制");

    main_layout->addWidget(enable_record);
    main_layout->addWidget(allTime_record);
    main_layout->addWidget(blowTime_record);

    ListView *time_View = new ListView(QString("星期,时间").split(","));
    main_layout->addWidget(time_View);
    QString btn_list("添加,修改,删除,清除");


    GroupBtnWidget *gbtn = new GroupBtnWidget(btn_list.split(","));
    connect(gbtn,SIGNAL(SignalById(int)),SLOT(slot_RecordVideo_signals(int)));
     main_layout->addWidget(gbtn);


     QWidget *w = new QWidget ;
     w->setLayout(main_layout);
     return w;
}

void CameraSetting::slot_RecordVideo_signals(int id)
{
    switch(id)
    {
    case 0:
        RecordTime *dlg = new RecordTime;
        dlg->exec();
        break;
    }
}


QWidget* CameraSetting::AppendInformation()
{
    QString("姓名,电话,地址,备注");
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->setSpacing(2);
    foreach(const QString &str ,QString("姓名,电话,地址,备注").split(",") )
    {
        LabAndWidget *t = new LabAndWidget(str+":",new QLineEdit);
        main_layout->addWidget(t,Qt::AlignCenter);
    }
    main_layout->addStretch(400);
    QWidget *w = new QWidget;
    w->setLayout(main_layout);
    return w;
}

void CameraSetting::slot_MapSignal(int id)
{
    id ? reject() : accept();
}

void CameraSetting::slot_SearchCameraFromLan()
{
    LanSearchCamera *search = new LanSearchCamera;
    search->exec();
}
