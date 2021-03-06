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
#include "camera_setting.h"
#include "control_widget.h"
#include "setting_panel.h"



class SettingPanel;

static QString actlist ("进入工作状态,退出工作状态,-,截图,报警,录像,停止报警,暂停错误报警,-,摄像机设置");





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



class Frame : public QWidget
{
    Q_OBJECT
public:
    explicit Frame(const QString &str="",QWidget *parent=0);
    ~Frame(){}
    QWidget *player;
    bool isPlaying()const {return m_playing;}
    void setPlaying(bool f) {m_playing = f; emit playStateChange(f);}
    void setCameraName(const QString &name ) { m_CameraName = name;}
    QString getCameraName() const{return m_CameraName;}

    void setCameraVerifyId(const QString &id) { m_CameraVerifyId = id;}
    QString getCameraVerifyId() const {return m_CameraVerifyId;}
signals:
    void clicked_Frame(Frame*);
    void mouse_move();
    void show_camera_settings();
    void playStateChange(bool);

protected:
//    void paintEvent(QPaintEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private slots:
    void slot_ProcessActions(QAction *p);
    void slot_timerout();
private:

    QString text;
    QTimer *ctrl_timer;
    ControlWidget *ctrl_widget;
    bool m_playing;
    QString m_CameraName;
    QString m_CameraVerifyId;
    bool isClicked;

};
class CameraView;
class WindowFrame : public  QWidget
{
    Q_OBJECT
public:
    explicit WindowFrame(const QString &str = "",QWidget *parent=0);
    ~WindowFrame(){ delete frame; this->destroyed(this);}
    WId getWindowId() const {return frame->winId();}
    void toggle_ctrlWidget_view(int);
    Frame *frame ;

private:

    QLabel *workstate;
    QList<QLabel*> m_lablist;
    QHBoxLayout *ctrl_layout;
    QSignalMapper *signalmap;
    QList<QPixmap> pixmaplist;
    CameraView *m_TreeView;
    QSettings *setfd;

    bool ctrlflag;



protected:
     void paintEvent(QPaintEvent *e);



//     bool eventFilter(QObject *obj, QEvent *e);

private slots:
     void slot_labelbtn_press(int);
     void slot_call_CameraSetting();
     void slot_call_ctlalarm_menu(QAction*);
     void slot_call_ctlrecord_menu(QAction*);
     void slot_workStateChangeed(bool);

public slots:
     void slot_updateCtlState();


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
     QList<WindowFrame*> getPlayFrame() const {return  m_list;}
     WId getPlayFrameWId(const QString &id);
     void updateItem(const QString &id, const QString &newName);
     QString getGridnumber() const { return m_GridNumber;}
     QList<WindowFrame*> m_list;

public slots:
    void swapFullScreenOrNormal(bool flag);

    void slot_deleteCamera(QString id);
    void slot_checkWorkState(QString n);


private slots:
    void slot_clicked_this(Frame *);
    void slot_timeout();
    void slot_BackToNormalWindown();


private:

    SettingPanel *setpanel;
    QSize screenSize;
    ToggleButton *toggleFS;
    FullScreen *FSWidget;
    QTimer *timer1;
    QGridLayout *lay;

    QString m_GridNumber;

};
#endif // VIEWFRAME_H
