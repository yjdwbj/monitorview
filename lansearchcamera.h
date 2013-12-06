#ifndef LANSEARCHCAMERA_H
#define LANSEARCHCAMERA_H

#include <QDialog>

namespace Ui {
class LanSearchCamera;
}

class LanSearchCamera : public QDialog
{
    Q_OBJECT
    
public:
    explicit LanSearchCamera(QWidget *parent = 0);
    ~LanSearchCamera();
    
private slots:
    void on_pushButton_2_clicked();

private:
    Ui::LanSearchCamera *ui;
};

#endif // LANSEARCHCAMERA_H
