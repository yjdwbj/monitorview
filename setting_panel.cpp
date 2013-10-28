#include "setting_panel.h"
SettingPanel::SettingPanel(QWidget *parent)
    :QWidget(parent)
{
    QGroupBox *gbox = new QGroupBox();

    QList<itemWidget> itemlist;
    itemlist  << qMakePair(1,tr(":/lcy/images/one.png")) << qMakePair(2,tr(":/lcy/images/four.png"))
              << qMakePair(3,tr(":/lcy/images/nine.png")) << qMakePair(4,tr(":/lcy/images/sixteen.png"))
              << qMakePair(othermenu,tr(":/lcy/images/other.png")) << qMakePair(fullscreen,tr(":/lcy/images/fullscreen.png"));
    VHWidget *widget = new VHWidget(itemlist,VHWidget::Horizontal);
    connect(widget,SIGNAL(gridofnumer(int)),SIGNAL(sig_gridofnumber(int)));
    QVBoxLayout *main_layout = new QVBoxLayout(gbox);
    main_layout->addWidget(widget);
    this->setLayout(main_layout);
}



SettingPanel::~SettingPanel()
{

}
