#include "viewframe.h"
#include "camera_settings.h"
#include "sqldriver.h"

static QString tips("报警通知|启动/停止录像|抓图|设置");



Frame::Frame(const QString &str,QWidget *parent)
    :QWidget(parent),
      player(new QWidget(this)),
      m_playing(false),
      isClicked(false),
      ctrl_timer(new QTimer),
      m_CameraVerifyId(""),
      m_CameraName("")
{
    connect(ctrl_timer,SIGNAL(timeout()),SLOT(slot_timerout()));
    player->setAttribute(Qt::WA_NativeWindow);
    QVBoxLayout *l = new QVBoxLayout();
    l->setMargin(0);
    l->addWidget(player);
    setLayout(l);
    setStyleSheet("background-color : #666666;");
}



void Frame::mouseDoubleClickEvent(QMouseEvent *e)
{

    if( (e->type() == QMouseEvent::MouseButtonDblClick)
            && (e->button() == Qt::LeftButton ))
    {

        emit clicked_Frame(this);
        e->accept();
    }

}

void Frame::mouseReleaseEvent(QMouseEvent *e)
{

    if(!isPlaying())
        return;

        if((e->button() == Qt::RightButton))
        {
            e->accept();
            QMenu *menu = new QMenu();
            QList<QAction*> list;
            foreach(const QString &str,actlist.split(","))
            {
                if(str.startsWith("-"))
                {
                    list.append(menu->addSeparator());
                }
                else
                    list << new QAction(str,menu);
            }

            connect(menu,SIGNAL(triggered(QAction*)),SLOT(slot_ProcessActions(QAction*)));
            menu->addActions(list);

            menu->exec(this->mapToGlobal( e->pos()));
            emit clicked_Frame(this);
        }
        else if((e->button() == Qt::LeftButton))
        {


            if(!isClicked)
            {
                isClicked = true;
                ctrl_widget = new ControlWidget();
                QPoint m = this->mapToGlobal(e->pos());
                ctrl_widget->move(m.rx(),m.ry());
                ctrl_timer->start(5000);
            }
            else
            {
                QPoint m = this->mapToGlobal(e->pos());
                ctrl_widget->move(m.rx(),m.ry());
            }
        }
}


void Frame::slot_timerout()
{
    delete ctrl_widget;
    ctrl_widget = 0;
    isClicked = false;
}


void Frame::slot_ProcessActions(QAction *p)
{
    if(!p->text().compare("摄像机设置"))
    {
        emit show_camera_settings();
    }

}



WindowFrame::WindowFrame(const QString &str,QWidget *parent)
    :QWidget(parent),
      workstate(new QLabel),
      frame(new Frame(str)),
      signalmap(new QSignalMapper(this)),
      ctrl_layout(new QHBoxLayout),
      ctrlflag(false)
{
    pixmaplist   << QPixmap(":/lcy/images/waring-16x16.png")
              << QPixmap(":/lcy/images/record-state-16x16.png")
              << QPixmap(":/lcy/images/snapshot-16x16.png")
              << QPixmap(":/lcy/images/settings16x16.png");


    connect(frame,SIGNAL(playStateChange(bool)),SLOT(slot_workStateChangeed(bool)));




    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->setMargin(0);
    main_layout->setSpacing(0);

    ctrl_layout->setSpacing(2);
    ctrl_layout->setMargin(0);
    ctrl_layout->addWidget(workstate);
    workstate->setPixmap(QPixmap(":/lcy/images/state-16x16.png").copy(0,0,16,16));
     ctrl_layout->addStretch();



    int pixnum =pixmaplist.size();

    for(int i =0 ; i < pixnum;i++)
    {
        QPixmap p = pixmaplist.at(i).copy(0,0,16,16);
        LabelBtn *btn = new LabelBtn(p,tips.section("|",i,i));
        m_lablist.append(btn);
        ctrl_layout->addWidget(btn);
        signalmap->setMapping(btn,i);
        connect(btn,SIGNAL(mouse_pressed()),signalmap,SLOT(map()));
    }
    connect(signalmap,SIGNAL(mapped(int)),SLOT(slot_labelbtn_press(int)));
    connect(frame,SIGNAL(show_camera_settings()),SLOT(slot_call_CameraSetting()));
    main_layout->addWidget(frame);
    main_layout->addLayout(ctrl_layout);
    setLayout(main_layout);
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    toggle_ctrlWidget_view(0);

    setfd = new QSettings(qApp->applicationDirPath()+"/devs/"+frame->getCameraName(),
          QSettings::IniFormat);
}

