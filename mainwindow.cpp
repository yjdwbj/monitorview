#include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    main_layout(new QHBoxLayout),
    view(new ViewFrame),
    panel(new SettingPanel)
{


    main_layout = new QHBoxLayout;
    connect(panel,SIGNAL(sig_gridofnumber(int)),SLOT(slot_GridNumberChanged(int)));
    connect(panel,SIGNAL(StartPlayer()),SLOT(slot_StartPlayer()));
    main_layout->addWidget(view);
    main_layout->addWidget(panel);



    QWidget *w = new QWidget;
    w->setLayout(main_layout);

    setCentralWidget(w);

}

void MainWindow::slot_StartPlayer()
{
     QList<MyFrame*> playframe = view->getPlayFrame();
    const char * const vlc_args[] = {
        "-I","dummy","--ignore-config","--extraintf=logger","--verbose=2"};
//        "--plugin-path=O:\\tool\\WebCameraClient\\plugins\\"};
//    _vlcinstance = libvlc_new(sizeof(vlc_args)/sizeof(vlc_args[0]),vlc_args);
    _vlcinstance = libvlc_new(0,NULL);


    _m = libvlc_media_new_path(_vlcinstance,"http://localhost:8090/camera1.avi");
//    _mp = libvlc_media_player_new_from_media(_m);
    libvlc_media_release(_m);
    libvlc_media_player_set_media(_mp,_m);
    libvlc_media_player_set_hwnd(_mp,reinterpret_cast<void*>(playframe.at(0)->winId()));
    libvlc_media_player_play(_mp);
//    QStringList cameralist =QString("camera1.avi,camera2.avi,camera3.avi").split(",");
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
    libvlc_media_player_stop(_mp);
    libvlc_media_player_release(_mp);
    libvlc_release(_vlcinstance);
}
