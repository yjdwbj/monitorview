#include "setting_trap_time.h"
#include "alarmaction.h"
#include <QTimeEdit>
#include <QSpinBox>
#include <QGraphicsLayout>

static QString week("一,二,三,四,五,六,日");
static QString triggerTerm("移动侦测,报警输入1,报警输入2,报警输入3,报警输入4");
static QString choices("全选,全不选");

AddTrapTimeDialog::AddTrapTimeDialog(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("添加布防时间段");
    QVBoxLayout *main_layout = new QVBoxLayout;

    QGroupBox *gbox_week = new QGroupBox("星期");
    QGridLayout *lay_week = new QGridLayout(gbox_week);
    lay_week->addWidget(new GroupBtnWidget(choices.split(",")),0,0,1,2);
    int i = 0;
    foreach(const QString &str,week.split(","))
    {
        QCheckBox *cbox = new QCheckBox("星期"+str);
        lay_week->addWidget(cbox,1,i++);
    }

    main_layout->addWidget(gbox_week);

    QGroupBox *gbox_time = new QGroupBox("时间");
    QHBoxLayout *lay_time = new QHBoxLayout(gbox_time);

    QLabel *s_time = new QLabel("起始时间:");
    QTimeEdit *s_timeEdit = new QTimeEdit(QTime(0,0));
    QLabel *e_time = new QLabel("终止时间:");
    QTimeEdit *e_timeEdit = new QTimeEdit(QTime(23,59,59));
    QLabel *warn_time = new QLabel("报警持续时间(秒):");
    QSpinBox *warn_sbox = new QSpinBox();

    lay_time->addWidget(s_time);
    lay_time->addWidget(s_timeEdit);
    lay_time->addWidget(e_time);
    lay_time->addWidget(e_timeEdit);
    lay_time->addWidget(warn_time);
    lay_time->addWidget(warn_sbox);
    lay_time->addStretch();

    main_layout->addWidget(gbox_time);



    QGroupBox *gbox_trigger = new QGroupBox("触发条件");
    QGridLayout *lay_trigger = new QGridLayout(gbox_trigger);
    lay_trigger->addWidget(new GroupBtnWidget(choices.split(",")),0,0,1,2);
    i = 0;
    foreach(const QString &str,triggerTerm.split(","))
    {
        QCheckBox *cbox = new QCheckBox(str);
        lay_trigger->addWidget(cbox,1,i++);
    }
    main_layout->addWidget(gbox_trigger);
    main_layout->addWidget(new AlarmAction);
    setLayout(main_layout);

}