void WindowFrame::slot_workStateChangeed(bool f)
{
    workstate->setPixmap(QPixmap(":/lcy/images/state-16x16.png").copy(0,f ? 16 : 0,16,16));
}




void WindowFrame::toggle_ctrlWidget_view(int flag)
{
    ctrlflag = flag;
    int n = ctrl_layout->count();
    for(int i = 0 ; i < n;i++)
    {
        QWidget *w = ctrl_layout->itemAt(i)->widget();
        if(w)
            flag ? w->show() : w->hide();
    }

}

void WindowFrame::slot_call_ctlalarm_menu(QAction *act)
{
   if(!act->text().compare("启用报警"))
    {
        setfd->setValue(EnableAlarm,true);
         setfd->sync();
          QString tsec = setfd->value(AlarmTimeSec).toString();
           m_lablist.at(0)->setPixmap(pixmaplist.at(0).copy(0,16,16,16));
          if(tsec.isEmpty())
          {
              int n = QMessageBox::question(this,"提示","启用了报警,但是还没有设置报警,是否立即设置报警?");
              if(n==QMessageBox::Yes)
              {
                  goto SET;
              }
          }


    }
    else if(!act->text().compare("不启用报警"))
    {
        setfd->setValue(EnableAlarm,false);
         setfd->sync();
        m_lablist.at(0)->setPixmap(pixmaplist.at(0).copy(0,0,16,16));
    }

    if(!act->text().compare("设置"))
    {
SET:
        camera_settings *cs = new camera_settings(2,frame->getCameraName());
        cs->exec();
        delete cs;
    }
}


void WindowFrame::slot_call_ctlrecord_menu(QAction *act)
{

    setfd = new QSettings(qApp->applicationDirPath()+"/devs/"+frame->getCameraName(),
          QSettings::IniFormat);
    if(!act->text().compare("不启用录像"))
    {
        m_lablist.at(1)->setPixmap(pixmaplist.at(1).copy(0,0,16,16));
        setfd->setValue(RecordType,0);
    }
    else if(!act->text().compare("启用计划录像"))
    {
        setfd->setValue(RecordType,2);
        setfd->sync();
        QString tsec = setfd->value(RecordTimeSec).toString();
         m_lablist.at(1)->setPixmap(pixmaplist.at(1).copy(0,16,16,16));
        if(tsec.isEmpty())
        {
            int n = QMessageBox::question(this,"提示","启用的按计划录像,但是还没有设置录像计划,是否立即设置录像计划?");
            if(n==QMessageBox::Yes)
            {
                goto SET;
            }
        }
    }
    else if(!act->text().compare("一直录像"))
    {
        setfd->setValue(RecordType,1);
        m_lablist.at(1)->setPixmap(pixmaplist.at(1).copy(0,32,16,16));
    }

    setfd->sync();

    if(!act->text().compare("设置"))
    {
SET:
        camera_settings *cs = new camera_settings(3,frame->getCameraName());
        cs->exec();
        delete cs;
    }
}

void WindowFrame::slot_updateCtlState()
{
    setfd = new QSettings(qApp->applicationDirPath()+"/devs/"+frame->getCameraName(),
          QSettings::IniFormat);
    bool f = setfd->value(EnableAlarm).toBool();  // update alarm icon.
     m_lablist.at(0)->setPixmap(pixmaplist.at(0).copy(0,f ? 16 : 0,16,16));

    int n  = setfd->value(RecordType).toInt();

    if(n < 1)  //  不启用录像
    {
        m_lablist.at(1)->setPixmap(pixmaplist.at(1).copy(0,0,16,16));
    }
    else if ( n == 1) // 一直录像
    {
        m_lablist.at(1)->setPixmap(pixmaplist.at(1).copy(0,32,16,16));
    }
    else
    {
        m_lablist.at(1)->setPixmap(pixmaplist.at(1).copy(0,16,16,16));
    }
}



