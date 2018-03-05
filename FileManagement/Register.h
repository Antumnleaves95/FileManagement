#pragma once
#include "stdafx.h"
#include <QtWidgets/QDialog>
#include "ui_Register.h"
#include "BaseWindow.h"

extern TcpClient * tcp;
class Register : public BaseWindow
{
	Q_OBJECT

public:
	Register(QWidget *parent = Q_NULLPTR);
	~Register();
	//void setWindowTitle(QString title, int titleFontSize = 10);  //���ñ�����

private:
	Ui::Register *ui;
	bool isAvailableUserName;
	bool isAvailablePassword;
	bool isAvailableStudentId;
	bool isokPassword;
	bool isOkTrueName;
	bool isOkTeacher;
	//void initTitleBar();  //��ʼ��������
	void init();

	//�ۺ���
public slots:
	void Click_Register();
	void Click_Cancel();

	void checkIsAvailableUserName();		//����û����Ƿ��ظ�
	void checkIsAvailableStudentId();    //���ѧ���Ƿ��ظ�
	void checkIsAvailablePassword();			//�����Ƿ�һ��
	void checkIsokPassword();//�����ʽ�Ƿ���ȷ


	void checkIsOkTrueName();
	void checkIsOkTeacher();

	void receiveLogin();  //����login�з�����źŹ����Ĳۺ���

	void receiveDataFromClient(QString data);  //���ܴӷ��������ص�����

signals:
	void showMain();
	void showLogin();
};
