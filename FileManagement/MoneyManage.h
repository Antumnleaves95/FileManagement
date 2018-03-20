#pragma once
#ifndef MONEYMANAGE
#define MONEYMANAGE
#include"stdafx.h"
#include "ui_MoneyManage.h"
#include "AccountInfo.h"
extern TcpClient * tcp;    //ȫ�ֱ�����TCP ������
extern QString globalUserName;
class MoneyManage :public QWidget
{
	Q_OBJECT
public:
	MoneyManage(QWidget *parent = Q_NULLPTR);
	~MoneyManage();
	void sendAccountLook(QString condition = "all");

	void initModel();
	public slots:
	void clickAccountingButton();
	void clickLookAccount();
	void showAccount();
	void showToolTip(const QModelIndex &index);

	
private:
	Ui::MoneyManage *ui;
	QStandardItemModel *model;
};
#endif // !MONEYMANAGE



