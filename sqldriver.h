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



     static int getTableCount(const QString &table_name);
     static QStringList getCameraSettings(int id);
    static  QStringList getColumnsList(const QString &table_name, const QStringList &list,
                               const QString &condition=QString(),
                               const QString &expectation=QString());

     static QStringList getColumnList(const QString &table_name, const QString &filed);
     static QString getColumnFirst(const QString &table_name,const QString &filed, const QString &condition,
                            const QString &expectation);
     static QStringList getRowList(const QString &table_name,const QString &filed,
                                      const QString &expectation);

     static void addAlarmAction(QStringList &list);
     static QStringList getRowFirst(const QString &table_name,const QString &filed,
                             const QString &expectation);

     static void deleteCamera(const QString &name);
     static void deleteRecordByCondition(const QString &table_name ,const QString &field,
                                  const QString &condition);
     static void insertItem(const QString &table, const QStringList &itemlist);
     static void updateItem(const QString &table, const QString &filed,
                            const QString &newValue, const QString &condition,
                            const QString &expectation);
     static int  getMaximumId(const QString &table_name,const QString &filed);

private:
    static QString appendSingleQuotationMarks(const QString &s);
    static  QSqlDatabase openDB();



};

#endif // SQLDRIVER_H
