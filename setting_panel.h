#ifndef SETTING_PANEL_H
#define SETTING_PANEL_H
#include "global.h"


class SettingPanel :public QWidget
{
    Q_OBJECT
public:
    explicit SettingPanel(QWidget *parent = 0);

    ~SettingPanel();
private slots:
    void slot_addnewCamera();
    void slot_searchCamera();
private:
//    QVBoxLayout *main_layout;
signals:
    void sig_gridofnumber(int);
};

#endif // SETTING_PANEL_H
