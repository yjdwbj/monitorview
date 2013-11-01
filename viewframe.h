#ifndef VIEWFRAME_H
#define VIEWFRAME_H
#include "global.h"
#include <QMouseEvent>
#include <QRect>
#include <QPainter>
#include <QBrush>
#include <QRgb>
#include <QPicture>
#include <QDesktopWidget>
#include <qt_windows.h>





class ToggleButton : public QWidget
{
    Q_OBJECT
public:
    explicit  ToggleButton(QWidget *parent =0)
        :QWidget(parent),
          timer1(new QTimer),
          imgpath(":/lcy/images/backfullscreen.png"),
          timenum(8000)
    {
        connect(timer1,SIGNAL(timeout()),SLOT(slot_timeout()));

        QImage img;
        img.load(imgpath);
        setFixedSize(img.size());
        QRect deskRect = QApplication::desktop()->availableGeometry();
        move(deskRect.width()-width(),deskRect.height()-height());
        setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

        timer1->start(timenum);
        show();

    }
    ~ToggleButton()
    {
        setAttribute(Qt::WA_DeleteOnClose);
    }
    void showMe()
    {
        show();
        timer1->start(timenum);

    }

signals:
    void toggled(bool);
private slots:
    void slot_timeout()
    {
        if(!underMouse())
        {
           this->hide();
           timer1->stop();
        }

    }

protected:
//    bool nativeEvent(const QByteArray &eventType, void *message, long *result)
//    {
//        MSG *msg = reinterpret_cast<MSG*>(message);
//        int msgType = msg->message;
//        if(msgType == WM_MOUSEMOVE)
//        {
//            show();
//        }
//        return false;

//    }

    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.drawPixmap(this->rect(),QPixmap(imgpath));
    }

    void mousePressEvent(QMouseEvent *e)
    {
        if(e->button() == Qt::LeftButton &&
                e->type() == QMouseEvent::MouseButtonPress)
        {

            emit toggled(false);

        }
    }

    void mouseMoveEvent(QMouseEvent *)
    {

            show();

    }

    void keyPressEvent(QKeyEvent *e)
    {
        if(e->text() == "f")
        {

            emit toggled(false);

        }

    }

private:
    QTimer *timer1;
    QString imgpath;
    int timenum;

};

class FullScreen : public QWidget
{
    Q_OBJECT
public:
    explicit FullScreen(QGridLayout *lay, QWidget *parent =0 )
        :QWidget(parent)
    {
       setLayout(lay);
       showFullScreen();
       setWindowFlags(Qt::FramelessWindowHint);
       show();
    }
    ~FullScreen(){setAttribute(Qt::WA_DeleteOnClose);}
    void setTogglebtn(ToggleButton *btn) {t = btn;}
signals:
    void GoBackToNormal(bool);
protected:
    void keyPressEvent(QKeyEvent *e)
    {
        if(e->text() == "f")
           emit GoBackToNormal(false);

    }

    void mouseMoveEvent(QMouseEvent *e)
    {
        e->accept();
        if(t)
            t->showMe();
    }

private:
    ToggleButton *t;
};



class MyFrame : public QFrame
{
    Q_OBJECT
public:
    explicit MyFrame(const QString &str="",QWidget *parent=0)
        :QFrame(parent)
    {
        text = str;
        setFrameShape(QFrame::StyledPanel);
        setStyleSheet("background-color: #555555;");
        setFrameShadow(QFrame::Plain);
    }
    ~MyFrame(){
//        delete this;
    }
signals:
    void clicked_Frame(MyFrame*);
    void mouse_move();

protected:
    void paintEvent(QPaintEvent *e)
    {

        QPainter p(this);
        int x = this->height()/2;
        int y = this->width() /2-30;
        p.drawText(y,x,text);
        e->accept();

    }

    void mouseDoubleClickEvent(QMouseEvent *e)
    {
         e->accept();
        if( (e->type() == QMouseEvent::MouseButtonDblClick)
                && (e->button() == Qt::LeftButton ))
        {

            emit clicked_Frame(this);
        }

    }

//    void mouseMoveEvent(QMouseEvent *e)
//    {
//        if(e->type == QMouseEvent::MouseMove)
//        {
//            emit mouse_move();
//        }
//    }

private:
    QString text;

};

class ViewFrame : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewFrame(QWidget *parent =0);

    ~ViewFrame(){
//        delete this;
//        delete toggleFS;
//        delete FSWidget;
//        deleteLater();
    }
    void setGridnumber(int row,int col);
    void setOnePlusSeven();
    void setFullScreen();
    void StartPlayer();
     QList<MyFrame*> getPlayFrame() const {return  m_list;}
public slots:
    void swapFullScreenOrNormal(bool flag);

private slots:
    void slot_clicked_this(MyFrame*);
//    void slot_testsignals(int);
    void slot_timeout();
    void slot_BackToNormalWindown();


private:


    QSize screenSize;
    ToggleButton *toggleFS;
    FullScreen *FSWidget;
    QTimer *timer1;
    QGridLayout *lay;
    QList<MyFrame*> m_list;

};
#endif // VIEWFRAME_H
