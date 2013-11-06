#include "setting_trap_time.h"
#include "alarmaction.h"
#include <QTimeEdit>
#include <QSpinBox>
#include <QGraphicsLayout>


static QString triggerTerm("移动侦测,报警输入1,报警输入2,报警输入3,报警输入4");





AddTrapTimeDialog::AddTrapTimeDialog(QWidget *parent)
    :QDialog(parent)
{

    setWindowTitle("添加布防时间段");
    QVBoxLayout *main_layout = new QVBoxLayout;

    QGroupBox *gbox_week = new QGroupBox("星期");
    QVBoxLayout *lay_week = new QVBoxLayout(gbox_week);
//    GroupBtnWidget *btn_week = new GroupBtnWidget(choices.split(","));
//    GroupChecBox *week_box = new GroupChecBox(week.split(","),"星期");
    lay_week->addWidget(new GroupBtnWidget(choices.split(",")));
    lay_week->addWidget(new GroupChecBox(weekNum.split(","),"星期"));


    main_layout->addWidget(gbox_week);

    QGroupBox *gbox_time = new QGroupBox("时间");
    QHBoxLayout *lay_time = new QHBoxLayout(gbox_time);


    LabAndWidget *start_time = new LabAndWidget("起始时间:",new QTimeEdit(QTime(0,0)));
    LabAndWidget *end_time = new LabAndWidget("终止时间:",new QTimeEdit(QTime(23,59,59)));
    LabAndWidget *warn_time = new LabAndWidget("报警持续时间(秒):",new QSpinBox,30);



    lay_time->addWidget(start_time);
    lay_time->addWidget(end_time);
    lay_time->addWidget(warn_time);

    lay_time->addStretch();

    main_layout->addWidget(gbox_time);



    QGroupBox *gbox_trigger = new QGroupBox("触发条件");
    QGridLayout *lay_trigger = new QGridLayout(gbox_trigger);
    lay_trigger->addWidget(new GroupBtnWidget(choices.split(",")),0,0,1,2);
    int i = 0;
    foreach(const QString &str,triggerTerm.split(","))
    {
        QCheckBox *cbox = new QCheckBox(str);
        lay_trigger->addWidget(cbox,1,i++);
    }
    main_layout->addWidget(gbox_trigger);
    main_layout->addWidget(new AlarmAction);

    GroupBtnWidget *btn_dialog = new GroupBtnWidget(QString("提交,取消").split(","));
    connect(btn_dialog,SIGNAL(SignalById(int)),SLOT(slot_Btn_Dialog(int)));
    main_layout->addWidget(btn_dialog);

    setLayout(main_layout);

}


void AddTrapTimeDialog::slot_Btn_Dialog(int id)
{
    id ? reject() : accept();
}
