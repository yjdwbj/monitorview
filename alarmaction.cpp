#include "alarmaction.h"
#include <QtWidgets/QComboBox>



static QString actionTerm("声音|录像|拍照|电子邮件|FTP上传|拨打电话|短信猫|Skype|调用预置位|应用程序|网页链接");
static const QStringList TermList(actionTerm.split("|"));
typedef QLayout*(*Action)(void);
 std::map<std::string,Action> funclist;

//static QLayout* ActionForAudio();
//static QLayout* ActionForVideo();
//static QLayout* ActionForPhoto();
//static QLayout* ActionForEmail();
//static QLayout* ActionForFTPUpload();
//static QLayout* ActionForDial_up();
//static QLayout* ActionForSMS();
//static QLayout* ActionForSkype();
//static QLayout* ActionForCallPreset();
//static QLayout* ActionForApplication();
//static QLayout* ActionForWebLink();



static QLayout* ActionForAudio()
{
    QVBoxLayout *lay_audio = new QVBoxLayout();
    QRadioButton *rbtn_audio = new QRadioButton("声音");
    QComboBox *cbbox_audio = new QComboBox;
    QRadioButton *rbtn_afile = new QRadioButton("声音文件");

    QHBoxLayout *lay_h = new QHBoxLayout();
    QLineEdit *edt_file = new QLineEdit;
    QPushButton *btn_openfile = new QPushButton;
    lay_h->addWidget(edt_file);
    lay_h->addWidget(btn_openfile);

    QCheckBox *cbox_read = new QCheckBox("朗读文字");
    QLineEdit *edt_text = new QLineEdit("test text");



    lay_audio->addWidget(rbtn_audio,Qt::AlignLeft);
    lay_audio->addWidget(cbbox_audio,Qt::AlignRight);
    lay_audio->addWidget(rbtn_afile,Qt::AlignLeft);
    lay_audio->addWidget(rbtn_audio);
    lay_audio->addLayout(lay_h);
    lay_audio->addWidget(cbox_read,Qt::AlignLeft);
    lay_audio->addWidget(edt_text,Qt::AlignRight);


    LabAndWidget *voice_source = new LabAndWidget("语音",new QComboBox,QStringList());
    lay_audio->addWidget(voice_source);

    QHBoxLayout *lay_h1 = new QHBoxLayout;
    QPushButton *btn_test = new QPushButton("测试");
    QLabel *lab_test = new QLabel("*发生异常情况时，会播放上面选择的声音");
    lay_h1->addWidget(btn_test);
    lay_h1->addWidget(lab_test);
    lay_audio->addLayout(lay_h1);

    return lay_audio;


}

static QLayout* ActionForVideo()
{
     QVBoxLayout *lay_v = new QVBoxLayout;
     QLabel *lab = new QLabel("在发生异常情况时，会自动保存录像");
     lay_v->addWidget(lab);
     return lay_v;

}

static QLayout* ActionForPhoto()
{

      QVBoxLayout *lay_g = new QVBoxLayout;
      lay_g->addWidget(new LabAndWidget("拍照张数:",new QSpinBox,30));
      lay_g->addWidget(new LabAndWidget("每隔多少秒拍一张",new QSpinBox,1));
      lay_g->addWidget(new QLabel("在发生异常情况时，会自动拍照"));
      lay_g->addStretch();
      return lay_g;

}

