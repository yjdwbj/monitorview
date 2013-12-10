#include "sqldriver.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>



SqlInstance::SqlInstance()
{



}


QSqlDatabase SqlInstance::opendb()
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

void SqlInstance::DeleteCamera(const QString &name)
{
    QString camera_name = GetColumnFirst("camera_settings","hostinfo_id","camera_name",name);
    DeleteRecordByCondition("hostinfo","host_id",camera_name);
    DeleteRecordByCondition("camera_settings","camera_name",name);

}





 QString SqlInstance::AppendSingleQuotationMarks(const QString &s)
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

void SqlInstance::DeleteRecordByCondition(const QString &table_name, const QString &field,
                                          const QString &expectation)
{

    QString  select("delete from ");
    select.append(table_name);
    select.append( " where ");
    select.append(field);
    select.append( " == ");
    select.append(AppendSingleQuotationMarks(expectation));
     QSqlQuery query(select,opendb());
     query.exec();
     QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

int SqlInstance::GetTableCount(const QString &table_name)
{

    int i = 0;

    QString select("select * from "+table_name);
    QSqlQuery query(select,opendb());
    query.exec();

    while(query.next())
        i++;
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return i;
}



/* select filed1,filed1,... from table where conditiopn == expectation */
QStringList SqlInstance::GetColumnsList(const QString &table_name, const QStringList &fileds,
                                       const QString &condition, const QString &expectation)
{


    QString select("select ");
    foreach(const QString &str,fileds)
    {
        select.append(str+",");
    }
   select = select.remove(select.length()-1,1);
   select.append(" from ");
   select.append(table_name);
   if(!condition.isEmpty() && !expectation.isEmpty() )
   {
       select.append(" where ");
       select.append(condition);
       select.append(" == ");
       select.append(AppendSingleQuotationMarks(expectation));
   }

   QSqlQuery query(select,opendb());

   query.exec();

   QStringList list;

   while(query.next())
   {
       QStringList r;
       for(int i = 0 ; i < fileds.count() ;i++)
       {
          r << query.value(i).toString();;
       }
       list << r.join(',');
   }

   QSqlDatabase::removeDatabase("qt_sql_default_connection");

   return list;
}

/* select id from table_name */
QStringList SqlInstance::GetColumnList(const QString &table_name, const QString &filed)
{



    QString select("select ");
    select.append(filed);
    select.append(" from ");
    select.append(table_name);
     QSqlQuery query(select,opendb());

    query.exec();
    QStringList list;
    while(query.next())
        list << query.value(0).toString();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return list;
}

QString SqlInstance::GetColumnFirst(const QString &table_name, const QString &filed,
                                    const QString &condition, const QString &expectation)
{




    QString select("select ");
    select.append(filed);
    select.append(" from ");
    select.append(table_name);
    select.append(" where ");
    select.append(condition);
    select.append(" == ");
    select.append(AppendSingleQuotationMarks(expectation));
     QSqlQuery query(select,opendb());
    query.exec();
    query.first();
    QString value = query.value(0).toString();


    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return value;
}


QStringList SqlInstance::GetRowList(const QString &table_name,const QString &filed,
                                              const QString &expectation)
{

    /* exmaple : select *  from table_name where filed == condition */



    QString select("select * from ");
    select.append(table_name);
    select.append(" where ");
    select.append(filed);
    select.append(" == ");
    select.append(AppendSingleQuotationMarks(expectation));
    QSqlQuery query(select,opendb());
    query.exec();
    QSqlRecord record = query.record();
    QStringList list;
    query.first();
    for( int i = 0 ; i  < record.count() ;i++)
    {
        list << query.value(i).toString();
    }

     QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return list;
}



void SqlInstance::InsertItem(const QString &table, const QStringList &itemlist)
{
    QString select("insert into ");
    select.append(table);
    select.append(" values (");
    int num = itemlist.count();
    for(int i = 0; i < num ; i++)
    {
        select.append("?,");
    }
    select.remove(select.length()-1,1);
    select.append(")");

     QSqlQuery query(opendb());
     query.prepare(select);
     num = 0;
     foreach(const QString &str,itemlist)
     {
         query.bindValue(num++,str);
     }

     query.exec();
     QSqlDatabase::removeDatabase("qt_sql_default_connection");

}

void SqlInstance::UpdateItem(const QString &table, const QString &filed, const QString &newValue,
                             const QString &condition, const QString &expectation)
{
    QString select("update ");
    select.append(table);
    select.append(" set ");
    select.append(filed);
    select.append(" = ? where ");
    select.append(condition);
    select.append(" == ?");
    QSqlQuery query(opendb());
    query.prepare(select);
    query.addBindValue(newValue);
    query.addBindValue(expectation);
    query.exec();
     QSqlDatabase::removeDatabase("qt_sql_default_connection");

}



