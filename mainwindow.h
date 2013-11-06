#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "global.h"
#include "setting_panel.h"
#include "viewframe.h"
#include <vlc/vlc.h>
#include <QProcess>


typedef struct {
    libvlc_instance_t *_vlcInstance;
    libvlc_media_player_t *_vlcMediaPlayer;
}
vlcItem;

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
    void slot_StartPlay();
    void slot_StopPlay();
private:

    ViewFrame *view;
    QHBoxLayout *main_layout;
    SettingPanel *panel;
    QList<QProcess*> plist;

    void ReadQss();

//    libvlc_instance_t *_vlcInstance;
//    libvlc_media_player_t *_vlcMediaPlayer;



//    libvlc_event_manager_t *_vlcEvents;

//    libvlc_media_t *_vlcMedia;
    QList<vlcItem> vlcItemList;

    void vlcPlayRtsp();
};

#endif // MAINWINDOW_H
