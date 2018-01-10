#include "Login.h"

#include "Database.h"
//#include "tcpclient.h"
#include <QMessageBox> 
#include <MyMessageBox.h>
#include <QDialog> 
#include <qdebug.h>
#include<QCryptographicHash>

//����һ��ȫ�ֱ���
 
Login::Login(QWidget *parent)
	: QDialog(parent), ui(new Ui::Login)
{
	tcp = new TcpClient();
	ui->setupUi(this);
	QString kk;
	QString xk;
	QString md5;
	QByteArray bb;

	QSettings *getting = new QSettings("E:/test.ini", QSettings::IniFormat);//��ʼ����ʾ��һ�ε�¼�˻�����
	ui->nameLine->setText(getting->value("uname", "20").toString());
	//kk = getting->value("pws", 1000).toString();

	//bb = QCryptographicHash::hash(kk.toLatin1(), QCryptographicHash::Md5);
	//md5.append(bb.toHex());
	//bb.append(kk);
	//QCryptographicHash hash(QCryptographicHash::Md5);
	//hash.addData(bb);//������ݵ����ܹ�ϣֵ
	//QByteArray result = hash.result();//�������յĹ�ϣֵ
	//xk = result.toHex();

	ui->passwordLine->setText(getting->value("pws",20).toString());
	
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
	
	if (tcp->tcpSocket->write(data.toLatin1()))//����Ϣд��socket
		qDebug() << data;
	else
		qDebug() << "write is false";
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
void Login::close()
{
	QApplication* app;
	app->quit();
}

void Login::raise()//��ס����
{

	if (ui->rem_pw->isChecked())
	{
		remeberPasswd = true;//��ѡ�˼�ס���� ������д�������ļ�


		QString  md5;
		QByteArray  ba, bb;
		QCryptographicHash md(QCryptographicHash::Md5);
		QSettings *settings = new  QSettings("E:/test.ini", QSettings::IniFormat);
		username = ui->nameLine->text();
		passwd = ui->passwordLine->text();
		//ba.append(passwd);
		//md.addData(ba);
		//bb = md.result();
		//md5.append(bb.toHex());


		settings->setValue("uname", username);
		settings->setValue("pws", passwd);
		delete settings;
	}
	else
	{
		ui->passwordLine->clear();
		remeberPasswd = false;
	}

}