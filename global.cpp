#include "global.h"


VHWidget::VHWidget(QList<itemWidget> &list, const LayoutForm form, QWidget *parent)
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


void VHWidget::slot_MenuAction(QAction *act)
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


LabAndLineEdit::LabAndLineEdit(const QString &labstr, const QString &tip, const QString context, QWidget *parent)
    :QWidget(parent)
{
    edit = new QLineEdit(context);
    edit->setToolTip(tip);

    QLabel *lab = new QLabel(labstr);
    lab->adjustSize();
    lab->setToolTip(tip);
    QHBoxLayout *lay = new QHBoxLayout;
//        lay->addStretch(spcing);
    lay->setMargin(1);
    lay->setSpacing(1);
    lay->addWidget(lab,Qt::AlignLeft);
    lay->addWidget(edit,Qt::AlignLeft);
    setLayout(lay);
}


ListView::ListView(const QStringList &list,QWidget *parent)
    :QTableWidget(parent)
{

    orglist = list;
    setContentsMargins(1,1,1,1);
    setShowGrid(false);
    setColumnCount(orglist.count());
//    setFixedWidth(700);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->hide();
    setHorizontalHeaderLabels(orglist);
    show();
}

void ListView::addNewLine(const QStringList &list)
{

    int count = this->rowCount();
    insertRow(count);
    int num =0;
    QCheckBox *box= new QCheckBox(QString::number(count+1));
    box->setChecked(true);
    setCellWidget(count,num++,box);
    foreach(const QString &str, list)
    {
        setItem(count,num++,new QTableWidgetItem(str));
    }
}


QStringList ListView::getViewCountList() const
{
    int rcout = this->rowCount();
    QStringList list;
    for(int i = 0 ; i < rcout ;i++ )
    {
        QCheckBox *box = qobject_cast<QCheckBox *>(cellWidget(i,0));
        if(!box->isChecked())
            continue;
        QStringList t;
        for(int j = 1 ; j < orglist.count();j++)
        {
            t << item(i,j)->text();
        }

        list << t.join(",");
        t.clear();
    }

    return list;
}
