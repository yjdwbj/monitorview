#include "sqldriver.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>



SqlInstance::SqlInstance()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(qApp->applicationDirPath()+"/camera.db");

    if(!db.open())
    {
        QMessageBox(QMessageBox::Critical,QString("数据库错误"),QString("不能打开数据库!"));
        qDebug() << QString("不能打开数据库!");

        return;
    }
}


//void SqlInstance::opendb()
//{

//}

//void SqlInstance::closedb()
//{
//    db.close();
//}

void SqlInstance::DeleteCamera(const QString &name)
{
//    QSqlQuery query(db);
//    QString  select("delete from hostinfo where host_id == "
//                    "(select hostinfo_id from camera_settings where camera_name == '"+name+"')");
//    query.exec(select);
    QString camera_name = GetFirstFieldByCondition("camera_settings","hostinfo_id","camera_name",name);
    DeleteRecordByCondition("hostinfo","host_id",camera_name);

//    select= "delete  from camera_settings where camera_name == '"+name+"'";

    DeleteRecordByCondition("camera_settings","camera_name",name);


//    query.exec(select);
//    err =  query.lastError();
//    err =  query.lastError();

}

QString SqlInstance::GetFirstFieldByCondition(const QString &table_name, const QString &filed,
                                              const QString &condition,const QString &expectation)
{


        QSqlQuery query(db);
        QString  select("select ");
        select.append(filed);
        select.append(" from ");
        select.append(table_name);
        select.append( " where ");
        select.append(condition);
        select.append( " == ");

        select.append(AppendSingleQuotationMarks(expectation));
        query.exec(select);
        query.first();
        return query.value(0).toString();

}

//QString SqlInstance::GetFieldByName(const QString &table_name, const QString &filed)
//{
//    QSqlQuery query(db);
//    QString  select("select ");
//    select.append(filed);
//    select.append(" from ");
//    select.append(table_name);

//    query.exec(select);
//    query.first();
//    return query.value(0).toString();
//}


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



    QSqlQuery query(db);
    QString  select("delete from ");
    select.append(table_name);
    select.append( " where ");
    select.append(field);
    select.append( " == ");
    select.append(AppendSingleQuotationMarks(expectation));
    query.exec(select);
}

int SqlInstance::GetTableCount(const QString &table_name)
{
    int i = 0;
    QSqlQuery query(db);
    QString select("select * from "+table_name);
    query.prepare(select);
    query.exec(select);
    while(query.next())
        i++;

    return i;
}


QStringList SqlInstance::GetFieldsListByCondition(const QString &table_name, const QString &field)
{
    QSqlQuery query(db);
    QString select("select ");
    select.append(field);
    select.append( " from ");
    select.append(table_name);
    query.exec(select);
    QStringList list;
    while(query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}


QStringList SqlInstance::GetFieldsList(const QString &table_name,const QStringList &list)
{
    QSqlQuery query(db);
    QString select("select ");
    foreach(const QString &str,list)
    {
        select.append("str,");
    }
   select = select.remove(select.length()-1);
   select.append(" from ");
   select.append(table_name);
   query.exec(select);
   query.first();

   QStringList r;
   for(int i = 0 ; i < list.count() ;i++)
   {
       r << query.value(i).toString();
   }


   return r;
}




QStringList SqlInstance::GetRecordByCondition(const QString &table_name,const QString &filed,
                                              const QString &expectation)
{

    /* exmaple : select *  from table_name where filed == condition */
    QSqlQuery query(db);

    QString select("select * from ");
    select.append(table_name);
    select.append(" where ");
    select.append(filed);
    select.append(" == ");
    select.append(AppendSingleQuotationMarks(expectation));
    query.exec(select);
    QSqlRecord record = query.record();
    QStringList list;
    query.first();
    for( int i = 0 ; i  < record.count() ;i++)
    {
        list << query.value(i).toString();
    }
    return list;
}



void SqlInstance::AddCameraSetting(const QStringList &list)
{

    if(!db.isOpen())
        return;
    QStringList cameraset ;
    QStringList ll(list);
    for(int i = 0 ; i < ll.count() ;i++)
    {
        QString s = list.at(i);
        s.prepend('\'').append('\'');
        ll.replace(i,s);
    }
    cameraset << ll.at(0) << ll.at(1) << ll.at(0) << ll.at(3)
              << "'admin'" << "'admin'" << "'0'" << "'0'" << "''" << "''" << "''" << "''"
                 << ll.at(0) << ll.at(0);
    QString select("insert into camera_settings values (" +cameraset.join(",")+")");
    QSqlQuery query(db);
    query.exec(select);
    QStringList hostinfo;

    hostinfo << ll.at(0)  << ll.at(2) << ll.at(4) << ll.at(5)
             << ll.at(6) << "''" << ll.at(7) ;
    AddHostInfo(hostinfo);
}

void SqlInstance::AddHostInfo(const QStringList &list)
{
    QSqlQuery query(db);
    QString select("insert into hostinfo values (" +list.join(",")+")");

    query.exec(select);

}



