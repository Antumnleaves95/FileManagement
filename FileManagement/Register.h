#pragma once
#include "stdafx.h"
#include <QtWidgets/QDialog>
#include "ui_Register.h"

extern TcpClient * tcp;
class Register : public QDialog
{
	Q_OBJECT

public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();

private:
	Ui::Register *ui;
	bool isAvailableUserName;
	bool isAvailablePassword;

	//�ۺ���
public slots:
	void Click_Register();
	void Click_Cancel();

	void checkIsAvailableUserName();		//����û����Ƿ��ظ�
	void checkIsAvailablePassword();			//�����Ƿ�һ��

	void receiveLogin();  //����login�з�����źŹ����Ĳۺ���

	void receiveDataFromServer(QString data);  //���ܴӷ��������ص�����
signals:
	void showMain();
	void showLogin();
};
