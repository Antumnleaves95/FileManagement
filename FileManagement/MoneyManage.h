#pragma once
#ifndef MONEYMANAGE
#define MONEYMANAGE
#include"stdafx.h"
#include "ui_MoneyManage.h"
extern TcpClient * tcp;    //ȫ�ֱ�����TCP ������
extern QString globalUserName;
class MoneyManage :public QWidget
{
	Q_OBJECT
public:
	MoneyManage(QWidget *parent = Q_NULLPTR);
	~MoneyManage();
	void sendAccountLook(QString condition = "all");
private:
	Ui::MoneyManage *ui;
};
#endif // !MONEYMANAGE



