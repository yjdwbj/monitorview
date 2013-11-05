#include "alarmaction.h"
#include <QtWidgets/QComboBox>



static QString actionTerm("声音|录像|拍照|电子邮件|FTP上传|拨打电话|短信猫|Skype|调用预置位|应用程序|网页链接");


AlarmAction::AlarmAction(QWidget *parent)
    :QGroupBox(parent),
      signalmap(new QSignalMapper(this)),
      lay_stack(new QStackedLayout),
      main_layout(new QHBoxLayout),
      tabWidgets(new QTableWidget)
{

    this->setTitle("报警动作");
    tabWidgets->setColumnCount(1);
    tabWidgets->horizontalHeader()->setHidden(true);
    tabWidgets->verticalHeader()->setHidden(true);
    tabWidgets->horizontalHeader()->setStretchLastSection(true);
    lay_stack->addWidget(ActionForAudio());
    lay_stack->addWidget(ActionForVideo());
    lay_stack->addWidget(ActionForPhoto());
    int i = 0;
    foreach(const QString &str,actionTerm.split("|"))
    {
        QCheckBox *cbox = new QCheckBox(str);
        tabWidgets->insertRow(i);
        tabWidgets->setCellWidget(i,0,cbox);
        signalmap->setMapping(cbox,i++);
        connect(cbox,SIGNAL(clicked()),signalmap,SLOT(map()));
    }


    main_layout->addWidget(tabWidgets);
    main_layout->addLayout(lay_stack);
    setLayout(main_layout);
//    connect(signalmap,SIGNAL(mapped(int)),SLOT(slot_ActionHasTriggered(int)));
    connect(tabWidgets,SIGNAL(clicked(QModelIndex)),SLOT(slot_ItemHasClicked(QModelIndex)));

}

void AlarmAction::slot_ItemHasClicked(QModelIndex midx)
{
    lay_stack->setCurrentIndex(midx.row());
    tabWidgets->setCurrentIndex(midx);
}

QWidget* AlarmAction::ActionForAudio()
{
    QTabWidget *tab = new QTabWidget();
    QGroupBox *gbox_audio = new QGroupBox("选择声音文件:");
    QVBoxLayout *lay_audio = new QVBoxLayout(gbox_audio);
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

    QLabel  *lab_speak = new QLabel("声音:");
    QComboBox *cbbox_speak = new QComboBox;

    lay_audio->addWidget(rbtn_audio,Qt::AlignLeft);
    lay_audio->addWidget(cbbox_audio,Qt::AlignRight);
    lay_audio->addWidget(rbtn_afile,Qt::AlignLeft);
    lay_audio->addWidget(rbtn_audio);
    lay_audio->addLayout(lay_h);
    lay_audio->addWidget(cbox_read,Qt::AlignLeft);
    lay_audio->addWidget(edt_text,Qt::AlignRight);
    lay_audio->addWidget(lab_speak,Qt::AlignLeft);
    lay_audio->addWidget(cbbox_speak,Qt::AlignRight);

    QHBoxLayout *lay_h1 = new QHBoxLayout;
    QPushButton *btn_test = new QPushButton("测试");
    QLabel *lab_test = new QLabel("*发生异常情况时，会播放上面选择的声音");
    lay_h1->addWidget(btn_test);
    lay_h1->addWidget(lab_test);
    lay_audio->addLayout(lay_h1);

    tab->addTab(gbox_audio,tr("声音"));
     return (QWidget *)tab;

}

QWidget *AlarmAction::ActionForVideo()
{
     QTabWidget *tab = new QTabWidget();
     QLabel *lab = new QLabel("在发生异常情况时，会自动保存录像");
     tab->addTab(lab,"录像");
     return (QWidget *)tab;
}

QWidget *AlarmAction::ActionForPhoto()
{
      QTabWidget *tab = new QTabWidget();


      QVBoxLayout *lay_g = new QVBoxLayout;
      lay_g->addWidget(new LabAndLineEdit("拍照张数:","","30"));
      lay_g->addWidget(new LabAndLineEdit("每隔多少秒拍一张","","1"));
      lay_g->addWidget(new QLabel("在发生异常情况时，会自动拍照"));
      lay_g->addStretch();
      QWidget *w = new QWidget;
      w->setLayout(lay_g);
      tab->addTab(w,"拍照");
       return (QWidget *)tab;
}

QWidget *AlarmAction::ActionForEmail()
{
    QTabWidget *tab = new QTabWidget();
    LabAndLineEdit *lab

    return (QWidget *)tab;
}

void AlarmAction::slot_ActionHasTriggered(int id)
{
    lay_stack->setCurrentIndex(id);

}
