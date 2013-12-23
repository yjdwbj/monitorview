#ifndef SYSTEM_SETTING_H
#define SYSTEM_SETTING_H
#include "global.h"
#include <QDialog>
#include <QListView>
#include <QListWidget>
#include <QStackedLayout>

class SystemSetting : public QDialog
{
    Q_OBJECT
public:
    explicit SystemSetting(QWidget *parent=0);
    ~SystemSetting(){}
    QString getStoragePath() { return pathText->item(0)->text();}
private:
    QListWidget *listwidget;
    QStackedLayout *stackLayout;
    QSignalMapper *signalmap;
    QListWidget *pathText;
    QPushButton *btn_addPath ;
    QCheckBox *cbox_viewname;
    QCheckBox *cbox_viewtimestamp;
    QCheckBox *StartAfterBoot;
    LabAndWidget *user;
    LabAndWidget *pass;
    QCheckBox *tray;
    QCheckBox *autoconnect;
    LabAndWidget *srvaddr;
    LabAndWidget *page_up;
    LabAndWidget *loss_camera;
    QCheckBox *log_img;
    QCheckBox *check_version;


    void closeEvent(QCloseEvent *e);
    void accept();

    QLayout *ViewPanel();
    QLayout *StoragePanel();
    QLayout *AutoRunning();
    QLayout *OtherPanel();
    QWidget *getWidgetFromLayout(QLayout *lay){QGroupBox *w = new QGroupBox;w->setLayout(lay);return (QWidget*)w;}
private slots:
    void slot_fontdialog();
    void slot_ListRowChanged(int);
    void slot_yesornocommit(int);
    void slot_addORdeleteStoragePath(int);
    void on_Yes_clicked();
    void on_StartAfterBoot_toggled(bool checked);

};

#endif // SYSTEM_SETTING_H
