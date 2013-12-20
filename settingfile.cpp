#include "settingfile.h"

SettingFile::SettingFile(const QString &file)
{
    my_settings = new QSettings(file,QSettings::IniFormat);
}

void SettingFile::setValues(const QList<iniPair> &pairlist)
{
    foreach(const iniPair &p,pairlist)
    {
        my_settings->setValue(p.first,p.second);
    }
    my_settings->sync();
}
