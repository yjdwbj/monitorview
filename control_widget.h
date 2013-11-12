#ifndef CONTROL_WIDGET_H
#define CONTROL_WIDGET_H
#include "global.h"
#include <QRegion>
#include <QBitmap>
class GuideButton : public QWidget
{
    Q_OBJECT
public:
    explicit GuideButton(QWidget *parent=0);
    ~GuideButton(){}
signals:
    void signals_to_id(int);
private:
    QSignalMapper *smap;
protected:
    void paintEvent(QPaintEvent *);
};

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);
    ~ControlWidget();
    void startTimer(){timer->start(5000);
                      show();}
private slots:
    void slot_TimerOut();
private:
    QTimer *timer;
    QGridLayout *main_layout;
};

#endif // CONTROL_WIDGET_H
