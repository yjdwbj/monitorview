#include "control_widget.h"
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>



GuideButton::GuideButton(QWidget *parent)
    :QWidget(parent),
      smap(new QSignalMapper(this))
{
    connect(parent,SIGNAL(destroyed()),SLOT(deleteLater()));
    static QPixmap pixtop(QString(":/lcy/images/top.png"));
    static QPixmap pixright(QString(":/lcy/images/right.png"));
    static QPixmap pixleft(QString(":/lcy/images/left.png"));
    static QPixmap pixmid(QString(":/lcy/images/mid.png"));
    static QPixmap pixbuttom(QString(":/lcy/images/buttom.png"));



    setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    QPushButton *btn_top = new QPushButton(QIcon(pixtop),"");

    QPushButton *btn_right = new QPushButton(QIcon(pixright),"");

    QPushButton *btn_left = new QPushButton(QIcon(pixleft),"");

    QPushButton *btn_buttom = new QPushButton(QIcon(pixbuttom),"");

    QPushButton *btn_mid = new QPushButton(QIcon(pixmid),"");

    QGridLayout *main_layout = new QGridLayout;
    main_layout->setSpacing(1);
    main_layout->setMargin(1);
    main_layout->addWidget(btn_top,0,1);
    main_layout->addWidget(btn_left,1,0);
    main_layout->addWidget(btn_mid,1,1);
    main_layout->addWidget(btn_right,1,2);
    main_layout->addWidget(btn_buttom,2,1);

     QList<QPixmap> pixmaplist;
     pixmaplist << pixtop << pixleft << pixmid << pixright << pixbuttom;

    QList<QPushButton*> btnlist;
    btnlist << btn_top  << btn_left << btn_mid << btn_right << btn_buttom;
    int i =0;
    foreach(QPushButton *b,btnlist)
    {

        b->setFixedSize(pixmaplist.at(i).size());
        b->setIconSize(b->size());
        b->setMask(pixmaplist.at(i).mask());
        smap->setMapping(b,i++);

        connect(b,SIGNAL(clicked()),smap,SLOT(map()));
    }
    connect(smap,SIGNAL(mapped(int)),SIGNAL(signals_to_id(int)));
    QPixmap mask(tr(":/lcy/images/guide.png"));
    this->setFixedSize(mask.size());

    setLayout(main_layout);




}

void GuideButton::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.drawPixmap(0,0,tr(":/lcy/images/guide.png"));

}


ControlWidget::ControlWidget(QWidget *parent)
    :QWidget(parent),
      main_layout(new QGridLayout),
      timer(new QTimer)
{
    connect(parent,SIGNAL(destroyed()),SLOT(deleteLater()));
    GuideButton *btn_test = new GuideButton;

    connect(timer,SIGNAL(timeout()),SLOT(slot_TimerOut()));
    setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    main_layout->addWidget(btn_test,0,0);
    main_layout->setMargin(1);
    QPixmap mask(tr(":/lcy/images/guide.png"));
    this->setFixedSize(mask.size());
    this->setMask(mask.mask());
    setLayout(main_layout);
}

ControlWidget::~ControlWidget()
{
    this->deleteLater();
}

void ControlWidget::slot_TimerOut()
{
    hide();
}