static QLayout* ActionForEmail()
{
    QVBoxLayout *email_layout = new QVBoxLayout;
    const QString appendtext("不附带文件，报警触发时的前后2张图片，所有报警图片，报警录像，所有报警图片和录像");
    LabAndWidget *receiver = new LabAndWidget("收件人:",new QLineEdit);
    LabAndWidget *appendfile = new LabAndWidget("附带文件:",new QComboBox,appendtext.split("，"));

    QGroupBox *gbox_smtp = new QGroupBox("配置发送服务");
    QGridLayout *lay_smtp = new QGridLayout(gbox_smtp);
    LabAndWidget *smtp_srv = new LabAndWidget("邮件发送（SMTP）服务器:",new QLineEdit);
    LabAndWidget *smtp_port = new LabAndWidget("端口:",new QLineEdit("25"));
    QCheckBox *smtp_identify = new QCheckBox("服务器需要身份验证");
    static QString tls("None,STARTTLS,TLS");
    LabAndWidget *smtp_ssl = new LabAndWidget("SSL:",new QComboBox,tls.split(","));
    LabAndWidget *accout = new LabAndWidget("帐号:",new QLineEdit);
    LabAndWidget *passwd = new LabAndWidget("密码:",new QLineEdit);
    LabAndWidget *send_box = new LabAndWidget("发送邮箱:",new QLineEdit);

    lay_smtp->addWidget(smtp_srv,0,0);
    lay_smtp->addWidget(smtp_port,1,0);
    lay_smtp->addWidget(smtp_identify,1,1);
    lay_smtp->addWidget(smtp_ssl,2,0);
    lay_smtp->addWidget(accout,3,0);
    lay_smtp->addWidget(passwd,3,1);
    lay_smtp->addWidget(send_box,4,0,1,2);

    email_layout->addWidget(receiver);
    email_layout->addWidget(appendfile);
    email_layout->addWidget(gbox_smtp);

    QHBoxLayout *lay_h = new QHBoxLayout;
    QPushButton *btn_test = new QPushButton("测试");
    btn_test->setFixedWidth(30);
    QLineEdit *edt_res = new QLineEdit;
    lay_h->addWidget(btn_test);
    lay_h->addWidget(edt_res);

    email_layout->addLayout(lay_h);
    return email_layout;



}

static QLayout* ActionForFTPUpload()
{

    QGridLayout *ftp_layout = new QGridLayout;
    static QString files("报警触发时的前后2张图片，所有报警图片，报警录像，所有报警图片和录像");
    LabAndWidget *upload_file = new LabAndWidget("上传文件:",new QComboBox,files.split("，"));
    LabAndWidget *ftp_srv = new LabAndWidget("FTP 服务器IP:",new QLineEdit,LabAndWidget::Vertical);
    LabAndWidget *ftp_port = new LabAndWidget("FTP 连接端口:",new QLineEdit("21"),LabAndWidget::Vertical);
    LabAndWidget *ftp_accout = new LabAndWidget("用户名:",new QLineEdit,LabAndWidget::Vertical);
    LabAndWidget *ftp_passwd = new LabAndWidget("密码:",new QLineEdit,LabAndWidget::Vertical);
    LabAndWidget *upload_dir = new LabAndWidget("上传目录:",new QLineEdit);

    QCheckBox *pasv_mode = new QCheckBox("PASV模式:");
    ftp_layout->addWidget(upload_file,0,0,1,2);
    ftp_layout->addWidget(ftp_srv,1,0);
    ftp_layout->addWidget(ftp_port,1,1);
    ftp_layout->addWidget(ftp_accout,2,0);
    ftp_layout->addWidget(ftp_passwd,2,1);
    ftp_layout->addWidget(upload_dir,3,0,1,2);
    ftp_layout->addWidget(pasv_mode,4,0);


    QHBoxLayout *lay_h = new QHBoxLayout;
    QPushButton *btn_test = new QPushButton("测试登录");
    QLineEdit *edt_res = new QLineEdit;
    lay_h->addWidget(btn_test);
    lay_h->addWidget(edt_res);

    ftp_layout->addLayout(lay_h,5,0,1,2);
    return ftp_layout;

}