void WindowFrame::slot_labelbtn_press(int id)
{
    QAction *setting = new QAction(QString("设置"),this);
    switch(id)
    {
    case 0:
    {
        QMenu *pmenu = new QMenu();  // alarm settings
        QAction *offalarm = new QAction(QIcon(pixmaplist.at(id).copy(0,0,16,16)),tr("不启用报警"),this);
        QAction *onalarm = new QAction(QIcon(pixmaplist.at(id).copy(0,16,16,16)),tr("启用报警"),this);

        pmenu->addAction(offalarm);
        pmenu->addAction(onalarm);
        pmenu->addAction(setting);
        pmenu->popup(this->cursor().pos());
        connect(pmenu,SIGNAL(triggered(QAction*)),SLOT(slot_call_ctlalarm_menu(QAction*)));
    }
        break;
    case 1:
    {
        QMenu *pmenu = new QMenu();
        QAction *offrecord =  new QAction(QIcon(pixmaplist.at(id).copy(0,0,16,16)),
                                       QString("不启用录像"),this);
        QAction *onplan = new QAction(QIcon(pixmaplist.at(id).copy(0,16,16,16)),
                                      QString("启用计划录像"),this);
        QAction *always = new QAction(QIcon(pixmaplist.at(id).copy(0,32,16,16)),
                                      QString("一直录像"),this);
        pmenu->addAction(offrecord);
        pmenu->addAction(onplan);
        pmenu->addAction(always);
        pmenu->addAction(setting);
        pmenu->popup(this->cursor().pos());
        connect(pmenu,SIGNAL(triggered(QAction*)),SLOT(slot_call_ctlrecord_menu(QAction*)));
    }
        break;
    case 2:
        break;
    case 3:
        slot_call_CameraSetting();
        break;
    }


}



void WindowFrame::slot_call_CameraSetting()
{
    camera_settings *cs = new camera_settings(0,frame->getCameraName());
    if(cs->exec())
    {
//        frame->setCameraName(cs->getCameraName());
//        m_TreeView = QApplication::activeWindow()->findChild<CameraView*>("ViewCamera");
//        if(m_TreeView)
//        {
//            m_TreeView->updateItem(frame->getCameraVerifyId(),frame->getCameraName());
//        }
    }
    delete cs;
}


void WindowFrame::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.setBrush(QBrush(QColor(66,66,66)));
    paint.drawRect(this->rect());
}


ViewFrame::ViewFrame(QWidget *parent)
    :QGroupBox(parent),
      lay(new QGridLayout),
      m_GridNumber("1x1")
//      FSWidget(new QWidget),
//      toggleFS(new ToggleButton)

{
    setObjectName("ViewFrame");
    lay->setSpacing(1);
    lay->setMargin(0);
//    lay->setVerticalSpacing(10);


    setGridnumber(1,1);
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
//    this->resize(QSize(540,540));
    this->setLayout(lay);
//    setpanel = qApp->activeWindow()->findChild<SettingPanel*>("SettingPanel");
//    connect(setpanel,SIGNAL(updateItemValue(QString)),SLOT(slot_checkWorkState(QString)));
}


void ViewFrame::slot_checkWorkState(QString str)
{
    foreach( WindowFrame *f, m_list )
    {
        if(!str.compare(f->frame->getCameraVerifyId()))
        {
            f->slot_updateCtlState();
        }
    }
}

void ViewFrame::setGridnumber(int row, int col)
{
    int sum = row *col;
    if(sum == m_list.count())
        return;

    if(m_list.size()!=0)
    {
        for(int i= 0 ; i < m_list.size();i++)
        {
            lay->removeWidget(m_list.at(i));
            m_list.at(i)->destroyed();
        }

    }

    while(m_list.size() < sum)
    {
        WindowFrame *f = new WindowFrame("No Signals...");
//        connect(f->frame,SIGNAL(clicked_Frame(Frame*)),SLOT(slot_clicked_this(Frame*)));

        m_list.append(f);

    }

    while(m_list.size() > sum)
    {
        delete m_list.takeLast();
    }


    int n = 0;
    for(int x = 0; x < row;x++)
    {

        for(int y = 0; y < col;y++)
        {
            lay->addWidget(m_list.at(n++),x,y);
        }

    }

    m_GridNumber = QString::number(row)+"x"+QString::number(col);

}

