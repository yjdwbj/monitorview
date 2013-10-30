#include "addnewcamera.h"
#include <QPushButton>

static QString camera_type ="F,H,T,E,Q,PnP,V";

SetCameraType::SetCameraType(QDialog *parent)
    :QDialog(parent)
{
    setWindowTitle("选择摄像机型号");
    QGridLayout *main_lay = new QGridLayout;





    QGroupBox *gbox_type = new QGroupBox("摄像机类型：");

    gbox_type->setFixedWidth(300);
    QVBoxLayout *lay_type = new QVBoxLayout(gbox_type);
//    QStringList list_type = camera_type.split(",");

    foreach(const QString &s, camera_type.split(","))
    {
        QRadioButton *box = new QRadioButton(s+tr(" 系列"));
        lay_type->addWidget(box);
    }
    main_lay->addWidget(gbox_type,0,0,1,3);

    QPushButton *btn_ok = new QPushButton("提交");
    connect(btn_ok,SIGNAL(clicked()),SLOT(accept()));
    QPushButton *btn_cancel = new QPushButton("取消");
    connect(btn_cancel,SIGNAL(clicked()),SLOT(reject()));
    main_lay->addItem(new QSpacerItem(5,10),1,0);
    main_lay->addWidget(btn_ok,1,1);
    main_lay->addWidget(btn_cancel,1,2);
    setLayout(main_lay);
    show();
}


AddNewCamera::AddNewCamera(QWidget *parent)
    :QWidget(parent)
{

}
