#ifndef ALARMACTION_H
#define ALARMACTION_H

#include "../global.h"
#include <QStackedLayout>


class AlarmAction : public QGroupBox
{
    Q_OBJECT


public:

    explicit AlarmAction(QWidget *parent = 0);
    ~AlarmAction(){ }
private slots:
    void slot_ActionHasTriggered(int);
    void slot_ItemHasClicked(QModelIndex);
private:
    QSignalMapper *signalmap;
    QStackedLayout *lay_stack;
    QHBoxLayout *main_layout;
    QTableWidget *tabWidgets ;



    QWidget* WidgetFromLayout(const QString &name, QLayout *lay);

};

#endif // ALARMACTION_H
