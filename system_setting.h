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
private:
    QListWidget *listwidget;
    QStackedLayout *stackLayout;

    QLayout *ViewPanel();
    QLayout *StoragePanel();
    QLayout *AutoRunning();
    QWidget *getWidgetFromLayout(QLayout *lay){QGroupBox *w = new QGroupBox;w->setLayout(lay);return (QWidget*)w;}
private slots:
    void slot_fontdialog();
    void slot_ListRowChanged(int);
};

#endif // SYSTEM_SETTING_H
