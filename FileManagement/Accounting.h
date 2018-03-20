#pragma once
#ifndef ACCOUNTING_H_
#define ACCOUNTING_H_
#include"stdafx.h"
#include "BaseWindow.h"
#include "ui_Accounting.h"

#include "AccountInfo.h"
extern TcpClient * tcp;    //ȫ�ֱ�����TCP ������
extern QString globalUserName;
class Accounting :public BaseWindow
{
	Q_OBJECT
public:
	Accounting(QWidget *parent = Q_NULLPTR);
	~Accounting();
	//void sendAccountLook(QString condition = "all");
	void sendIncomeToServer();
	void sendOutcomeToServer();
	

public slots:
void clickIncomeButton();
void clickOutcomeButton();
void receiveDataFromClient(QString data);


private:
	Ui::Accounting *ui;
};
#endif // !Accounting 



