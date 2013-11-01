#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "global.h"
#include "setting_panel.h"
#include "viewframe.h"
#include <vlc/vlc.h>
#include <QProcess>





class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *e);


private slots:
    void slot_GridNumberChanged(int);
//    void slot_GetViewFrameClicked(QRect);
    void slot_StartPlayer();
private:

    ViewFrame *view;
    QHBoxLayout *main_layout;
    SettingPanel *panel;
    QList<QProcess*> plist;


    libvlc_instance_t *_vlcinstance;
    libvlc_media_player_t *_mp;
    libvlc_media_t *_m;
};

#endif // MAINWINDOW_H
