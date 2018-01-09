#include "Login.h"

#include "Database.h"
//#include "tcpclient.h"
#include <QMessageBox> 
#include <MyMessageBox.h>
#include <QDialog> 
#include <qdebug.h>

//����һ��ȫ�ֱ���
 
Login::Login(QWidget *parent)
	: QDialog(parent), ui(new Ui::Login)
{
	tcp = new TcpClient();
	ui->setupUi(this);
	ui->passwordLine->setEchoMode(QLineEdit::Password);//����������ʱ����ʾΪ*******
}

Login::~Login()
{
	delete ui;
}

//�����¼,����ʵʱͨ�ţ�Ҫ�öԻ����������ܷ�����Ϣ��
void Login::Click_Login()
{
	QString name = this->ui->nameLine->text();
	QString passwd = this->ui->passwordLine->text();


	QString sql = "select userName, userPassword from user where userName = '"
		+ name + "'and userPassword ='" + passwd + "'";

	QString bs = "L";
	QString data = bs + "#" + sql;
	
	tcp->tcpSocket->write(data.toLatin1());//����Ϣд��socket
	qDebug() << data;

	connect(tcp, SIGNAL(sendDataToLogin(QString)), this, SLOT(receiveDataFromClient(QString)));
	//MessageBox();
}



//��ת��ע�ᴰ��
void Login::Click_Register()
{
	this->hide();			//���ε�¼����
	emit showRegister();	//�����źţ����źŴ��͵��ض�ҳ��
}

void Login::receiveRegister()
{
	this->show();  //��ʾ��¼����
}

//��֤��¼���ӷ���˵���������
void Login::receiveDataFromClient(QString data)
{
	//��֤�ɹ���
	if (QString::compare(data, "T") == 0)
	{
		//MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��¼�ɹ���"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
		this->hide();
		emit showMain();
	}
	else if (QString::compare(data, "F") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�û��������벻����ȷ��"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
		ui->passwordLine->clear();  //�������passwardLine
		ui->passwordLine->setFocus();  //��������¶�λ��password
	}
	else
		return;
}