//void ViewFrame::slot_testsignals(int p)
//{
//    QMessageBox::warning(this,"test","test");
//}

void ViewFrame::setOnePlusSeven()
{
    if(m_list.size()!=0)
    {
        for(int i= 0 ; i < m_list.size();i++)
        {
            lay->removeWidget(m_list.at(i));
            m_list.at(i)->destroyed();
        }

    }

    while(m_list.size() < 8)
    {
        WindowFrame *f = new WindowFrame("No Signals...");
//        connect(f->frame,SIGNAL(clicked_Frame(Frame*)),SLOT(slot_clicked_this(Frame*)));
        m_list.append(f);
    }

    while(m_list.size() > 8)
    {
        delete m_list.takeLast();
    }


    lay->addWidget(m_list.at(0),0,0,3,3);
    lay->addWidget(m_list.at(1),0,3);
    lay->addWidget(m_list.at(2),1,3);
    lay->addWidget(m_list.at(3),2,3);
    lay->addWidget(m_list.at(4),3,0);
    lay->addWidget(m_list.at(5),3,1);
    lay->addWidget(m_list.at(6),3,2);
    lay->addWidget(m_list.at(7),3,3);
    m_GridNumber = QString::number(1)+"x"+QString::number(7);

}

void ViewFrame::swapFullScreenOrNormal(bool flag)
{
    if(flag)
     {
        setFullScreen();

     }
     else
     {
         slot_BackToNormalWindown();
     }
//    fullscreentoggle = !fullscreentoggle;
}

void ViewFrame::setFullScreen()
{

    FSWidget= new FullScreen(lay);

    connect(FSWidget,SIGNAL(GoBackToNormal(bool)),SLOT(swapFullScreenOrNormal(bool)));
    toggleFS = new ToggleButton;
    connect(toggleFS,SIGNAL(toggled(bool)),SLOT(swapFullScreenOrNormal(bool)));

    FSWidget->setTogglebtn(toggleFS);

//    timer1 = new QTimer;
//    connect(timer1,SIGNAL(timeout()),SLOT(slot_timeout()));
//    timer1->start(3000);

}


void ViewFrame::updateItem(const QString &id,const QString &newName)
{
    foreach(WindowFrame *w,m_list) // change camera name by id;
    {
        if(w->frame->getCameraVerifyId().compare(id))
            continue;
        w->frame->setCameraName(newName);
        break;
    }
}



void ViewFrame::slot_deleteCamera(QString id)
{

     foreach(WindowFrame *w,m_list)
     {
         if(w->frame->getCameraVerifyId().compare(id) )
             continue;
         w->toggle_ctrlWidget_view(0);
         break;
     }

}


void ViewFrame::slot_BackToNormalWindown()
{

    toggleFS->hide();
    this->setLayout(lay);
    FSWidget->hide();
    delete FSWidget;
    delete toggleFS;
    this->parentWidget()->setFocus();
}

void ViewFrame::slot_timeout()
{
    this->setLayout(lay);
    FSWidget->hide();
}

WId ViewFrame::getPlayFrameWId(const QString &id)
{
    foreach(const WindowFrame *w, m_list)
    {
        if(w->frame->getCameraVerifyId().compare(id))
            continue;
        return w->frame->winId();
    }
    return WId(0);
}


void ViewFrame::slot_clicked_this(Frame *w)
{

//    for(int i = 0; i < m_list.size();i++)
//    {
//        WindowFrame *p = m_list.at(i);
//        if(w == p)
//        {
//            w->frame->setFrameShape(QFrame::WinPanel );
//            w->frame->setStyleSheet("background-color: #000000;");
//            w->frame->setFrameShadow(QFrame::Sunken);
//        }
//        else
//        {
//            p->frame->setFrameShape(QFrame::Box);
//            p->frame->setStyleSheet("background-color: #555555;");
//            p->frame->setFrameShadow(QFrame::Plain);
//        }
////         (w->rect().contains(pos.rx(),pos.ry()) ? QFrame::WinPanel : QFrame::Box);

//    }
    this->parentWidget()->setFocus();

}


