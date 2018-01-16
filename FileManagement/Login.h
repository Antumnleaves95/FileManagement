#pragma once

#include"stdafx.h"
#include <QtWidgets/QDialog>
#include <QtNetwork>
#include <QMessageBox>
//#include"tcpclient.h"
#include "ui_Login.h"
#include "BaseWindow.h"

#ifndef LOGIN_H_
#define LOGIN_H_
extern TcpClient * tcp;
extern QString globalUserName ;
//����һ��ȫ�ֱ���,����stdafx��ȫ�ֱ�����˵��login����ʹ�ñ�����

class Login : public BaseWindow
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();
	void setWindowTitle(QString title, int titleFontSize = 10);  //���ñ�����

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

	void initTitleBar();

public slots:
	void Click_Login();  //�����½
	void Click_Register();  //�����ת��ע���
	//void MessageBox();
	void receiveRegister();   //����ע�����ź�
	void receiveMain();
	void raise();//��ס����
	void lower();//�Զ���¼

	void hide1();//�޸��˺ſ�����
	void receiveDataFromClient(QString data);   //���մӿͻ��˴�������Ϣ
signals: 
	void showMain();
	void showRegister();

};



#endif // LOGIN_H_

