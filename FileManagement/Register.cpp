
#include "Register.h"
#include "Database.h"
//#include "Login.h"
//#include "tcpclient.h"
#include <QMessageBox> 
#include <qdebug.h>
#include <MyMessageBox.h>


Register::Register(QWidget *parent)
	: QDialog(parent), ui(new Ui::Register)   //�������Ҫ��Ҫ�Թ��캯����ʼ����
{
	ui->setupUi(this);
	isAvailableUserName = false;     //һ��ʼ�û����ǲ����õ�
	isAvailablePassword = false;		//�����Ƿ�һ��
	ui->userMessage->hide();

	//���û�������������Ƿ��ظ�
	connect(ui->userName, SIGNAL(editingFinished()), this, SLOT(checkIsAvailableUserName()));

	//��������Ƿ����Ҫ��
	connect(ui->userPassword2, SIGNAL(editingFinished()), this, SLOT(checkIsAvailablePassword()));


	connect(tcp, SIGNAL(sendDataToRegister(QString)), this, SLOT(receiveDataFromServer(QString)));
}

Register::~Register()
{
	delete ui;
}

//���ע��
void Register::Click_Register()
{
	if (!isAvailableUserName)
	{
		qDebug() << "please check your username!";
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("���ȼ���û����Ƿ���ã�"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
	}
	//m
	else if (!isAvailablePassword)
	{
		qDebug() << "the password is not same";
	}

	//���������û�����֤��ͨ�������ע��
	else if (isAvailableUserName && isAvailablePassword)
	{
		qDebug() << "get into the register";
		QString name = this->ui->userName->text();
		QString password = this->ui->userPassword->text();
		QString password2 = this->ui->userPassword2->text();
		QString sql = "insert into user (userName,userPassword)values('" + name + "','" + password + "')";
		QString bs = "R";
		QString data = bs + "#" + sql;
		tcp->tcpSocket->write(data.toLatin1());//����Ϣд��socket
		qDebug() << data;
	}
	else
		return;
}

void Register::Click_Cancel()
{
	this->close();
	emit showLogin();
}
void Register::checkIsAvailableUserName()
{
	QString name = this->ui->userName->text();
	QString sql = "select userName, userPassword from user where userName = '"
		+ name + "'";
	QString bs = "RCU";
	QString data = bs + "#" + sql;
	tcp->tcpSocket->write(data.toLatin1());//����Ϣд��socket
	qDebug() << data;
}

void Register::checkIsAvailablePassword()
{
	QString password = this->ui->userPassword->text();
	QString password2 = this->ui->userPassword2->text();


	if (QString::compare(password, password2) == 0)
	{
		isAvailablePassword = true;
		qDebug() << "the password is equal";
		//��textlabel����ʾ��ȷ��Ϣ
	}
	else
	{
		qDebug() << "the password is not equal";
		//��textlabel����ʾ������Ϣ
	}


}


void Register::receiveDataFromServer(QString data)
{
	//ע��ɹ�
	if (QString::compare(data, "R_T") == 0)
	{
		qDebug() << "Register success!";

		//��ʾ��

		this->close();
		emit showLogin();
	}
	else if (QString::compare(data, "R_F") == 0)
	{
		qDebug() << "Register failed";
	}
	//��һ�����û���
	else if (QString::compare(data, "RCU_T") == 0)
	{
		qDebug() << "it is a same username";
		//����UI��ʾ�û����ظ�
		ui->userMessage->setText(QString::fromLocal8Bit("���û����Ѵ��ڣ�"));
		ui->userMessage->setStyleSheet("color:red;");
		ui->userMessage->show();
	}
	else if (QString::compare(data, "RCU_F") == 0)
	{
		qDebug() << "good username";
		isAvailableUserName = true;
		ui->userMessage->setText(QString::fromLocal8Bit("���û������ã�"));
		ui->userMessage->setStyleSheet("color:green;");
		ui->userMessage->show();
		////����UI��ʾ�û������ã�
	}
}

//�����ź�
void Register::receiveLogin()
{
	this->show();   //��ʾע�ᴰ��
}
