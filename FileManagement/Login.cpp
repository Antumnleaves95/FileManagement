#include "Login.h"
#include <QMessageBox> 
Login::Login(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

//�����¼
void Login::Click_Login()
{
	QMessageBox msg;
	msg.setText("hello world!");
	msg.exec();
}

//��ת��ע�ᴰ��
void Login::Click_Register()
{
	this->hide();			//���ε�¼����
	emit showRegister();	//�����źţ����źŴ��͵��ض�ҳ��
}