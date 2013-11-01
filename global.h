#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QMessageBox>
#include <QApplication>
#include <QSignalMapper>
#include <QVariant>
#include <QPair>
#include <QMenu>
#include <QTimer>


typedef QPair<int,QString> itemWidget;
static const int othermenu =10;
static const int fullscreen = 11;
//static bool fullscreentoggle = false;


class VHWidget : public QGroupBox
{
    Q_OBJECT
    Q_ENUMS(LayoutForm)
public:
        enum LayoutForm {
            Horizontal,
            Vertical
        };
    VHWidget(QList<itemWidget> &list,const LayoutForm form, QWidget *parent = 0)
        :QGroupBox(parent)
    {
        signalMapper = new QSignalMapper(this);
        QBoxLayout *layout = new QBoxLayout(form ? QBoxLayout::TopToBottom : QBoxLayout::LeftToRight );
        layout->setSpacing(5);
        layout->addStretch();
        for(int i = 0 ; i < list.size();i++)
        {
            QPushButton *btn = new QPushButton;
            btn->setObjectName(QString::number(list.at(i).first));

            if(list.at(i).first == othermenu)
            {
                QMenu *menu = new QMenu();
                menu->addAction("1+7");
                for(int i =5; i < 10;i++)
                {
                    QString s = QString::number(i);
                    menu->addAction(s+tr("x")+s);
                }
                btn->setMenu(menu);
                connect(menu,SIGNAL(triggered(QAction*)),SLOT(slot_MenuAction(QAction*)));
            }
            else
            {
                connect(btn,SIGNAL(clicked()),signalMapper,SLOT(map()));
                signalMapper->setMapping(btn,list.at(i).first);
            }

            btn->setStyleSheet("background-image: url("+
                               list.at(i).second+");background-position:center;");
            QImage img;
            img.load(list.at(i).second);
            btn->setFixedSize(img.size());
            layout->addWidget(btn);
        }
        if(form)
        {
            QHBoxLayout *p = (QHBoxLayout *)layout;
            this->setLayout(p);
        }
        else
        {
            QVBoxLayout *p = (QVBoxLayout *)layout;
            this->setLayout(p);

        }
        connect(signalMapper,SIGNAL(mapped(int)),
                this,SIGNAL(gridofnumer(int)));
    }
    ~VHWidget(){}

private slots:
    void slot_MenuAction(QAction* act)
    {
        if(act->text().contains("x"))
        {
            int num = act->text().section("x",0,0).toInt();
            emit gridofnumer(num);
        }
        else
        {
            emit gridofnumer(10);
        }
    }

signals:
    void gridofnumer(int n);
private:
    QSignalMapper *signalMapper;

};

#endif // GLOBAL_H
