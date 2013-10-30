#ifndef SEARCHCAMERA_H
#define SEARCHCAMERA_H
#include "global.h"
#include <QDialog>
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QtWidgets/QCheckBox>

class HostView : public QTableWidget
{
    Q_OBJECT
public:
    explicit HostView(QWidget *parent=0);
    ~HostView(){}
    void addNewLine(const QStringList &list);
};



class SearchCamera : public QDialog
{
    Q_OBJECT
public:
    explicit SearchCamera(QDialog *parent =0);
    ~SearchCamera(){}

private:
    HostView *m_viewlist;
};


#endif // SEARCHCAMERA_H
