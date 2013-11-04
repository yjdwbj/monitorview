#ifndef SEARCHCAMERA_H
#define SEARCHCAMERA_H
#include "global.h"
#include <QDialog>
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>

#include <QtWidgets/QCheckBox>




class SearchCamera : public QDialog
{
    Q_OBJECT
public:
    explicit SearchCamera(QDialog *parent =0);
    ~SearchCamera(){}
    QStringList getVaildCameraList() const {return m_viewlist->getViewCountList();}

private slots:
    void slot_SwitchCheckedAll();
    void slot_SwitchCheckedNone();
    void slot_Reload();
    void slot_MapSignal(int id);

private:
    void Switch_CheckBox(bool);
    ListView *m_viewlist;
    QStringList vaidlist;
    QSignalMapper *signalmap;
};


#endif // SEARCHCAMERA_H
