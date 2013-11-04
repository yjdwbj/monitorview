#include "setting_panel.h"
#include "addnewcamera.h"
#include "searchcamera.h"

SettingPanel::SettingPanel(QWidget *parent)
    :QWidget(parent)
{

    QGroupBox *gbox_main = new QGroupBox();

    QGroupBox *gbox_addnew = new QGroupBox(gbox_main);

    QVBoxLayout *lay_addnew = new QVBoxLayout(gbox_addnew);
    QPushButton *btn_serachCamera = new QPushButton("自动查找且新增摄像机");
    connect(btn_serachCamera,SIGNAL(clicked()),SLOT(slot_searchCamera()));

    QPushButton *btn_addnewCamera = new QPushButton("新增摄像机");
    connect(btn_addnewCamera,SIGNAL(clicked()),SLOT(slot_addnewCamera()));
    QPushButton *btn_addnewGroup = new QPushButton("新增组");

    lay_addnew->addWidget(btn_serachCamera);
    lay_addnew->addWidget(btn_addnewCamera);
    lay_addnew->addWidget(btn_addnewGroup);
    lay_addnew->addStretch();








    QList<itemWidget> itemlist;
    itemlist  << qMakePair(1,tr(":/lcy/images/one.png")) << qMakePair(2,tr(":/lcy/images/four.png"))
              << qMakePair(3,tr(":/lcy/images/nine.png")) << qMakePair(4,tr(":/lcy/images/sixteen.png"))
              << qMakePair(othermenu,tr(":/lcy/images/other.png")) << qMakePair(fullscreen,tr(":/lcy/images/fullscreen.png"));
    VHWidget *widget = new VHWidget(itemlist,VHWidget::Horizontal);
    connect(widget,SIGNAL(gridofnumer(int)),SIGNAL(sig_gridofnumber(int)));

//    VHWidget *ctrol = new VHWidget(itemlist,VHWidget::Horizontal);
    QPushButton *btn_open = new QPushButton("开始播放");
    connect(btn_open,SIGNAL(clicked()),SIGNAL(StartPlay()));
    QPushButton *btn_stop = new QPushButton("停止播放");
    connect(btn_stop,SIGNAL(clicked()),SIGNAL(StopPlay()));


//    ViewFrame *view_ctrl = new ViewFrame;
//    view_ctrl->setGridnumber(2,3);
//    view_ctrl->setFixedSize(150,100);



    main_layout = new QVBoxLayout(gbox_main);
    main_layout->addWidget(gbox_addnew);
    main_layout->addWidget(widget);
    main_layout->addWidget(btn_open);
    main_layout->addWidget(btn_stop);
    main_layout->setMargin(0);
    this->setLayout(main_layout);
}


void SettingPanel::slot_addnewCamera()
{
    SetCameraType *type = new SetCameraType;

}

void SettingPanel::slot_searchCamera()
{



    SearchCamera *search = new SearchCamera();

    if(search->exec())
    {
        QLayoutItem *litem = main_layout->takeAt(0);
        if(litem)
            litem->widget()->hide();
        playlist = search->getVaildCameraList();
        CameraView *cv = new CameraView(playlist);
        main_layout->insertWidget(0,cv);

    }
}


void SettingPanel::slot_StartPlayer()
{

}

SettingPanel::~SettingPanel()
{

}
