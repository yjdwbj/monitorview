#ifndef SETTING_PANEL_H
#define SETTING_PANEL_H
#include "global.h"
#include "viewframe.h"
#include <QtWidgets/QTreeWidget>
#include <QStringListModel>

static QString CameraMenu ("进入工作状态|退出工作状态|停止报警|暂停错误报警|摄像机设置|-|删除摄像机|新增组|新增摄像机|自动查找及新增摄像机");
static QString GroupMenu("连接|断开连接|-|新增摄像机|自动查找及新增摄像机|-|新增组|修改组|删除组");
static QString MenuAction("进入工作状态|退出工作状态|停止报警|暂停错误报警|摄像机设置|删除摄像机|删除组|新增组|新增摄像机|自动查找及新增摄像机|修改组|连接|断开连接");


class GroupDialog : public QDialog
{
    Q_OBJECT
public:
    QLineEdit *edit ;
    GroupDialog(QWidget *parent =0)
        :QDialog(parent),
        edit(new QLineEdit())
    {
        QVBoxLayout *lay  = new QVBoxLayout;

        QLabel *lab = new QLabel("新建组名：");
        GroupBtnWidget *g = new GroupBtnWidget(yesorno.split(","),GroupBtnWidget::Horizontal);
        connect(g,SIGNAL(SignalById(int)),SLOT(slot_signals(int)));
        lay->addWidget(lab);
        lay->addWidget(edit);
        lay->addWidget(g);
        setLayout(lay);
    }
private slots:
    void slot_signals(int id )
    {
        id ? reject():accept();
    }

};


class CameraView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit CameraView(QWidget *parent =0);
    void addItem(const QStringList &list,bool isgroup,QTreeWidgetItem *root=0);
    QStringList getCameraList()const {return oldlist;}
    ~CameraView(){}


private slots:
    void slot_itemclicked(QTreeWidgetItem *,int);

private:
    QStringList oldlist;
    QIcon CameraIcon;
    QIcon GroupIcon;
signals:
    void this_is_empty();
    void searchCameraAndAdded();
    void triggerViewMenu(QAction*);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};


class SettingPanel :public QWidget
{
    Q_OBJECT
public:
    explicit SettingPanel(QWidget *parent = 0);

    ~SettingPanel();
//    void setViewFrame(ViewFrame *e) { vf =e;}
      QStringList getPlayList() const ;
private slots:
    void slot_addnewCamera();
    void slot_searchCamera();
    void slot_addCameraGroup();
    void slot_StartPlayer();
    void slot_Process_Ctrl_signals(int);
    void slot_ProcessMenuAction(QAction*);
private:
    QVBoxLayout *main_layout;
    QStringList playlist;
    CameraView *m_TreeView;
    QGroupBox *gbox_addnew;
signals:
    void sig_gridofnumber(int);
    void StartPlay();
    void StopPlay();
    void addedNewCamera(int);


};

#endif // SETTING_PANEL_H
