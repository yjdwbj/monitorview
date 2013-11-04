#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QCheckBox>
#include <QMessageBox>
#include <QApplication>
#include <QSignalMapper>
#include <QVariant>
#include <QPair>
#include <QMenu>
#include <QTimer>
#include <QHeaderView>


typedef QPair<int,QString> itemWidget;
static const int othermenu =10;
static const int fullscreen = 11;
//static bool fullscreentoggle = false;


////////////// VHWidget ///////////

class VHWidget : public QGroupBox
{
    Q_OBJECT
    Q_ENUMS(LayoutForm)
public:
        enum LayoutForm {
            Horizontal,
            Vertical
        };
    VHWidget(QList<itemWidget> &list,const LayoutForm form, QWidget *parent = 0);
    ~VHWidget(){}

private slots:
    void slot_MenuAction(QAction* act);

signals:
    void gridofnumer(int n);
private:
    QSignalMapper *signalMapper;

};


//////////////LabAndLineEdit/////////////

class LabAndLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit LabAndLineEdit(const QString &labstr,const QString &tip ="",const QString context="",QWidget *parent=0);
    ~LabAndLineEdit(){}
private:
    QLineEdit *edit;
};


/////////////ListView////////////////


class ListView : public QTableWidget
{
    Q_OBJECT
public:
    explicit ListView(const QStringList &list, QWidget *parent=0);
    ~ListView(){}
    void addNewLine(const QStringList &list);
    QStringList getViewCountList()const ;
private:
        QStringList orglist;
};



#endif // GLOBAL_H
