#ifndef CAMERA_SETTINGS_H
#define CAMERA_SETTINGS_H

#include <QDialog>

namespace Ui {
class camera_settings;
}

class camera_settings : public QDialog
{
    Q_OBJECT
    
public:
    explicit camera_settings(int index=0,const QString &name="",QWidget *parent = 0);
    ~camera_settings();
    
private slots:
    void on_btn_search_clicked();

    void on_btn_add_clicked();

    void on_btn_add_2_clicked();

    void on_pushButton_15_clicked();

private:
    Ui::camera_settings *ui;
    QString m_verifyid;
};

#endif // CAMERA_SETTINGS_H
