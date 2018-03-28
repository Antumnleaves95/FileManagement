#pragma once
#ifndef DATABASE_H_
#define DATABASE_H_
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Database
{
public:
	Database();
	~Database();

	//�������ݿ�
	void connDB();

	//����QStringList�Ĳ�ѯ���
	static QList<QStringList> queryDB(const QString &Sql,int size);  
	//����
	static bool insertDB(const QString &Sql);

	void createTable();

	//static bool deleteDB()
};

#endif // !DATABASE_H_


