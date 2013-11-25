#include "setting_panel.h"
#include "addnewcamera.h"
#include "system_setting.h"
#include "searchcamera.h"


static QString allbutton("开始所有|停止所有|系统设置|录像记录|报警记录|其它");


CameraView::CameraView(QWidget *parent)
    :QTreeWidget(parent)
{
    CameraIcon = QIcon(":/lcy/images/cameraicon.png");
    GroupIcon = QIcon(":/lcy/images/camera_group.png");
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->header()->hide();
    oldlist = QStringList();
}

void CameraView::addItem(const QStringList &list,bool isgroup,QTreeWidget *root)
{
    oldlist = list;

    foreach(const QString &str,list)
    {
        QTreeWidgetItem *w = new QTreeWidgetItem(root,QStringList(str.section(",",0,0)),isgroup ? 0 : 1000);
        w->setIcon(0,isgroup ?  GroupIcon : CameraIcon);
        this->addTopLevelItem(w);
    }
}

void CameraView::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->clearSelection();
    if(event->button() == Qt::LeftButton )
        {
            QTreeWidgetItem *w = itemAt(event->pos());

            if(w)
            {
               w->setSelected(true);
            }
    }
}

void CameraView::mousePressEvent(QMouseEvent *event)
{
     this->clearSelection();
//    if(event->type() == Qt::LeftButton)
//    {
//        QTreeWidgetItem *w = itemAt(event->pos());
//        if(w)
//        {
//            w->setSelected(true);
//        }
//    }else
    if(event->button() == Qt::RightButton)
        {
            QTreeWidgetItem *w = itemAt(event->pos());

            if(w)
            {
                QMenu *menu = new QMenu();
                QList<QAction*> list;

                w->setSelected(true);
                if(!w->type())  // here is group menu
                {

                    foreach(const QString &str,GroupMenu.split("|"))
                    {
                        if(str.startsWith("-"))
                        {
                            list.append(menu->addSeparator());
                        }
                        else
                            list << new QAction(str,menu);
                    }

                    connect(menu,SIGNAL(triggered(QAction*)),SLOT(slot_ViewMenu(QAction*)));
                    menu->addActions(list);

                    menu->exec(this->mapToGlobal( event->pos()));
                }
                else
                {
                    foreach(const QString &str,CameraMenu.split("|"))
                    {
                        if(str.startsWith("-"))
                        {
                            list.append(menu->addSeparator());
                        }
                        else
                            list << new QAction(str,menu);
                    }

                    connect(menu,SIGNAL(triggered(QAction*)),SLOT(slot_ViewMenu(QAction*)));
                    menu->addActions(list);

                    menu->exec(this->mapToGlobal( event->pos()));
                }


            }
        }
    event->accept();
}

void CameraView::slot_ViewMenu(QAction *act)
{
    const QStringList actions(MenuAction.split("|"));
    switch(actions.indexOf(act->text()))
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        CameraSetting *cs = new CameraSetting("test");
        cs->exec();
        break;
    }
}


SettingPanel::SettingPanel(QWidget *parent)
    :QWidget(parent),
      m_TreeView(new CameraView)
{

    QGroupBox *gbox_main = new QGroupBox();

    QGroupBox *gbox_addnew = new QGroupBox(gbox_main);

    QVBoxLayout *lay_addnew = new QVBoxLayout(gbox_addnew);
    QPushButton *btn_serachCamera = new QPushButton("自动查找且新增摄像机");
    connect(btn_serachCamera,SIGNAL(clicked()),SLOT(slot_searchCamera()));

    QPushButton *btn_addnewCamera = new QPushButton("新增摄像机");
    connect(btn_addnewCamera,SIGNAL(clicked()),SLOT(slot_addnewCamera()));
    QPushButton *btn_addnewGroup = new QPushButton("新增组");
    connect(btn_addnewGroup,SIGNAL(clicked()),SLOT(slot_addCameraGroup()));

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


    QGridLayout *lay_control = new QGridLayout;

    int x =0 ,y = 0,n=0;
    QSignalMapper *ctrl_signal = new QSignalMapper;
    foreach(const QString &str,allbutton.split("|"))
    {
        QPushButton *btn = new QPushButton(str);
        btn->setFixedSize(60,60);
        if(y > 2)
        {
            y=0;
            x =1;
        }

        ctrl_signal->setMapping(btn,n++);
        lay_control->addWidget(btn,x,y++);
        connect(btn,SIGNAL(clicked()),ctrl_signal,SLOT(map()));
    }

    connect(ctrl_signal,SIGNAL(mapped(int)),SLOT(slot_Process_Ctrl_signals(int))); // control play


//    ViewFrame *view_ctrl = new ViewFrame;
//    view_ctrl->setGridnumber(2,3);
//    view_ctrl->setFixedSize(150,100);



    main_layout = new QVBoxLayout(gbox_main);
    main_layout->addWidget(gbox_addnew);
    main_layout->addWidget(widget);
    main_layout->addLayout(lay_control);
    main_layout->setMargin(0);
    this->setLayout(main_layout);
}


void SettingPanel::slot_Process_Ctrl_signals(int id)
{
    switch(id)
    {
    case 0:emit StartPlay();break;
    case 1: emit StopPlay(); break;
    case 2:
        SystemSetting *sys = new SystemSetting;
        sys->exec();
        break;


    }
}

void SettingPanel::slot_addCameraGroup()
{
   GroupDialog *g = new GroupDialog;
   if(g->exec())
   {
       QLayoutItem *litem = main_layout->takeAt(0);
       if(litem)
           litem->widget()->hide();
       m_TreeView->addItem(QStringList(g->edit->text()),true);
       main_layout->insertWidget(0,m_TreeView);
   }

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
        m_TreeView->addItem(playlist,false);
        main_layout->insertWidget(0,m_TreeView);

    }
}


void SettingPanel::slot_StartPlayer()
{

}

SettingPanel::~SettingPanel()
{

}
