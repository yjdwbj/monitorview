#include "system_setting.h"
#include "sqldriver.h"
#include <QTextEdit>
#include <QFontDialog>
#include <QFileDialog>
#include <QSettings>

static QString items("存储|显示|自动运行|其它");
static QString btns("添加|删除");


QLayout* SystemSetting::StoragePanel()
{


    QVBoxLayout *lay = new QVBoxLayout;

    QGroupBox *gbox_path = new QGroupBox("录像报警数据保存路径:");
    QHBoxLayout *lay_path = new QHBoxLayout(gbox_path);

     pathText = new QListWidget;
    pathText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);



    GroupBtnWidget *btn_addPath = new GroupBtnWidget(btns.split("|"),GroupBtnWidget::Vertical);
    lay_path->addWidget(pathText);
    lay_path->addWidget(btn_addPath);
    connect(btn_addPath,SIGNAL(SignalById(int)),SLOT(slot_addORdeleteStoragePath(int)));

    QGroupBox *gbox_savedays = new QGroupBox("录像报警数据保留天数:");
    QVBoxLayout *lay_save = new QVBoxLayout(gbox_savedays);
    LabAndWidget *record = new LabAndWidget("录像记录:",new QSpinBox,7);
    LabAndWidget *alarm = new LabAndWidget("报警记录:",new QSpinBox,30);
    lay_save->addWidget(record);
    lay_save->addWidget(alarm);

    LabAndWidget *record_len = new LabAndWidget("录像文件分段长度(分钟):",
                                                new QSpinBox,60);
    lay->addWidget(gbox_path);
    lay->addWidget(gbox_savedays);
    lay->addWidget(record_len);
    return lay;
}

void SystemSetting::slot_addORdeleteStoragePath(int id)
{
    if(id)   // 1 == delete
    {
        pathText->takeItem(pathText->currentRow());
    }
    else  // 0 == add
    {


        QString path = QFileDialog::getExistingDirectory(this,"选择录像存储目录");
        if(path.isEmpty())
            return;
        pathText->addItem(path);
    }

}


QLayout *SystemSetting::ViewPanel()
{
    QGroupBox *gbox_view = new QGroupBox("视频上显示的内容");
    QVBoxLayout *lay_view = new QVBoxLayout(gbox_view);
    cbox_viewname = new QCheckBox("显示摄像机名称");
    cbox_viewtimestamp = new QCheckBox("显示时间戳");
    QPushButton *btn_font = new QPushButton("字体");
    btn_font->setFixedWidth(40);
    QLineEdit *txt_view  = new QLineEdit("Sample Text");
    txt_view->setFixedSize(220,40);
    txt_view->setEnabled(false);
    txt_view->setObjectName("text");
    QObject::connect(btn_font,SIGNAL(clicked()),this,SLOT(slot_fontdialog()));
    lay_view->addWidget(cbox_viewname);
    lay_view->addWidget(cbox_viewtimestamp);
    lay_view->addWidget(btn_font,Qt::AlignLeft);
    lay_view->addWidget(txt_view);

    QVBoxLayout *main_lay = new QVBoxLayout;
    main_lay->addWidget(gbox_view);
    static QString cboxs("视频窗口显示标题栏|视频窗口显示状态栏|画面显示保持原始比例|平时不显示实时画面，可以降低性能使用");
    GroupCheckBox *misc_cbox = new GroupCheckBox(cboxs.split("|"),"",Vertical);
    main_lay->addWidget(misc_cbox);
    return main_lay;



}

void SystemSetting::slot_fontdialog()
{
    bool f = false;
    QFont font = QFontDialog::getFont(&f);
    if(f)
    {
        QLineEdit *txt_view  = this->findChild<QLineEdit*>("text");
        txt_view->setFont(font);
    }
}

SystemSetting::SystemSetting(QWidget *parent)
    :QDialog(parent),
    listwidget(new QListWidget),
    stackLayout(new QStackedLayout),
    signalmap(new QSignalMapper(this))
{
    this->setWindowTitle("系统设置");

    this->setObjectName("SystemSettings");
    StartAfterBoot = new QCheckBox("系统启动时自动运行程序");
    StartAfterBoot->setObjectName("StartAfterBoot");
    QGridLayout *main_layout = new QGridLayout;
    listwidget->addItems(items.split("|"));
    connect(listwidget,SIGNAL(currentRowChanged(int)),SLOT(slot_ListRowChanged(int)));

    stackLayout->addWidget(getWidgetFromLayout(StoragePanel()));
    stackLayout->addWidget(getWidgetFromLayout(ViewPanel()));
    stackLayout->addWidget(getWidgetFromLayout(AutoRunning()));
    stackLayout->addWidget(getWidgetFromLayout(OtherPanel()));

    main_layout->addWidget(listwidget,0,0);
    main_layout->addLayout(stackLayout,0,1);
    GroupBtnWidget *btn = new GroupBtnWidget(yesorno.split(","));
    btn->getItemByid(0)->setObjectName("Yes");
    connect(btn,SIGNAL(SignalById(int)),SLOT(slot_yesornocommit(int)));

//    connect(listwidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*))
    connect(StartAfterBoot,SIGNAL(toggled(bool)),SLOT(on_StartAfterBoot_toggled(bool)));

    main_layout->addWidget(btn,1,1,1,2);
    setLayout(main_layout);
    QMetaObject::connectSlotsByName(this);

}

