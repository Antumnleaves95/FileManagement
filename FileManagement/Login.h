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

	QString     username;
    QString    passwd;
	QString     AutoLogin;
	QString     RemeberPasswd;

	bool        remeberPasswd;
	bool        autologin;
	void        savecfg();
	void        loadcfg();


	public slots:
	void Click_Login();  //�����½
	void Click_Register();  //�����ת��ע���
	//void MessageBox();
	void close();//����˳�����
	void receiveRegister();   //����ע�����ź�
	void raise();//��ѡ��ס�����
	void receiveDataFromClient(QString data);   //���մӿͻ��˴�������Ϣ
	void lower();//��ѡ�Զ���¼


signals: 
	void showMain();
	void showRegister();
};



#endif // LOGIN_H_

