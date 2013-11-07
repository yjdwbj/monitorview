#include "global.h"

GroupChecBox::GroupChecBox(const QStringList &list,const QString &prefix,
                           LayoutOriant form,QWidget *parent)
    :QWidget(parent)
{
//    QHBoxLayout *lay = new QHBoxLayout ;
    QBoxLayout *layout = new QBoxLayout(form ? QBoxLayout::TopToBottom : QBoxLayout::LeftToRight );
    foreach(const QString &str,list)
    {
        QCheckBox *cbox = new QCheckBox(prefix+str);
        layout->addWidget(cbox);
    }
    setLayout(layout);
}


GroupBtnWidget::GroupBtnWidget(const QStringList &list,const LayoutForm form,
                               const int width,QWidget *parent)
    :QWidget(parent)
{
    QBoxLayout *layout = new QBoxLayout(form ? QBoxLayout::TopToBottom : QBoxLayout::LeftToRight );
    QSignalMapper *signalmap = new QSignalMapper(this);
    int i = 0;
    foreach(const QString &str,list)
    {
        QPushButton *btn = new QPushButton(str);
        btn->setFixedWidth(width);
        layout->addWidget(btn);
        signalmap->setMapping(btn,i++);
         connect(btn,SIGNAL(clicked()),signalmap,SLOT(map()));
    }
    connect(signalmap,SIGNAL(mapped(int)),SIGNAL(SignalById(int)));
    this->setLayout(layout);
}


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


LabAndWidget::LabAndWidget(const QString &label, QComboBox *cbb, const QStringList &items,
                               const QString &tip, int spcing, QWidget *parent)
    :QWidget(parent)
{


    QLabel *lab = new QLabel(label);
    lab->adjustSize();
    lab->setToolTip(tip);
    QHBoxLayout *lay = new QHBoxLayout;
    cbb->addItems(items);
    lay->setMargin(1);
    lay->setSpacing(spcing);
    lay->addWidget(lab,Qt::AlignLeft);
    lay->addWidget(cbb,Qt::AlignLeft);
    lay->addStretch(0);
    setLayout(lay);
    two = static_cast<QWidget*>(cbb);
}

LabAndWidget::LabAndWidget(const QString &label, QLineEdit *le,LayoutForm form ,const QString &tip,
                               int spcing, QWidget *parent)
    :QWidget(parent)
{
    QLabel *lab = new QLabel(label);
    lab->adjustSize();
    lab->setToolTip(tip);
//    QHBoxLayout *lay = new QHBoxLayout;
    QBoxLayout *lay = new QBoxLayout(form ? QBoxLayout::TopToBottom : QBoxLayout::LeftToRight );
    lay->setMargin(1);
    lay->setSpacing(spcing);
    lay->addWidget(lab);
    lay->addWidget(le);
    lay->addStretch();
    setLayout(lay);
     two = static_cast<QWidget*>(le);
}

LabAndWidget::LabAndWidget(const QString &label, QSpinBox *sb, const int value, const QString &tip,
                               int spcing, QWidget *parent)
    :QWidget(parent)
{
    QLabel *lab = new QLabel(label);
    lab->adjustSize();
    lab->setToolTip(tip);
    QHBoxLayout *lay = new QHBoxLayout;
    sb->setValue(value);
    lay->setMargin(1);
    lay->setSpacing(spcing);
    lay->addWidget(lab,Qt::AlignLeft);
    lay->addWidget(sb,Qt::AlignLeft);
    lay->addStretch(0);
    setLayout(lay);
     two = static_cast<QWidget*>(sb);
}

LabAndWidget::LabAndWidget(const QString &label, QTimeEdit *te,LayoutForm form,
                           const QString &tip,int spcing, QWidget *parent)
    :QWidget(parent)
{
    QLabel *lab = new QLabel(label);
    lab->adjustSize();
    lab->setToolTip(tip);
     QBoxLayout *lay = new QBoxLayout(form ? QBoxLayout::TopToBottom : QBoxLayout::LeftToRight );
    lay->setMargin(1);
    lay->setSpacing(spcing);
    lay->addWidget(lab,Qt::AlignLeft);
    lay->addWidget(te,Qt::AlignLeft);
    lay->addStretch(0);
    setLayout(lay);
     two = static_cast<QWidget*>(te);
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