void SystemSetting::on_StartAfterBoot_toggled(bool checked)
{
    user->setEnabled(checked);
    pass->setEnabled(checked);
    tray->setEnabled(checked);
}


void SystemSetting::slot_yesornocommit(int id)
{
    id ? reject() : accept();
}


void SystemSetting::on_Yes_clicked()
{
    QSettings set(qApp->applicationFilePath()+tr("options.ini"),QSettings::IniFormat);

    QString g("global/");
    set.setValue(g+"Layout",QString());
    set.setValue(g+"AutoTurn",StartAfterBoot->isChecked());
    set.setValue(g+"WindowsState",QVariant());
    set.setValue(g+"SaveRecDay",QVariant());
    set.setValue(g+"SaveAlarmDay",QVariant());
    set.setValue(g+"RecSecTime",QVariant());
    set.setValue(g+"ProxyIP",QVariant());
    set.setValue(g+"AutoTray",tray->isChecked());
    set.setValue(g+"AutoTurn_Time",QVariant());
    set.setValue(g+"ErrAlarm_Time",QVariant());
    set.setValue(g+"AlarmSnd",QVariant());
    set.setValue(g+"AutoStartDev",autoconnect->isChecked());
    set.setValue(g+"AutoDecRate",QVariant());
    set.setValue(g+"AutoDecRateNum",QVariant());
    set.setValue(g+"VideoCache",QVariant());
    set.setValue(g+"ShowKey",QVariant());
    set.setValue(g+"Paint_ShowTitle",QVariant());
    set.setValue(g+"Paint_ShowStatus",QVariant());
    set.setValue(g+"VideoFixRatio",QVariant());
    set.setValue(g+"CheckUpate",QVariant());
    set.setValue(g+"Screen",QVariant());
    set.setValue(g+"ShowVideo",QVariant());


}

QLayout *SystemSetting::AutoRunning()
{

    QGroupBox *gbox_login = new QGroupBox("自动使用下面操作员");
    user = new LabAndWidget("操作员:",new QLineEdit);
    pass = new LabAndWidget("密码:",new QLineEdit);
    user->setEnabled(false);
    pass->setEnabled(false);
    tray = new QCheckBox("程序自动启动后进入系统托盘区");
    tray->setEnabled(false);
    QVBoxLayout *lay_login = new QVBoxLayout(gbox_login);
    lay_login->addWidget(user);
    lay_login->addWidget(pass);
    lay_login->addWidget(tray);

    autoconnect = new QCheckBox("程序启动时自动连接所有摄像机");

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(StartAfterBoot);
    main_layout->addWidget(gbox_login);
    main_layout->addWidget(autoconnect);
    main_layout->addStretch();
    return main_layout;
}


QLayout* SystemSetting::OtherPanel()
{
    QVBoxLayout *main_layout = new QVBoxLayout();
    srvaddr = new LabAndWidget("服务器地址:",
                                             new QLineEdit("video.zh-jl.com"));
    page_up = new LabAndWidget("自动翻页时间间隔(秒):",
                                             new QSpinBox,10);
    loss_camera = new LabAndWidget("摄像机丢失图像后多少时间触发报警(秒):",
                                          new QSpinBox,60);
    log_img = new  QCheckBox("图像丢失后进行声音报警");
    check_version = new QCheckBox("启动时检测是否有新版本");

    main_layout->addWidget(srvaddr);
    main_layout->addWidget(page_up);
    main_layout->addWidget(loss_camera);
    main_layout->addWidget(log_img);
    main_layout->addWidget(check_version);
    main_layout->addStretch();
    return main_layout;
}

void SystemSetting::slot_ListRowChanged(int id)
{
    stackLayout->setCurrentIndex(id);
}

void SystemSetting::accept()
{
    close();
}

void SystemSetting::closeEvent(QCloseEvent *e)
{
    if(StartAfterBoot->isChecked())
    {
        QLineEdit *u = (QLineEdit*)user->getTwoObj();
        QLineEdit *p = (QLineEdit*)pass->getTwoObj();
        if(u->text().isEmpty() || p->text().isEmpty())
        {
            QMessageBox::warning(this,"出错啦",
                                 "请输入自动运行时的用户名与正解的密码,否则不用运行.");
            e->ignore();
        }
    }
    e->accept();
}
