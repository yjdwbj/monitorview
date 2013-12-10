#include "setting_panel.h"
#include "addnewcamera.h"
#include "system_setting.h"
#include "searchcamera.h"
#include "camera_settings.h"
#include "sqldriver.h"


static QString allbutton("开始所有|停止所有|系统设置|录像记录|报警记录|其它");


CameraView::CameraView(QWidget *parent)
    :QTreeWidget(parent)
{
    CameraIcon = QIcon(":/lcy/images/cameraicon.png");
    GroupIcon = QIcon(":/lcy/images/camera_group.png");
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    this->header()->hide();
    oldlist = QStringList();
//    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(slot_itemclicked(QTreeWidgetItem*,int)));
}

void CameraView::slot_itemclicked(QTreeWidgetItem *w, int c)
{
    this->setCurrentItem(w,c);
    this->setItemSelected(w,true);
}

void CameraView::addItem(const QStringList &list,bool isgroup,QTreeWidgetItem *root)
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

    if(event->button() == Qt::LeftButton )
        {
            QTreeWidgetItem *w = itemAt(event->pos());

            if(w)
            {
                if(!w->type())
                {
                   w->setExpanded(!w->isExpanded());
                }

            }
    }
}

void CameraView::mousePressEvent(QMouseEvent *event)
{


    QTreeWidgetItem *w = this->currentItem();

    if(event->button() == Qt::RightButton)
        {

            if(w)
            {
                QMenu *menu = new QMenu();
                QList<QAction*> list;
                 this->clearSelection();
                this->setCurrentItem(w);
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

                    connect(menu,SIGNAL(triggered(QAction*)),SIGNAL(triggerViewMenu(QAction*)));
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

                    connect(menu,SIGNAL(triggered(QAction*)),SIGNAL(triggerViewMenu(QAction*)));
                    menu->addActions(list);

                    menu->exec(this->mapToGlobal( event->pos()));
                }


            }
        }
    else if(event->button() == Qt::LeftButton )
    {
        QTreeWidgetItem *ww = itemAt(event->pos());

        if(ww)
        {
            this->clearSelection();
            this->setCurrentItem(ww);
           ww->setSelected(true);

        }
    }
    event->accept();
}

void SettingPanel::slot_ProcessMenuAction(QAction *act)
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
    {
         QTreeWidgetItem *w =  m_TreeView->currentItem();

         camera_settings *cs = new camera_settings(0, w ? w->text(0):"");
        cs->exec();
        delete cs;
    }
        break;
    case 5: // delete camera
    case 6:
    {
        SqlInstance sql;
        sql.DeleteCamera(m_TreeView->currentItem()->text(0));
//        sql.closedb();
        m_TreeView->takeTopLevelItem(m_TreeView->indexOfTopLevelItem(m_TreeView->currentItem()));
        if(!m_TreeView->topLevelItemCount())
           {
            m_TreeView->hide();
            gbox_addnew->show();
           }
    }


        break;
    case 7: // added group
       slot_addCameraGroup();
        break;
    case 8:
        slot_addnewCamera();
        break;
    case 9:
        slot_searchCamera();
        break;

    }
}


SettingPanel::SettingPanel(QWidget *parent)
    :QWidget(parent),
      m_TreeView(new CameraView)
{
    connect(m_TreeView,SIGNAL(triggerViewMenu(QAction*)),SLOT(slot_ProcessMenuAction(QAction*)));
    QGroupBox *gbox_main = new QGroupBox();

    gbox_addnew = new QGroupBox(gbox_main);

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


    main_layout = new QVBoxLayout(gbox_main);
    main_layout->addWidget(gbox_addnew);
    main_layout->addWidget(m_TreeView);
    main_layout->addWidget(widget);
    main_layout->addLayout(lay_control);
    main_layout->setMargin(0);
    this->setLayout(main_layout);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    this->adjustSize();

    SqlInstance sql;
    QStringList namelist = sql.GetColumnList("camera_settings","camera_name");
//    sql.closedb();
    foreach(const QString &str,namelist)
    {
        m_TreeView->addItem(QStringList() << str,false);
    }



    if(m_TreeView->topLevelItemCount() > 0)
    {
        gbox_addnew->hide();
        m_TreeView->show();
    }
    else
    {
        gbox_addnew->show();
         m_TreeView->hide();
    }

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
       gbox_addnew->hide();
       m_TreeView->addItem(QStringList(g->edit->text()),true);
       m_TreeView->show();
       m_TreeView->setCurrentItem(m_TreeView->topLevelItem(m_TreeView->topLevelItemCount()-1),1);
       m_TreeView->setItemSelected(m_TreeView->currentItem(),true);
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



        playlist = search->getVaildCameraList();
        SqlInstance sql;
        int num = sql.GetTableCount("camera_settings");
        QStringList org;

        foreach(const QString &str,playlist) // insert record to sql db;
        {
            QString v = QString::number(num++)+","+str;
            org = v.split(',');
           QStringList cameraset;
           cameraset << org.at(0) << org.at(1) << org.at(0) << org.at(3)
           << "admin" << "admin" << "0" << "0" << "" << "" << "" << ""
                         << org.at(0) << org.at(0);
           SqlInstance::InsertItem("camera_settings",cameraset);
           QStringList hostinfo;

           hostinfo << org.at(0)  << org.at(2) << org.at(4) << org.at(5)
                    << org.at(6) << "" << org.at(7) <<  org.at(8);
          SqlInstance::InsertItem("hostinfo",hostinfo);

        }

        QStringList vaildlist = SqlInstance::GetColumnList("camera_settings","camera_name");

        if(m_TreeView->topLevelItemCount() > 0)
            m_TreeView->addItem(vaildlist,false,!m_TreeView->currentItem()->type() ?
                                m_TreeView->currentItem() : (QTreeWidgetItem*)(0));
        else
            m_TreeView->addItem(vaildlist,false,(QTreeWidgetItem*)(0));

        gbox_addnew->hide();
        m_TreeView->show();
        emit addedNewCamera(playlist.count());

    }
    delete  search;

}


QStringList SettingPanel::getPlayList() const
{
    SqlInstance sql;
    QStringList ipname = sql.GetColumnList("camera_settings","camera_name");
//    sql.closedb();

    return ipname;

}

void SettingPanel::slot_StartPlayer()
{

}

SettingPanel::~SettingPanel()
{

}
