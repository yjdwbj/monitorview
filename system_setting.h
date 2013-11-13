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

};

#endif // SYSTEM_SETTING_H
