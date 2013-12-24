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
    connect(panel,SIGNAL(deleteCamera(QString)),view,SLOT(slot_deleteCamera(QString)));
    connect(panel,SIGNAL(addedNewCamera(int)),SLOT(slot_viewCtrolWidget(int)));
    connect(panel,SIGNAL(gotoWorkState(int)),SLOT(slot_playById(int)));
    connect(panel,SIGNAL(outofWorkState(int)),SLOT(slot_stopById(int)));


    main_layout->addWidget(view);
    main_layout->addWidget(panel);



    QWidget *w = new QWidget;
    w->setLayout(main_layout);
    resize(QSize(800,400));

    setCentralWidget(w);
    readLayout();

}

void MainWindow::readLayout()
{

    QSettings set(qApp->applicationDirPath()+"/options.ini",QSettings::IniFormat);
    QString lay = set.value("global/Layout").toString();

    if(!lay.compare("1x7"))
    {
        view->setOnePlusSeven();
    }
    else
    {
        int x = lay.section('x',0,0).toInt();
//        int y = lay.section('x',1,1).toInt();
        slot_GridNumberChanged(x);
    }

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
    QString verifyid ;
    for(int i = vlcItemList.count() ; i < num ; i++)
    {
        verifyid = playlist.at(i);
        QString captureOption(":sout=#stream_out_duplicate{dst=display,dst=std{access=file,mux,ts,dst=");
        captureOption+=tr("D:/camera-video/")+tr("camera")+QString::number(i)+tr(".mpg}}");
        QString url = tr("rtsp://192.168.8.31/")+verifyid;
        QString fname =  url + tr(".mpg");
        libvlc_instance_t *_vlc_inst = libvlc_new(0,NULL);
        libvlc_media_t *_vlc_media  = libvlc_media_new_location(_vlc_inst,fname.toUtf8().data());
        libvlc_media_player_t *_vlc_play=  libvlc_media_player_new_from_media(_vlc_media);
        WId wid = view->getPlayFrameWId(verifyid);
        if(wid == 0)
            continue;
        libvlc_media_player_set_hwnd(_vlc_play,(void*)wid);
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

void MainWindow::closeEvent(QCloseEvent *e)
{
    int res = QMessageBox::question(this,"提示","你确定要退出吗?");
    if(res== QMessageBox::Yes)
    {
        QSettings set(qApp->applicationDirPath()+"/options.ini",
                      QSettings::IniFormat);
        set.setValue("global/Layout",view->getGridnumber());
        set.sync();
        e->accept();
    }
    else
    {
        e->ignore();
    }
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

void MainWindow::slot_playById(int id)
{
     QList<WindowFrame*> playframe = view->getPlayFrame();
    QString  cameraname = panel->m_TreeView->topLevelItem(id)->text(0);

    QString  verifyid = SqlInstance::getColumnFirst("camera_settings",
                                                    "camera_verifyid","camera_name",cameraname);
    QStringList l  = SqlInstance::getColumnList("hostinfo","addrport");
    if(l.isEmpty())
        return;
    QString url = tr("rtsp://")+l.at(id)+"/"+verifyid;
    QString fname =  url + tr(".mpg");
    libvlc_instance_t *_vlc_inst = libvlc_new(0,NULL);
    libvlc_media_t *_vlc_media  = libvlc_media_new_location(_vlc_inst,fname.toUtf8().data());
    libvlc_media_player_t *_vlc_play=  libvlc_media_player_new_from_media(_vlc_media);
    WId wid = view->getPlayFrameWId(verifyid);
    if(wid == 0)
        return;
    libvlc_media_player_set_hwnd(_vlc_play,(void*)wid);
    playframe.at(id)->frame->setPlaying(true);

//    libvlc_media_add_option(_vlc_media,captureOption.toLocal8Bit().data());
    libvlc_media_player_play(_vlc_play);
    vlcItem tmp;
    tmp._vlcInstance = _vlc_inst;
    tmp._vlcMediaPlayer = _vlc_play;
    tmp.order = id;

    bool f = true;
    for(int i = 0; i < vlcItemList.size();i++)
    {
        if(vlcItemList[i].order == id)
        {
            vlcItemList.replace(i,tmp);
            f = false;
            break;
        }
    }
    if(f)
        vlcItemList.append(tmp);


}

void MainWindow::slot_stopById(int id)
{

    foreach(vlcItem it,vlcItemList)
    {
        if(it.order == id)
        {
            libvlc_media_player_stop(it._vlcMediaPlayer);
            libvlc_media_player_release(it._vlcMediaPlayer);
            libvlc_release(it._vlcInstance);
            break;
        }
    }


}

void MainWindow::slot_StartPlay()
{
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
//    if(vlcItemList.count() == n)
//        return ;


    QStringList l = SqlInstance::getColumnsList("camera_settings",QStringList() << "camera_name"
                                      << "camera_verifyid");
    for(int i = vlcItemList.count() ; i < n ; i++)
    {

        if(l.isEmpty())
            continue;
        playframe[i]->toggle_ctrlWidget_view(1);
        playframe[i]->frame->setCameraName(l.at(i).split(',').first());
        playframe[i]->frame->setCameraVerifyId(l.at(i).split(',').last());
    }

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
