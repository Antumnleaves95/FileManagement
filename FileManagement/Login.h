#pragma once

#include"stdafx.h"
#include <QtWidgets/QDialog>
#include <QtNetwork>
#include <QMessageBox>
//#include"tcpclient.h"
#include "ui_Login.h"

#ifndef LOGIN_H_
#define LOGIN_H_
extern TcpClient * tcp;
//����һ��ȫ�ֱ���

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();

private:
	Ui::Login *ui;   //��������װ��һϵ�е������֮��ģ��Ͳ����ظ�������

public slots:
	void Click_Login();  //�����½
	void Click_Register();  //�����ת��ע���
	//void MessageBox();
	void receiveRegister();   //����ע�����ź�

	void receiveDataFromClient(QString data);   //���մӿͻ��˴�������Ϣ
signals: 
	void showMain();
	void showRegister();
};



#endif // LOGIN_H_

