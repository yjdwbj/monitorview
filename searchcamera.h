#ifndef SEARCHCAMERA_H
#define SEARCHCAMERA_H
#include "global.h"
#include <QDialog>
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>

#include <QtWidgets/QCheckBox>


namespace Ui {
class Dialog;
}

class SearchCamera : public QDialog
{
    Q_OBJECT
public:
    explicit SearchCamera(QWidget *parent =0);
    ~SearchCamera(){ delete ui;}
    QStringList getVaildCameraList() const;
    void addNewLine(QStringList list);
    QStringList getViewCountList() const;

private slots:
    void slot_SwitchCheckedAll();
    void slot_SwitchCheckedNone();

private:
    void emulator_readfile();
    void Switch_CheckBox(bool);
    QStringList vaidlist;
    QSignalMapper *signalmap;
    Ui::Dialog *ui;
};


#endif // SEARCHCAMERA_H
