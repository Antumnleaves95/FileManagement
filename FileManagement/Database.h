#pragma once
#ifndef DATABASE_H_
#define DATABASE_H_


#include <QObject>
//#include <QSql>  
//#include <QSqlDatabase>  
//#include <QSqlQuery>  
//#include <QDebug>  

class MySql : public QObject
{
public:
	MySql(QObject *parent = 0);

	//�������ݿ�
	bool connDB();
	//��ѯ���ݿ�
	bool queryDB(const QString &Sql);

	bool insertDB(const QString &Sql);

private:
	QString dbDriver;
	QString dbName;
	QString userName;
	QString userPwd;
	QString hostName;
	int hostPort;
};
#endif // !DATABASE_H_