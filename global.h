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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QSpinBox>
#include <QMessageBox>
#include <QApplication>
#include <QSignalMapper>
#include <QVariant>
#include <QPair>
#include <QMenu>
#include <QTimer>
#include <QHeaderView>
#include <QMouseEvent>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>



typedef QPair<int,QString> itemWidget;
static const int othermenu =10;
static const int fullscreen = 11;
//static bool fullscreentoggle = false;

static QString choices("全选,全不选");
static QString yesorno("提交,取消");
static QString weekNum("一,二,三,四,五,六,日");
enum LayoutOriant {
    Horizontal,
    Vertical
};


typedef struct {
    int id;
    char *name;
    int hostinfo;
    char *verifyid;
    char *logo_name;
    char *logo_passwd;
    bool yuntai;
    bool reflactimage;
    char *append_name;
    char *append_phone;
    char *append_commit;
    int  alarm;
    int  record;
}_StructSettings;




class LabelBtn : public QLabel
{
    Q_OBJECT
public:
    explicit LabelBtn(const QPixmap &pix, const QString &tip="",QWidget *parent=0);
    ~LabelBtn(){}
protected:
    void mousePressEvent(QMouseEvent *ev);
signals:
    void mouse_pressed();
};


class GroupChecBox : public QWidget
{
    Q_OBJECT
public:
    explicit GroupChecBox(const QStringList &list,const QString &prefix="",
                          LayoutOriant form = Horizontal,QWidget *parent=0);
    ~GroupChecBox(){}
};


///////////// GroupButtonWidget //////

class GroupBtnWidget : public QWidget
{
    Q_OBJECT
public:
    enum LayoutForm {
        Horizontal,
        Vertical
    };
    explicit GroupBtnWidget(const QStringList &list,const LayoutForm form = Horizontal,const int width=60,QWidget *parent=0);
    ~GroupBtnWidget(){}
signals:
    void SignalById(int);
};


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

class LabAndWidget : public QWidget
{
    Q_OBJECT
public:
    enum LayoutForm {
        Horizontal,
        Vertical
    };
     LabAndWidget(const QString &label,QLineEdit *le,LayoutForm form = Horizontal,const QString &tip ="",int spcing =5,QWidget *parent=0);
     LabAndWidget(const QString &label,QComboBox *cbb, const QStringList &items,const QString &tip="", int spcing=5, QWidget *parent=0);
     LabAndWidget(const QString &label, QSpinBox *sb, const int value, const QString &tip="", int spcing=5, QWidget *parent =0);
     LabAndWidget(const QString &label, QTimeEdit *te, LayoutForm form = Horizontal, const QString &tip ="", int spcing = 5, QWidget *parent = 0);
     ~LabAndWidget(){}

     QWidget *getTwoObj() { return two;}
private:
//    T one;
    QWidget *two;
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