static QLayout* ActionForDial_up()
{
     static QString linetype("WAN微型端口(L2TP),LPT,H323线路");
     LabAndWidget *dial_device = new LabAndWidget("拨号设备:",new QComboBox,linetype.split(","));
     LabAndWidget *call_number = new LabAndWidget("呼叫号码:",new QLineEdit);

     QPushButton *btn_test = new QPushButton("测试");
     QLabel *lab_commit = new QLabel("*该功能需要您的计算机上安装支持拨号的调制解调器.");
     QVBoxLayout *dial_layout = new QVBoxLayout;

     dial_layout->addWidget(dial_device);
     dial_layout->addWidget(call_number);
     dial_layout->addWidget(btn_test);
     dial_layout->addWidget(lab_commit);
     return dial_layout;


}

static QLayout* ActionForSMS()
{
      QVBoxLayout *sms_layout = new QVBoxLayout;
      LabAndWidget *sms_port = new LabAndWidget("短信猫端口:",new QComboBox,QStringList());
      LabAndWidget *sms_phone = new LabAndWidget("接收短信手机号码:",new QLineEdit,LabAndWidget::Vertical);
      QLabel *notes = new QLabel("*多个手机号用逗号隔开.");
      QCheckBox *cbox_invatel = new QCheckBox("两条短信的时间间隔需要大于(分)");
      QSpinBox *sb_num = new QSpinBox;

      QPushButton *btn_test = new QPushButton("测试");
      btn_test->setFixedWidth(50);
      sms_layout->setSpacing(3);


      sms_layout->addWidget(sms_port);
      sms_layout->addWidget(sms_phone);
      sms_layout->addWidget(notes);
      sms_layout->addWidget(cbox_invatel);
      sms_layout->addWidget(sb_num);
      sms_layout->addWidget(btn_test);
      sms_layout->addStretch();
      return sms_layout;

}

static QLayout* ActionForSkype()
{
    QCheckBox *enable_skype = new QCheckBox("发送Skype消息");
    LabAndWidget *account = new LabAndWidget("接收方的Skype帐号:",new QLineEdit);
    QCheckBox *enable_sms = new QCheckBox("发送短信");
    LabAndWidget *phone_number = new LabAndWidget("接收短信的手机号码:",new QLineEdit);
    LabAndWidget *sms_text = new LabAndWidget("信息内容:",new QLineEdit);

    QCheckBox *call = new QCheckBox("呼叫");
    LabAndWidget *call_account = new LabAndWidget("被呼叫的Skype帐号或电话号码:",new QLineEdit);

    QPushButton *btn_test = new QPushButton("测试");

    QLabel *notice = new QLabel("*需要安装Skype软件");

    QGridLayout *skype_layout = new QGridLayout;
    skype_layout->addWidget(enable_skype,0,0);
    skype_layout->addWidget(account,0,1);
    skype_layout->addWidget(enable_sms,1,0);
    skype_layout->addWidget(phone_number,1,1);
    skype_layout->addWidget(sms_text,2,0);

    skype_layout->addWidget(call,2,1);
    skype_layout->addWidget(call_account,3,0);
    skype_layout->addWidget(btn_test,4,0);
    btn_test->setFixedWidth(30);
    skype_layout->addWidget(notice,5,0);
    return skype_layout;

}


static QLayout* ActionForCallPreset()
{
    LabAndWidget *preset = new LabAndWidget("预置位号:",new QSpinBox,1);
    QLabel *notice = new QLabel("在发现异常情况时，会调用摄像机的预置位，转动到指定位置.");
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(preset);
    lay->addWidget(notice);
    return lay;
}

static QLayout* ActionForApplication()
{
    LabAndWidget *app = new LabAndWidget("应用程序:",new QLineEdit);
    LabAndWidget *arg = new LabAndWidget("运行参数:",new QLineEdit);

    QCheckBox *run_once = new QCheckBox("只运行一次");
    QPushButton *btn_test = new QPushButton("测试");
    btn_test->setFixedWidth(30);
    QLabel *notice = new QLabel("*当发生报警时应用程序会被运行");
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(app);
    lay->addWidget(arg);
    lay->addWidget(run_once);
    lay->addWidget(btn_test);
    lay->addWidget(notice);
    return lay;
}

