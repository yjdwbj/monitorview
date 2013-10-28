#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    main_layout(new QHBoxLayout),
    view(new ViewFrame),
    panel(new SettingPanel)
{

    main_layout = new QHBoxLayout;
    connect(panel,SIGNAL(sig_gridofnumber(int)),SLOT(slot_gridnumberchanged(int)));
    main_layout->addWidget(view);
    main_layout->addWidget(panel);


    QWidget *w = new QWidget;
    w->setLayout(main_layout);

    setCentralWidget(w);

}

void MainWindow::slot_gridnumberchanged(int num)
{
    if(num != othermenu)
    view->setGridnumber(num,num);
    else
        view->setOnePlusSeven();
}

MainWindow::~MainWindow()
{

}
