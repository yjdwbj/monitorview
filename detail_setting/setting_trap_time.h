#ifndef SETTING_TRAP_TIME_H
#define SETTING_TRAP_TIME_H
#include "../global.h"



class AddTrapTimeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddTrapTimeDialog(QWidget *parent = 0);
    ~AddTrapTimeDialog(){}
private slots:
    void slot_Btn_Dialog(int id);

};


#endif // SETTING_TRAP_TIME_H
