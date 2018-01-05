#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include "Database.h"  

MySql::MySql(QObject *parent)
	:QObject(parent)
{
	dbDriver = "QMYSQL";
	dbName = "filemanagement";
	userName = "root";
	userPwd = "123456";
	hostName = "127.0.0.1";
	hostPort = 5432;
	connDB();
}

bool MySql::connDB()
{
		QSqlDatabase db = QSqlDatabase::addDatabase(dbDriver);//�������
		db.setHostName(hostName);//����������
		db.setDatabaseName(dbName);//�������ݿ���
		db.setUserName(userName);//�����û���
		db.setPassword(userPwd);//�����û�����

		if (db.open())
		{
			qDebug() << "Database connected successfully!";
			return true;
		}
		else
		{
			qDebug() << "Database connected failed!";
			return false;
		}
}

bool MySql::queryDB(const QString &Sql)
{
	QSqlQuery query;
	query.exec(Sql);
	if (query.next())
		//����м�¼��Ϊ�� �����˳��ж�
	{
		qDebug() << query.value(0).toString();
		return true;
	}
	return false;
}


//void MySql::createtable()
//{
//	query = new QSqlQuery;
//
//	query->exec("create table user(name VARCHAR(30) PRIMARY KEY UNIQUE NOT NULL,passward VARCHAR(30))");
//
//	/*����root�û�*/
//	query->exec("insert into user value('root', 'root')");
//}
//
//
//bool MySql::loguser(QString name, QString passward)
//{
//	QString str = QString("select * from user where name='%1' and passward='%2'").arg(name).arg(passward);
//	query = new QSqlQuery;
//	query->exec(str);
//	query->last();
//	int record = query->at() + 1;
//	if (record == 0)
//		return false;
//	return true;
//}
//
//
//bool MySql::signup(QString name, QString passward)
//{
//	QString str = QString("select * from user where name='%1").arg(name);
//	query = new QSqlQuery;
//	query->exec(str);
//	query->last();
//	int record = query->at() + 1;
//	if (record != 0)
//		return false;
//	str = QString("insert into user value('%1','%2')").arg(name).arg(passward);
//	bool ret = query->exec(str);
//	return ret;
//}