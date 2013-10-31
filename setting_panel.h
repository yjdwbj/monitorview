#ifndef SETTING_PANEL_H
#define SETTING_PANEL_H
#include "global.h"
#include "viewframe.h"


class SettingPanel :public QWidget
{
    Q_OBJECT
public:
    explicit SettingPanel(QWidget *parent = 0);

    ~SettingPanel();
private slots:
    void slot_addnewCamera();
    void slot_searchCamera();
    void slot_StartPlayer();
private:
//    QVBoxLayout *main_layout;
signals:
    void sig_gridofnumber(int);
    void StartPlayer();
};

#endif // SETTING_PANEL_H
