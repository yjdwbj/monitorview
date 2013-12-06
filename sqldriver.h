#ifndef SQLDRIVER_H
#define SQLDRIVER_H
#include "global.h"

#include <QSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>

class SqlInstance
{
public:
      SqlInstance();
      ~SqlInstance(){db.close();}
//     void opendb();
     void AddHostInfo(const QStringList &list);
     int GetHostInfo();
     int GetTableCount(const QString &table_name);
     QStringList GetCameraSettings(int id)const;
     QStringList GetFieldsList(const QString &table_name, const QStringList &list);
     QStringList GetRecordByCondition(const QString &table_name, const QString &filed,
                                      const QString &expectation);

     QString GetFirstFieldByCondition(const QString &table_name,const QString &filed,
                                      const QString &condition,const QString &expectation);
//     QString GetFieldByName(const QString &table_name,const QString &filed);


     QStringList GetFieldsListByCondition(const QString &table_name, const QString &field);
     void AddCameraSetting(const QStringList &list);

     void AddAlarmAction(QStringList &list);

     void DeleteCamera(const QString &name);
     void DeleteRecordByCondition(const QString &table_name ,const QString &field,
                                  const QString &condition);
      void closedb() {db.close();}


private:
    QSqlDatabase db;
    QString AppendSingleQuotationMarks(const QString &s);


};

#endif // SQLDRIVER_H
