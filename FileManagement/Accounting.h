#pragma once
#ifndef ACCOUNTING_H_
#define ACCOUNTING_H_
#include"stdafx.h"
#include "ui_Accounting.h"
extern TcpClient * tcp;    //ȫ�ֱ�����TCP ������
extern QString globalUserName;
class Accounting :public QWidget
{
	Q_OBJECT
public:
	Accounting(QWidget *parent = Q_NULLPTR);
	~Accounting();
	//void sendAccountLook(QString condition = "all");
private:
	Ui::Accounting *ui;
};
#endif // !Accounting



