#include "searchcamera.h"

static const QStringList headerlist  = QString("序号,名称,访问地址,机身编码,子网掩码,网关,主DNS服务器").split(",");

HostView::HostView(QWidget *parent)
    :QTableWidget(parent)
{

    setShowGrid(false);
    setColumnCount(headerlist.count());
    setFixedWidth(700);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->hide();

    setHorizontalHeaderLabels(headerlist);

    const QStringList itemlist = QString("test,192.168.8.10:9999,1111,255.255.255.0,192.168.8.1,8.8.8.8").split(",");


    addNewLine(itemlist);
    addNewLine(itemlist);

    show();
}

void HostView::addNewLine(const QStringList &list)
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


SearchCamera::SearchCamera(QDialog *parent)
    :QDialog(parent),
      m_viewlist(new HostView)
{
    QVBoxLayout *main_layout =  new QVBoxLayout;
    setWindowTitle("批量添加局域网内摄像机");
    const QStringList btnlist = QString("全选,全不选,刷新,提交,取消").split(",");
    QHBoxLayout *lay_btn = new QHBoxLayout;
    foreach(const QString &str, btnlist)
    {
        QPushButton *btn = new QPushButton(str);
        btn->setObjectName(str);
        lay_btn->addWidget(btn);
    }

    main_layout->addWidget(m_viewlist);
    main_layout->addLayout(lay_btn);
    setLayout(main_layout);
    show();


}
