#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    main_layout(new QHBoxLayout),
    view(new ViewFrame),
    panel(new SettingPanel)
{

    main_layout = new QHBoxLayout;
    connect(panel,SIGNAL(sig_gridofnumber(int)),SLOT(slot_GridNumberChanged(int)));
    main_layout->addWidget(view);
    main_layout->addWidget(panel);



    QWidget *w = new QWidget;
    w->setLayout(main_layout);

    setCentralWidget(w);

}

void MainWindow::slot_GetViewFrameClicked(QRect rect)
{
    int x = rect.right();
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
        view->swapFullScreenOrNormal();
    }
}

MainWindow::~MainWindow()
{
    delete view;
}
