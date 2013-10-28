#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "global.h"
#include "setting_panel.h"

class ViewFrame : public QGroupBox
{
    Q_OBJECT
public:
    explicit ViewFrame(QWidget *parent =0)
        :QGroupBox(parent),
          lay(new QGridLayout)
    {

//        QGridLayout *lay = new QGridLayout;
        lay->setSpacing(0);
        lay->setMargin(1);
        lay->setVerticalSpacing(10);

        setGridnumber(1,1);
        this->setFixedSize(540,400);
        this->setLayout(lay);

    }
    ~ViewFrame(){
        delete this;
//        deleteLater();
    }
    void setGridnumber(int row,int col)
    {
        if(m_list.size()!=0)
        {
            for(int i= 0 ; i < m_list.size();i++)
            {
                lay->removeWidget(m_list.at(i));
            }

        }

        while(m_list.size() < (row*col))
        {
            QFrame *f = new QFrame;
            f->setFrameShape(QFrame::WinPanel);
            m_list.append(f);
        }

        while(m_list.size() > (row*col))
        {
            delete m_list.takeLast();
        }


        int n = 0;
        for(int x = 0; x < row;x++)
        {

            for(int y = 0; y < col;y++)
            {
                lay->addWidget(m_list.at(n++),x,y);
            }

        }
    }
    void setOnePlusSeven()
    {
        if(m_list.size()!=0)
        {
            for(int i= 0 ; i < m_list.size();i++)
            {
                lay->removeWidget(m_list.at(i));
            }

        }

        while(m_list.size() < 8)
        {
            QFrame *f = new QFrame;
            f->setFrameShape(QFrame::WinPanel);
            m_list.append(f);
        }

        while(m_list.size() > 8)
        {
            delete m_list.takeLast();
        }


        lay->addWidget(m_list.at(0),0,0,3,3);
        lay->addWidget(m_list.at(1),0,3);
        lay->addWidget(m_list.at(2),1,3);
        lay->addWidget(m_list.at(3),2,3);
        lay->addWidget(m_list.at(4),3,0);
        lay->addWidget(m_list.at(5),3,1);
        lay->addWidget(m_list.at(6),3,2);
        lay->addWidget(m_list.at(7),3,3);

    }

private:

    QGridLayout *lay;
    QList<QFrame*> m_list;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slot_gridnumberchanged(int);
    
private:
    ViewFrame *view;
    QHBoxLayout *main_layout;
    SettingPanel *panel;
};

#endif // MAINWINDOW_H