static QLayout *ActionForWebLink()
{
    LabAndWidget *url = new LabAndWidget("应用程序:",new QLineEdit);
    QPushButton *btn_test = new QPushButton("测试");
    btn_test->setFixedWidth(30);
    QLabel *notice = new QLabel("*当发生报警时会访问指定的网页地址（不会打开网页浏览器）");
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(url);
    lay->addWidget(btn_test);
    lay->addWidget(notice);
    return lay;
}

AlarmAction::AlarmAction(QWidget *parent)
    :QGroupBox(parent),signalmap(new QSignalMapper(this)),
      lay_stack(new QStackedLayout),main_layout(new QHBoxLayout),
      tabWidgets(new QTableWidget)
{

       funclist[TermList.at(0).toStdString()]= &ActionForAudio;
       funclist[TermList.at(1).toStdString()]= &ActionForVideo;
       funclist[TermList.at(2).toStdString()]= &ActionForPhoto;
       funclist[TermList.at(3).toStdString()]= &ActionForEmail;
       funclist[TermList.at(4).toStdString()]= &ActionForFTPUpload;
       funclist[TermList.at(5).toStdString()]= &ActionForDial_up;
       funclist[TermList.at(6).toStdString()]= &ActionForSMS;
       funclist[TermList.at(7).toStdString()]= &ActionForSkype;
       funclist[TermList.at(8).toStdString()]= &ActionForCallPreset;
       funclist[TermList.at(9).toStdString()]= &ActionForApplication;
       funclist[TermList.at(10).toStdString()]= &ActionForWebLink;

       this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setTitle("报警动作");
    tabWidgets->setColumnCount(1);
    tabWidgets->horizontalHeader()->setHidden(true);
    tabWidgets->verticalHeader()->setHidden(true);
    tabWidgets->horizontalHeader()->setStretchLastSection(true);
    tabWidgets->setFixedWidth(100);
    tabWidgets->setStyleSheet("QCheckBox::hover { background-color: #00ff00;}");

    std::map<std::string,Action>::iterator  iterator;
    int i = 0;
    foreach(const QString &str,TermList)
    {
        QCheckBox *cbox = new QCheckBox(str);
        tabWidgets->insertRow(i);
        tabWidgets->setCellWidget(i,0,cbox);
        signalmap->setMapping(cbox,i++);
        iterator =  funclist.find(str.toStdString());
        if(iterator != funclist.end())
        {
            Action memFunc = iterator->second;
            lay_stack->addWidget( WidgetFromLayout(QString::fromStdString(iterator->first),memFunc()));
        }
        connect(cbox,SIGNAL(clicked()),signalmap,SLOT(map()));
    }


    main_layout->addWidget(tabWidgets);
    main_layout->addLayout(lay_stack);
    main_layout->addStretch();
    setLayout(main_layout);
    connect(signalmap,SIGNAL(mapped(int)),SLOT(slot_ActionHasTriggered(int)));
    this->adjustSize();
//    connect(tabWidgets,SIGNAL(pressed(QModelIndex)),SLOT(slot_ItemHasClicked(QModelIndex)));

}

void AlarmAction::slot_ItemHasClicked(QModelIndex midx)
{
    lay_stack->setCurrentIndex(midx.row());
    tabWidgets->setCurrentIndex(midx);
}



QWidget*  AlarmAction::WidgetFromLayout(const QString &name,  QLayout *lay)
{
    QTabWidget *tab = new QTabWidget();
//    tab->setFixedWidth(400);

   QWidget *w = new QWidget;
   w->setLayout(lay);
   w->adjustSize();
   tab->addTab(w,name);

   return (QWidget *)tab;
}




void AlarmAction::slot_ActionHasTriggered(int id)
{
    tabWidgets->setCurrentItem(tabWidgets->item(id,0));
    lay_stack->setCurrentIndex(id);

}
