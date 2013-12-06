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
    explicit camera_settings(const QString &name="",QWidget *parent = 0);
    ~camera_settings();
    
private slots:
    void on_btn_search_clicked();

    void on_btn_add_clicked();

    void on_btn_add_2_clicked();

private:
    Ui::camera_settings *ui;
};

#endif // CAMERA_SETTINGS_H
