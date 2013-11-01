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
//    QStringList cameralist =QString("camera1.avi,camera2.avi,camera3.avi").split(",");
    QStringList playlist = panel->getPlayList();
    QList<MyFrame*> playframe = view->getPlayFrame();
    int num = playframe.count() < playlist.count() ? playframe.count() : playlist.count();
    while(plist.count() < num)
    {
        QProcess* p = new QProcess;
        plist.append(p);
    }


   const QString play = qApp->applicationDirPath()+tr("/mplayer.exe");
   for(int i = 0; i < num;i++)
   {

       QString http = tr("http://")+playlist.at(i).section(",",1,1);
       QString fname =  http +tr("/")+ playlist.at(i).section(",",0,0);
       plist.at(i)->start(play,QStringList () <<  "-wid" << QString::number(playframe.at(i)->winId())<< fname.append(".avi"));
       plist.at(i)->waitForStarted(5000);

   }
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
}
