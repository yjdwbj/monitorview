#ifndef SETTING_PANEL_H
#define SETTING_PANEL_H
#include "global.h"
#include "viewframe.h"
#include <QtWidgets/QListView>
#include <QStringListModel>

class CameraView : public QListView
{
    Q_OBJECT
public:
    explicit CameraView(const QStringList &list,QWidget *parent =0)
        :QListView(parent)
    {
        oldlist = list;
        QStringListModel *listModel= new QStringListModel(this);

        this->setViewMode(QListView::ListMode);

        QStringList tlist;
        foreach(const QString &str,list)
        {

            tlist << str.section(",",0,0);
        }
        listModel->setStringList(tlist);
        this->setModel(listModel);
    }
    QStringList getCameraList()const {return oldlist;}
    ~CameraView(){}
private:
    QStringList oldlist;
};


class SettingPanel :public QWidget
{
    Q_OBJECT
public:
    explicit SettingPanel(QWidget *parent = 0);

    ~SettingPanel();
//    void setViewFrame(ViewFrame *e) { vf =e;}
     inline QStringList getPlayList() const {return playlist;}
private slots:
    void slot_addnewCamera();
    void slot_searchCamera();
    void slot_StartPlayer();
private:
    QVBoxLayout *main_layout;
    QStringList playlist;
signals:
    void sig_gridofnumber(int);
    void StartPlayer();

};

#endif // SETTING_PANEL_H
