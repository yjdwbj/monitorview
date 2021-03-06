#include "setting_trap_time.h"
#include "alarmaction.h"
#include <QTimeEdit>
#include <QSpinBox>
#include <QGraphicsLayout>


static QString triggerTerm("移动侦测,报警输入1,报警输入2,报警输入3,报警输入4");


RecordTime::RecordTime(QWidget *parent)
    :QDialog(parent)

{

    setWindowFlags(Qt::WindowStaysOnTopHint);
    QVBoxLayout *main_layout = new QVBoxLayout;
    QGroupBox *gbox_week = new QGroupBox("星期");
    QVBoxLayout *lay_week = new QVBoxLayout(gbox_week);

    GroupBtnWidget *w = new GroupBtnWidget(choices.split(","));
    lay_week->addWidget(w);
    cboxlist = new GroupCheckBox(weekNum.split(","),"星期");
    connect(w,SIGNAL(SignalById(int)),cboxlist,SLOT(slot_ToggleAll(int)));

    lay_week->addWidget(cboxlist);




    QGroupBox *gbox_time = new QGroupBox("时间");
    QHBoxLayout *lay_time = new QHBoxLayout(gbox_time);


    LabAndWidget *start_time = new LabAndWidget("起始时间:",new QTimeEdit(QTime(0,0)),LabAndWidget::Vertical);
    LabAndWidget *end_time = new LabAndWidget("终止时间:",new QTimeEdit(QTime(23,59,59)),LabAndWidget::Vertical);



    lay_time->addWidget(start_time);
    lay_time->addWidget(end_time);
    main_layout->addWidget(gbox_week);
    main_layout->addWidget(gbox_time);
    GroupBtnWidget *yorn =  new GroupBtnWidget(yesorno.split(","));
    main_layout->addWidget(yorn);
    connect(yorn,SIGNAL(SignalById(int)),SLOT(slot_signalbyid(int)));
    setLayout(main_layout);

}

void RecordTime::slot_signalbyid(int id)
{
    id ? reject() : accept();
}





AddTrapTimeDialog::AddTrapTimeDialog(QWidget *parent)
    :QDialog(parent)
{

    setStyleSheet("QPushButton {width: 18px;font: bold 16px;}"
                  "QPushButton::hover{ background: gray;font: bold 18px;width: 14px;}"
                  "QGroupBox {font:  13px;}");
    setWindowFlags(Qt::WindowStaysOnTopHint| Qt::WindowCloseButtonHint);
    setWindowTitle("添加布防时间段");
    QVBoxLayout *main_layout = new QVBoxLayout;

    QGroupBox *gbox_week = new QGroupBox("星期");

    QVBoxLayout *lay_week = new QVBoxLayout(gbox_week);
    lay_week->setSpacing(3);
    lay_week->setMargin(2);
//    GroupBtnWidget *btn_week = new GroupBtnWidget(choices.split(","));
//    GroupChecBox *week_box = new GroupChecBox(week.split(","),"星期");
    GroupBtnWidget *AllorNone = new GroupBtnWidget(choices.split(","));
    lay_week->addWidget(AllorNone);
    GroupCheckBox *week = new GroupCheckBox(weekNum.split(","),"星期");
    lay_week->addWidget(week);
    connect(AllorNone,SIGNAL(SignalById(int)),week,SLOT(slot_ToggleAll(int)));


    main_layout->addWidget(gbox_week);

    QGroupBox *gbox_time = new QGroupBox("时间");
    QHBoxLayout *lay_time = new QHBoxLayout(gbox_time);
    lay_time->setSpacing(3);



    LabAndWidget *start_time = new LabAndWidget("起始时间:",new QTimeEdit(QTime(0,0)));
    LabAndWidget *end_time = new LabAndWidget("终止时间:",new QTimeEdit(QTime(23,59,59)));
    LabAndWidget *warn_time = new LabAndWidget("报警持续时间(秒):",new QSpinBox,30);



    lay_time->addWidget(start_time);
    lay_time->addWidget(end_time);
    lay_time->addWidget(warn_time);

    lay_time->addStretch();

    main_layout->addWidget(gbox_time);



    QGroupBox *gbox_trigger = new QGroupBox("触发条件");
    gbox_trigger->adjustSize();
    QGridLayout *lay_trigger = new QGridLayout(gbox_trigger);
    lay_trigger->setMargin(2);
    lay_trigger->setSpacing(3);
    GroupBtnWidget *trigAllOrNone = new GroupBtnWidget(choices.split(","));
    lay_trigger->addWidget(trigAllOrNone,0,0,1,2);
    int i = 0;
     GroupCheckBox *trigger = new GroupCheckBox(triggerTerm.split(","));
     connect(trigAllOrNone,SIGNAL(SignalById(int)),trigger,SLOT(slot_ToggleAll(int)));

     lay_trigger->addWidget(trigger);
    main_layout->addWidget(gbox_trigger);
    main_layout->addWidget(new AlarmAction);

    GroupBtnWidget *btn_dialog = new GroupBtnWidget(QString("提交,取消").split(","));
    connect(btn_dialog,SIGNAL(SignalById(int)),SLOT(slot_Btn_Dialog(int)));
    main_layout->addWidget(btn_dialog);
    this->setFixedWidth(500);
    setLayout(main_layout);


}


void AddTrapTimeDialog::slot_Btn_Dialog(int id)
{
    id ? reject() : accept();
}
