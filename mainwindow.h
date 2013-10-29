#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "global.h"
#include "setting_panel.h"
#include "viewframe.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *e);


private slots:
    void slot_GridNumberChanged(int);
    void slot_GetViewFrameClicked(QRect);
private:

    ViewFrame *view;
    QHBoxLayout *main_layout;
    SettingPanel *panel;
};

#endif // MAINWINDOW_H
