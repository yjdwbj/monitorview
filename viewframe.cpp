#include "viewframe.h"


Frame::Frame(const QString &str,QWidget *parent)
    :QFrame(parent),
      ctrl_widget(new ControlWidget)
{
    text = str;
    setFrameShape(QFrame::StyledPanel);
    setStyleSheet("background-color: #555555;");
    setFrameShadow(QFrame::Plain);
}

void Frame::paintEvent(QPaintEvent *e)
{

    QPainter p(this);
    int x = this->height()/2;
    int y = this->width() /2-30;
    p.drawText(y,x,text);
    e->accept();

}

void Frame::mouseDoubleClickEvent(QMouseEvent *e)
{
     e->accept();
    if( (e->type() == QMouseEvent::MouseButtonDblClick)
            && (e->button() == Qt::LeftButton ))
    {

        emit clicked_Frame(this);
    }

}

void Frame::mousePressEvent(QMouseEvent *e)
{


    if((e->type() == QMouseEvent::MouseButtonPress))
    {
        if((e->button() == Qt::RightButton))
        {

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
        }
        else if((e->button() == Qt::LeftButton))
        {
            ctrl_widget->startTimer();
            QPoint m = this->mapToGlobal(e->pos());
            ctrl_widget->move(m.rx(),m.ry());
        }
    }

}


void Frame::slot_ProcessActions(QAction *p)
{
    if(!p->text().compare("摄像机设置"))
    {
        CameraSetting *cs = new CameraSetting("test");
        cs->exec();
    }

}



WindowFrame::WindowFrame(const QString &str,QWidget *parent)
    :QWidget(parent),
      lab_frameRate(new QLabel),
      lab_speedRate(new QLabel),
      btn_warn(new QPushButton),
      btn_capture(new QPushButton),
      btn_snapshot(new QPushButton),
      btn_setting(new QPushButton),
      frame (new Frame(str))
{
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->setMargin(0);
    main_layout->setSpacing(1);


    QHBoxLayout *ctrl_layout = new QHBoxLayout;
    ctrl_layout->setSpacing(1);
    ctrl_layout->setMargin(0);
    ctrl_layout->addWidget(lab_frameRate);
    lab_frameRate->setPixmap(QPixmap(":/lcy/images/runorstop.png").copy(0,16,16,16));
    ctrl_layout->addWidget(lab_speedRate);

    ctrl_layout->addStretch();
    ctrl_layout->addWidget(btn_warn);
    ctrl_layout->addWidget(btn_capture);
    btn_capture->setIcon(QPixmap(":/lcy/images/record-state.png").copy(0,0,16,16));
//    btn_capture->setMask(QPixmap(":/lcy/images/record-state.png").copy(0,0,16,16).mask());
    ctrl_layout->addWidget(btn_snapshot);
    ctrl_layout->addWidget(btn_setting);
    main_layout->addWidget(frame);
    main_layout->addLayout(ctrl_layout);
    setLayout(main_layout);

}


ViewFrame::ViewFrame(QWidget *parent)
    :QGroupBox(parent),
      lay(new QGridLayout)
//      FSWidget(new QWidget),
//      toggleFS(new ToggleButton)

{



//    signalMapper = new QSignalMapper(this);
    lay->setSpacing(1);
    lay->setMargin(0);
//    lay->setVerticalSpacing(10);


    setGridnumber(1,1);
    this->setFixedSize(540,400);
    this->setLayout(lay);

}


void ViewFrame::StartPlayer()
{

}

//void ViewFrame::mouseDoubleClickEvent(QMouseEvent *e)
//{
//    if( (e->type() == QMouseEvent::MouseButtonDblClick)
//            && (e->button() == Qt::LeftButton ))
//    {
////            setFrameShadow(QFrame::Sunken);
////            this->setFocus();
////        emit clickedtopos(e->pos());
//    }
//    e->accept();
//}

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
        }

    }

    while(m_list.size() < sum)
    {
        WindowFrame *f = new WindowFrame("No Signals...");
        connect(f->frame,SIGNAL(clicked_Frame(WindowFrame*)),SLOT(slot_clicked_this(WindowFrame*)));

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
        }

    }

    while(m_list.size() < 8)
    {
        WindowFrame *f = new WindowFrame("No Signals...");
        connect(f->frame,SIGNAL(clicked_Frame(Frame*)),SLOT(slot_clicked_this(Frame*)));
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


