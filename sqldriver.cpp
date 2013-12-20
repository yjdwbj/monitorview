#include "sqldriver.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>



SqlInstance::SqlInstance()
{



}


QSqlDatabase SqlInstance::openDB()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("QSQLITE"))
        db = QSqlDatabase::database("QSQLITE");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");

        db.setDatabaseName(qApp->applicationDirPath()+"/camera.db");
    if(!db.isOpen())
    {
        db.open();
    }
    return db;
}

void SqlInstance::deleteCamera(const QString &name)
{
    QString camera_name = getColumnFirst("camera_settings","hostinfo_id","camera_name",name);
    deleteRecordByCondition("hostinfo","host_id",camera_name);
    deleteRecordByCondition("camera_settings","camera_name",name);

}





 QString SqlInstance::appendSingleQuotationMarks(const QString &s)
{
    QString c(s);
    if(!c.startsWith('\''))
    {
        c.prepend('\'');
    }

    if(!c.endsWith('\''))
        c.append('\'');
    return c;
}

void SqlInstance::deleteRecordByCondition(const QString &table_name, const QString &field,
                                          const QString &expectation)
{

    QString  select("delete from ");
    select.append(table_name);
    select.append( " where ");
    select.append(field);
    select.append( " == ");
    select.append(appendSingleQuotationMarks(expectation));
     QSqlQuery query(select,openDB());
     query.exec();
     QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

int SqlInstance::getTableCount(const QString &table_name)
{
    int i = 0;
    QString select("select * from "+table_name);
    QSqlQuery query(select,openDB());
    query.exec();
    while(query.next())
        i++;
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return i;
}

int SqlInstance::getMaximumId(const QString &table_name,const QString &filed)
{
    /* SELECT row from table ORDER BY id DESC LIMIT 1;*/
    QString select("select ");
    select.append(filed);
    select.append(" from ");
    select.append(table_name);
    select.append(" ORDER BY ");
    select.append(filed);
    select.append(" DESC LIMIT 1");
    QSqlQuery query(select,openDB());
    query.exec();
    query.first();
    int n = query.value(0).toString().isEmpty() ? -1 : query.value(0).toInt();
     QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return n;
}

int SqlInstance::getMinimumId(const QString &table_name, const QString &filed)
{
    /* SELECT row from table ORDER BY id DESC LIMIT 1;*/
    QString select("select ");
    select.append(filed);
    select.append(" from ");
    select.append(table_name);
    select.append(" ORDER BY ");
    select.append(filed);
    select.append(" ASC LIMIT 1");
    QSqlQuery query(select,openDB());
    query.exec();
    query.first();
    int n = query.value(0).toString().isEmpty() ? -1 : query.value(0).toInt();
     QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return n;
}


/* select filed1,filed1,... from table where conditiopn == expectation */
QStringList SqlInstance::getColumnsList(const QString &table_name, const QStringList &fileds,
                                       const QString &condition, const QString &expectation)
{


    QString select("select ");

    select.append(fileds.join(','));
   select.append(" from ");
   select.append(table_name);
   if(!condition.isEmpty() && !expectation.isEmpty() )
   {
       select.append(" where ");
       select.append(condition);
       select.append(" == ");
       select.append(appendSingleQuotationMarks(expectation));
   }

   QSqlQuery query(select,openDB());

   query.exec();

   QStringList list;

   while(query.next())
   {
       QStringList r;
       int count = fileds.count();
       for(int i = 0 ; i < count ;i++)
       {
          r << query.value(i).toString();;
       }
       list << r.join(',');
   }

   QSqlDatabase::removeDatabase("qt_sql_default_connection");

   return list;
}

/* select id from table_name */
QStringList SqlInstance::getColumnList(const QString &table_name, const QString &filed)
{



    QString select("select ");
    select.append(filed);
    select.append(" from ");
    select.append(table_name);
     QSqlQuery query(select,openDB());

    query.exec();
    QStringList list;
    while(query.next())
        list << query.value(0).toString();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return list;
}

QString SqlInstance::getColumnFirst(const QString &table_name, const QString &filed,
                                    const QString &condition, const QString &expectation)
{




    QString select("select ");
    select.append(filed);
    select.append(" from ");
    select.append(table_name);
    select.append(" where ");
    select.append(condition);
    select.append(" == ");
    select.append(appendSingleQuotationMarks(expectation));
     QSqlQuery query(select,openDB());
    query.exec();
    query.first();
    QString value = query.value(0).toString();


    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return value;
}


QStringList SqlInstance::getRowFirst(const QString &table_name,const QString &filed,
                                     const QString &expectation)
{
    QString select("select * from ");
    select.append(table_name);
    select.append(" where ");
    select.append(filed);
    select.append(" == ?");
    QSqlQuery query(openDB());
    query.prepare(select);
    query.addBindValue(expectation);
    query.exec();
    QSqlRecord record = query.record();
    QStringList list;
    query.first();
    int count = record.count();
    for(int i = 0 ; i < count ;i++)
    {
        list << query.value(i).toString();
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
   return list;
}


QStringList SqlInstance::getRowList(const QString &table_name,const QString &filed,
                                              const QString &expectation)
{

    /* exmaple : select *  from table_name where filed == condition */

    QString select("select * from ");
    select.append(table_name);
    select.append(" where ");
    select.append(filed);
    select.append(" == ?");
    QSqlQuery query(openDB());
    query.prepare(select);
    query.addBindValue(expectation);
    query.exec();
    QSqlRecord record = query.record();
    QStringList list;
    while(query.next())
    {
        QStringList t;
        int count = record.count();

        for(int i = 0 ; i < count ;i++)
        {
            t << query.value(i).toString();
        }
        list << t.join(',');
    }


     QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return list;
}



QStringList SqlInstance::QuerySqlFromString(const QString &sql,const QStringList &bindVaule)
{
      QSqlQuery query(openDB());
      query.prepare(sql);
      int n = 0;
      foreach(const QString &str,bindVaule)
      {
          query.bindValue(n++,str);
      }

      query.exec();
      QSqlRecord record = query.record();
      QStringList t;
      query.first();

      int count = record.count();
      for(int i = 0 ; i < count ;i++)
      {
              t << query.value(i).toString();
          }

      QSqlDatabase::removeDatabase("qt_sql_default_connection");
      return t;
}


void SqlInstance::insertItem(const QString &table, const QStringList &itemlist)
{
    QString select("insert into ");
    select.append(table);
    select.append(" values (");

    int num = itemlist.size();
    QStringList l;
    for(int i = 0; i < num ; i++)
    {
        l << "?";
    }
    select.append(l.join(','));
    select.append(")");

     QSqlQuery query(openDB());
     query.prepare(select);
     num = 0;
     foreach(const QString &str,itemlist)
     {
         query.bindValue(num++,str);
     }

     query.exec();
//     QSqlError err = query.lastError();
     QSqlDatabase::removeDatabase("qt_sql_default_connection");

}

void SqlInstance::updateItem(const QString &table, const QString &filed, const QString &newValue,
                             const QString &condition, const QString &expectation)
{
    QString select("update ");
    select.append(table);
    select.append(" set ");
    select.append(filed);
    select.append(" = ? where ");
    select.append(condition);
    select.append(" == ?");
    QSqlQuery query(openDB());
    query.prepare(select);
    query.addBindValue(newValue);
    query.addBindValue(expectation);
    query.exec();
     QSqlDatabase::removeDatabase("qt_sql_default_connection");

}



