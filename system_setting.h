#ifndef SYSTEM_SETTING_H
#define SYSTEM_SETTING_H
#include "global.h"

#include <QDialog>
#include <QListView>
#include <QListWidget>
#include <QStackedLayout>
#include <QTreeWidgetItem>

namespace Ui {
class systemdialog;
}

class SystemSetting : public QDialog
{
    Q_OBJECT
public:
    explicit SystemSetting(QWidget *parent=0);
    ~SystemSetting(){}
private slots:
    void on_store_add_clicked();

    void on_auto_chkautorun_toggled(bool checked);

    void on_view_btnfont_clicked();

    void on_pushButton_3_clicked();

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_store_del_clicked();

private:


    QSettings *set;
    void closeEvent(QCloseEvent *e);
    void accept();
    QStringList getTreeWidgetChildLst();


    Ui::systemdialog *ui;

};

#endif // SYSTEM_SETTING_H
