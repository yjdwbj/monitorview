#include "setting_panel.h"
#include "addnewcamera.h"
#include "system_setting.h"
#include "searchcamera.h"
#include "camera_settings.h"
#include "sqldriver.h"
#include <QMap>
#include "settingfile.h"


static QString allbutton("开始所有|停止所有|系统设置|录像记录|报警记录|其它");


CameraView::CameraView(QWidget *parent)
    :QTreeWidget(parent)
{
    setObjectName("ViewCamera");
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

void CameraView::addItem(const QString &nameid, QTreeWidgetItem *root)
{
        QString key = nameid.section(",",0,0);
        QString value = nameid.section(",",1,1);
        QMap<QString,QString>::const_iterator it = mapVerifyId.find(key);
        if(!it.key().isEmpty())
            return;

        mapVerifyId[key]=value;
        QTreeWidgetItem *w = new QTreeWidgetItem(root,QStringList(value),1000);
        w->setIcon(0,CameraIcon);
        this->addTopLevelItem(w);
}

void CameraView::updateItem(const QString &key, const QString &value)
{

    int n = this->topLevelItemCount();
    for(int i = 0; i < n ;i++)
    {
        QTreeWidgetItem *w = topLevelItem(i);
        if(w)
        {
           QString k = mapVerifyId.key(w->text(0));
           if(!k.compare(key))
           {
               mapVerifyId[key] = value;
               w->setText(0,value);
               break;
           }
        }
    }


}


void CameraView::addGroup(const QString &name, QTreeWidgetItem *root)
{
    QTreeWidgetItem *w = new QTreeWidgetItem(root,QStringList(name));
    w->setIcon(0,GroupIcon);
    this->addTopLevelItem(w);
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
        if(cs->exec())
        {
            w->setText(0,cs->getCameraName());
            emit updateItemValue(cs->getVerifyId());
        }
        delete cs;
    }
        break;
    case 5: // delete camera
    case 6:
    {

        QString id = SqlInstance::getColumnFirst("camera_settings","camera_verifyid",
                                                 "camera_name",m_TreeView->currentItem()->text(0));
        SqlInstance::deleteCamera(m_TreeView->currentItem()->text(0));
        m_TreeView->takeTopLevelItem(m_TreeView->indexOfTopLevelItem(m_TreeView->currentItem()));
        if(!m_TreeView->topLevelItemCount())
           {
            m_TreeView->hide();
            gbox_addnew->show();
           }
        emit deleteCamera(id);
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
    addCameraFromSql();

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
       m_TreeView->addGroup(g->edit->text());
       m_TreeView->show();
       m_TreeView->setCurrentItem(m_TreeView->topLevelItem(m_TreeView->topLevelItemCount()-1),1);
       m_TreeView->setItemSelected(m_TreeView->currentItem(),true);
   }

}

void SettingPanel::slot_addnewCamera()
{


}

void SettingPanel::slot_searchCamera()
{
    SearchCamera *search = new SearchCamera();

    if(search->exec())
    {
        playlist = search->getVaildCameraList();
        int num = SqlInstance::getMaximumId("camera_settings","camera_id");
        QStringList org;

        QString path(qApp->applicationDirPath()+"/devs/");

        foreach(const QString &str,playlist) // insert record to sql db;
        {
            QString v = QString::number(++num)+","+str;
            org = v.split(',');
           QStringList cameraset;
           cameraset << org.at(0) << org.at(1) << org.at(0) << org.at(3)
           << "admin" << "admin" << "0" << "0" << "" << "" << "" << ""
                         << org.at(0) << org.at(0);
           SqlInstance::insertItem("camera_settings",cameraset);
           QStringList hostinfo;

           hostinfo << org.at(0)  << org.at(2) << org.at(4) << org.at(5)
                    << org.at(6) << "" << org.at(7) <<  org.at(8);
          SqlInstance::insertItem("hostinfo",hostinfo);

          initalDevSettings(path+org.at(1));


        }

        addCameraFromSql();

        gbox_addnew->hide();
        m_TreeView->show();
        emit addedNewCamera(playlist.count());
    }
    delete  search;

}



void SettingPanel::initalDevSettings(const QString &name)
{
    QSettings set(name,QSettings::IniFormat);
    QString global("Global/");
    set.setValue(global+"DevType",QString());
    set.setValue(global+"AlarmGroup",QString());
    set.setValue(global+"MainType",QString());
    set.setValue(global+"TurnImage",false);
    set.setValue(global+"AlarmTimeSec",QString());
    set.setValue(global+"RecordTimeSec",QString());
    set.setValue(global+"EnableAlrm",false);
    QString attrinfo("Info/");
    set.setValue(attrinfo+"User",QString());
    set.setValue(attrinfo+"Tel",QString());
    set.setValue(attrinfo+"Addr",QString());
    set.setValue(attrinfo+"Commit",QString());
    set.setValue("RecVideo/RecType",0);
    QSettings::Status s = set.status();
    set.sync();
    s = set.status();
}


void SettingPanel::addCameraFromSql()
{
    QStringList nameidlist  = SqlInstance::getColumnsList("camera_settings",
        QString("camera_verifyid,camera_name").split(','));

    foreach(const QString &str,nameidlist)
    {

        if(m_TreeView->topLevelItemCount() > 0  &&
               m_TreeView->currentItem() )
            m_TreeView->addItem(str,!m_TreeView->currentItem()->type()?
                                m_TreeView->currentItem() : (QTreeWidgetItem*)(0));
        else
            m_TreeView->addItem(str);
    }
}


QStringList SettingPanel::getPlayList() const
{

    QStringList ipname = SqlInstance::getColumnList("camera_settings","camera_verifyid");
    return ipname;

}

void SettingPanel::slot_StartPlayer()
{

}

SettingPanel::~SettingPanel()
{

}
