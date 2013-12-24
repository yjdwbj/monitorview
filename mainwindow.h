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
//    QString camera_name;
    int order;
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
//    bool nativeEvent(const QByteArray &eventType, void *message, long *result);


private slots:
    void slot_GridNumberChanged(int);
//    void slot_GetViewFrameClicked(QRect);
    void slot_StartPlay();
    void slot_StopPlay();
    void slot_viewCtrolWidget(int);
    void slot_playById(int);
    void slot_stopById(int);

private:

    ViewFrame *view;
    QHBoxLayout *main_layout;
    SettingPanel *panel;
    QList<QProcess*> plist;

    void updateViewFrame();
    bool eventFilter(QObject *, QEvent *);
    void closeEvent(QCloseEvent *);

    void ReadQss();
    void readLayout();

    QList<vlcItem> vlcItemList;

    void vlcPlayRtsp();
};

#endif // MAINWINDOW_H
