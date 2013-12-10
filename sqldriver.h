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



     static int GetTableCount(const QString &table_name);
     static QStringList GetCameraSettings(int id);
    static  QStringList GetColumnsList(const QString &table_name, const QStringList &list,
                               const QString &condition=QString(),
                               const QString &expectation=QString());

     static QStringList GetColumnList(const QString &table_name, const QString &filed);
     static QString GetColumnFirst(const QString &table_name,const QString &filed, const QString &condition,
                            const QString &expectation);
     static QStringList GetRowList(const QString &table_name,const QString &filed,
                                      const QString &expectation);

     static void AddAlarmAction(QStringList &list);

     static void DeleteCamera(const QString &name);
     static void DeleteRecordByCondition(const QString &table_name ,const QString &field,
                                  const QString &condition);
     static void InsertItem(const QString &table, const QStringList &itemlist);
     static void UpdateItem(const QString &table, const QString &filed,
                            const QString &newValue, const QString &condition,
                            const QString &expectation);

private:
    static QString AppendSingleQuotationMarks(const QString &s);
    static  QSqlDatabase opendb();



};

#endif // SQLDRIVER_H
