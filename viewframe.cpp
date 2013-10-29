#include "viewframe.h"
#include <QApplication>
#include <QDesktopWidget>

ViewFrame::ViewFrame(QWidget *parent)
    :QGroupBox(parent),
      lay(new QGridLayout),
      FSWidget(new QWidget),
      toggleFS(new ToggleButton)

{
    toggleFS->setStyleSheet("background-image: url(:/lcy/images/:/lcy/images/backfullscreen.png);");
    QImage img;
    img.load(":/lcy/images/backfullscreen.png");
    toggleFS->setFixedSize(img.size());
    QDesktopWidget *dsk = qApp->desktop();
    screenSize.setWidth(dsk->x());
    screenSize.setHeight(dsk->y());



    toggleFS->setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
//    toggleFS->setWindowState(Qt::WindowNoState);
    QPoint pos;
    pos.setX(screenSize.width()- toggleFS->width());
    pos.setY(screenSize.height()-toggleFS->height());
    toggleFS->setGeometry(QRect(pos,toggleFS->size()));

    connect(toggleFS,SIGNAL(toggled()),SLOT(swapFullScreenOrNormal()));


//    signalMapper = new QSignalMapper(this);
    lay->setSpacing(1);
    lay->setMargin(0);
//    lay->setVerticalSpacing(10);


    setGridnumber(1,1);
    this->setFixedSize(540,400);
    this->setLayout(lay);

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
        MyFrame *f = new MyFrame("No Signals...");
        connect(f,SIGNAL(clicked_Frame(MyFrame*)),SLOT(slot_clicked_this(MyFrame*)));
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

void ViewFrame::slot_testsignals(int p)
{
    QMessageBox::warning(this,"test","test");
}

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
        MyFrame *f = new MyFrame("No Signals...");
        connect(f,SIGNAL(clicked_Frame(MyFrame*)),SLOT(slot_clicked_this(MyFrame*)));
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

void ViewFrame::swapFullScreenOrNormal()
{
    if(fullscreentoggle)
     {
        setFullScreen();

     }
     else
     {
         slot_BackToNormalWindown();
     }
    fullscreentoggle = !fullscreentoggle;
}

void ViewFrame::setFullScreen()
{

//    FSWidget= new QWidget;
    FSWidget->setLayout(lay);
    FSWidget->showFullScreen();
    FSWidget->setWindowFlags(Qt::WindowStaysOnTopHint| Qt::FramelessWindowHint);
    FSWidget->show();
    toggleFS->show();

//    timer1 = new QTimer;
//    connect(timer1,SIGNAL(timeout()),SLOT(slot_timeout()));
//    timer1->start(3000);

}


void ViewFrame::slot_BackToNormalWindown()
{

    toggleFS->hide();
    this->setLayout(lay);
    FSWidget->hide();
}

void ViewFrame::slot_timeout()
{
    this->setLayout(lay);
    FSWidget->hide();

}

void ViewFrame::slot_clicked_this(MyFrame *w)
{

    for(int i = 0; i < m_list.size();i++)
    {
        MyFrame *p = m_list.at(i);
        if(w == p)
        {
            w->setFrameShape(QFrame::WinPanel );
            w->setStyleSheet("background-color: #000000;");
            w->setFrameShadow(QFrame::Sunken);
        }
        else
        {
            p->setFrameShape(QFrame::Box);
            p->setStyleSheet("background-color: #555555;");
            p->setFrameShadow(QFrame::Plain);
        }
//         (w->rect().contains(pos.rx(),pos.ry()) ? QFrame::WinPanel : QFrame::Box);

    }
    this->parentWidget()->setFocus();

}


