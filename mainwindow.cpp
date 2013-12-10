#include "mainwindow.h"
#include <QThread>
#include "sqldriver.h"





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    main_layout(new QHBoxLayout),
    view(new ViewFrame),
    panel(new SettingPanel)
{

//    ReadQss();

    main_layout = new QHBoxLayout;
    connect(panel,SIGNAL(sig_gridofnumber(int)),SLOT(slot_GridNumberChanged(int)));
    connect(panel,SIGNAL(StartPlay()),SLOT(slot_StartPlay()));
    connect(panel,SIGNAL(StopPlay()),SLOT(slot_StopPlay()));
//    connect(panel,SIGNAL(addedNewCamera(int)),SLOT(slot_viewCtrolWidget(int)));
    main_layout->addWidget(view);
    main_layout->addWidget(panel);



    QWidget *w = new QWidget;
    w->setLayout(main_layout);
    resize(QSize(800,400));

    setCentralWidget(w);

}


void MainWindow::slot_viewCtrolWidget(int num)
{

     QList<WindowFrame*> playframe = view->getPlayFrame();

     num = playframe.count() < num ? playframe.count() : num;
     for(int i = 0 ; i < num ; i++)
     {
         playframe[i]->toggle_ctrlWidget_view(1);
     }
}

void MainWindow::vlcPlayRtsp() // start play from rtsp service.
{
    QList<WindowFrame*> playframe = view->getPlayFrame();
    QStringList playlist = panel->getPlayList();
    int num = playframe.count() < playlist.count() ? playframe.count() : playlist.count();
    if(vlcItemList.count() == num)
        return ;
    for(int i = vlcItemList.count() ; i < num ; i++)
    {
        QString captureOption(":sout=#stream_out_duplicate{dst=display,dst=std{access=file,mux,ts,dst=");
        captureOption+=tr("D:/camera-video/")+tr("camera")+QString::number(i)+tr(".mpg}}");
        QString url = tr("rtsp://192.168.8.31/")+playlist.at(i);
        QString fname =  url + tr(".mpg");
        libvlc_instance_t *_vlc_inst = libvlc_new(0,NULL);
        libvlc_media_t *_vlc_media  = libvlc_media_new_location(_vlc_inst,fname.toUtf8().data());
        libvlc_media_player_t *_vlc_play=  libvlc_media_player_new_from_media(_vlc_media);
//        libvlc_video_set_mouse_input(_vlc_play,false);
//        libvlc_media_release(_vlc_media);
        libvlc_media_player_set_hwnd(_vlc_play,(void*)playframe.at(i)->getWindowId());
        playframe.at(i)->frame->setPlaying(true);

        libvlc_media_add_option(_vlc_media,captureOption.toLocal8Bit().data());
        libvlc_media_player_play(_vlc_play);


        vlcItem tmp;
        tmp._vlcInstance = _vlc_inst;
        tmp._vlcMediaPlayer = _vlc_play;
        vlcItemList.append(tmp);

    }
}

void MainWindow::ReadQss()
{

    QFile qss(":/lcy/images/systemsetting.qss");
    if(!qss.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    this->setStyleSheet(qss.readAll());
    qss.close();


}

bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *me = static_cast<QMouseEvent*>(e);
        if(me->button() == Qt::RightButton)
            QMessageBox::warning(this,"o","eee");
    }
    return QMainWindow::eventFilter(o,e);

}


void MainWindow::slot_StopPlay()
{
    foreach(vlcItem p,vlcItemList)
    {
        if(p._vlcMediaPlayer)
        {
            libvlc_media_player_stop(p._vlcMediaPlayer);
            libvlc_media_player_release(p._vlcMediaPlayer);
        }

        if(p._vlcInstance)
        libvlc_release(p._vlcInstance);
    }
    vlcItemList.clear();
}

void MainWindow::slot_StartPlay()
{

//    QStringList playlist = panel->getPlayList();

//    int num = playframe.count() < playlist.count() ? playframe.count() : playlist.count();
//    while(plist.count() < num)
//    {
//        QProcess* p = new QProcess;
//        plist.append(p);
//    }


//   const QString play = qApp->applicationDirPath()+tr("/mplayer.exe");
//   for(int i = 0; i < num;i++)
//   {

//       QString http = tr("http://")+playlist.at(i).section(",",1,1);
//       QString fname =  http +tr("/")+ playlist.at(i).section(",",0,0);
//       plist.at(i)->start(play,QStringList () <<  "-wid" << QString::number(playframe.at(i)->winId())<< fname.append(".avi"));
//       plist.at(i)->waitForStarted(5000);

//   }
    vlcPlayRtsp();
}


void MainWindow::slot_GridNumberChanged(int num)
{
    if(num == fullscreen )
        view->setFullScreen();
    else if(num != othermenu)
    {
        view->setGridnumber(num,num);
    }
    else
        view->setOnePlusSeven();

    QList<WindowFrame*> playframe = view->getPlayFrame();
    QStringList playlist = panel->getPlayList();
    int n = playframe.count() < playlist.count() ? playframe.count() : playlist.count();
    if(vlcItemList.count() == n)
        return ;
    SqlInstance sql;

    QStringList l = sql.GetColumnsList("camera_settings",QStringList() << "camera_name"
                                      << "camera_verifyid");
    for(int i = vlcItemList.count() ; i < n ; i++)
    {

        if(l.isEmpty())
            continue;
        playframe[i]->toggle_ctrlWidget_view(1);
        playframe[i]->frame->setCameraName(l.at(i).split(',').first());
        playframe[i]->frame->setCameraVerifyId(l.at(i).split(',').last());
    }
//    sql.closedb();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if( e->text() == "f")
    {
        view->swapFullScreenOrNormal(true);
    }
}

MainWindow::~MainWindow()
{
    delete view;
    foreach(QProcess *p, plist)
    {
        p->terminate();
        delete p;
    }
    slot_StopPlay();

}
