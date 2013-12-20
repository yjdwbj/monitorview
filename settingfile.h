#ifndef SETTINGFILE_H
#define SETTINGFILE_H
#include <QSettings>
#include "global.h"
#include <QPair>

class SettingFile
{
public:
    SettingFile(const QString &file);
    ~SettingFile(){ delete my_settings;}
    void setValues(const QList<iniPair> &pairlist);
private:
    QSettings *my_settings;



};


#endif // SETTINGFILE_H
