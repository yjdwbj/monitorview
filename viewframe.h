#ifndef VIEWFRAME_H
#define VIEWFRAME_H
#include "global.h"
#include <QMouseEvent>
#include <QRect>
#include <QPainter>
#include <QBrush>
#include <QRgb>

class ToggleButton : public QWidget
{
    Q_OBJECT
public:
    explicit  ToggleButton(QWidget *parent =0)
        :QWidget(parent)
    {

    }

signals:
    void toggled();

protected:
    void mousePressEvent(QMouseEvent *e)
    {
        if(e->button() == Qt::LeftButton &&
                e->type() == QMouseEvent::MouseButtonPress)
        {


            emit toggled();

        }
    }

    void keyPressEvent(QKeyEvent *e)
    {
        if(e->text() == "f")
        {

            emit toggled();

        }

    }

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
    void clicked_noArg();

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

    void mousePressEvent(QMouseEvent *e)
    {
        e->accept();
       if( (e->type() == QMouseEvent::MouseButtonPress)
               && (e->button() == Qt::LeftButton ))
       {
           emit clicked_noArg();
       }
    }

private:
    QString text;

};

class ViewFrame : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewFrame(QWidget *parent =0);

    ~ViewFrame(){
        delete this;
        delete toggleFS;
        delete FSWidget;
//        deleteLater();
    }
    void setGridnumber(int row,int col);


    void setOnePlusSeven();
    void setFullScreen();
public slots:
    void swapFullScreenOrNormal();

private slots:
    void slot_clicked_this(MyFrame*);
    void slot_testsignals(int);
    void slot_timeout();
    void slot_BackToNormalWindown();


private:


    QSize screenSize;
    ToggleButton *toggleFS;
    QWidget *FSWidget;
    QTimer *timer1;
    QGridLayout *lay;
    QList<MyFrame*> m_list;

};
#endif // VIEWFRAME_H
