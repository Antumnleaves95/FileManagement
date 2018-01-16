#include "Login.h"

//#include "tcpclient.h"
#include <QMessageBox> 
#include "MyMessageBox.h"
#include <QMovie>
#include<qdebug.h>
#include<QDialog>
#include<QCryptographicHash>
#include <MyMessageBox.h>
//����һ��ȫ�ֱ���



Login::Login(QWidget *parent)
	: BaseWindow(parent), ui(new Ui::Login)
{
	tcp = new TcpClient();
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	
	QString kk;
	QSettings *getting = new QSettings("E:/test.ini", QSettings::IniFormat);//��ʼ����ʾ��һ�ε�¼�˻�����
	ui->nameLine->setText(getting->value("uuu", "").toString());
	kk = getting->value("ppp", "").toString();
	QString xk = getting->value("position", "").toString();
	QString xx = getting->value("auto", "").toString();

	if (xk == "true")
	{
		ui->rem_pw->setChecked(true);
	}


	QByteArray tempass = QByteArray::fromBase64(kk.toLatin1());
	ui->passwordLine->setText(tempass);

	QMovie *movie = new QMovie("Resource/Logo.gif");
	ui->label->setMovie(movie);
	movie->start();
	setFixedSize(420, 350);
	ui->passwordLine->setEchoMode(QLineEdit::Password);//����������ʱ����ʾΪ*******
	
	initTitleBar();
	if (xx == "true")
	{
		ui->auto_login->setChecked(true);
		Click_Login();
	}
	

	//TCP��ص���Ϣ,���ͻ���Ҫ�������ݸ�login
	connect(tcp, SIGNAL(sendDataToLogin(QString)), this, SLOT(receiveDataFromClient(QString)));
	//��ʾע���
	//connect(this, SIGNAL(showRegister()), &R, SLOT(receiveLogin()));
}

void Login::initTitleBar()
{
	m_titleBar->move(1, 2);
	m_titleBar->raise();
	m_titleBar->setWindowBorderWidth(2);
	m_titleBar->setBackgroundColor(255, 255, 255);
	m_titleBar->setButtonType(MIN_BUTTON);
	m_titleBar->setTitleWidth(this->width());
}

void Login::setWindowTitle(QString title, int titleFontSize)
{
	m_titleBar->setTitleContent(title, titleFontSize);
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
	 

	QString sql = "select * from user where userName = '"
		+ name + "'and userPassword ='" + passwd + "'";

	QString bs = "L";
	QString data = bs + "#" + sql;
	
	if (tcp->tcpSocket->write(data.toLatin1()))//����Ϣд��socket
		qDebug() << data;
	else
		qDebug() << "write is false";

	//������������Ӳۺ�����Ȼ����Ӧ���δ���
	//connect(tcp, SIGNAL(sendDataToLogin(QString)), this, SLOT(receiveDataFromClient(QString)));
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
		//���û������ø�ȫ�ֱ����û���
		globalUserName = this->ui->nameLine->text();
		//MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��¼�ɹ���"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
		this->hide();
		emit showMain();
	}
	else if (QString::compare(data, "F") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�û��������벻��ȷ��"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
		ui->passwordLine->clear();  //�������passwardLine
		ui->passwordLine->setFocus();  //��������¶�λ��password
	}
	else
		return;
}
void Login::hide1()
{
	ui->passwordLine->clear();
	ui->rem_pw->setChecked(false);
	ui->auto_login->setChecked(false);
}
void Login::raise()
{
	if (ui->rem_pw->isChecked())
	{
		remeberPasswd = true;//��ѡ�˼�ס���� ������д�������ļ�
		QSettings *settings = new  QSettings("E:/test.ini", QSettings::IniFormat);
		username = ui->nameLine->text();
		passwd = ui->passwordLine->text().trimmed();
		QByteArray passArray = passwd.toLatin1();

		QString password;
		password.prepend(passArray.toBase64());

		settings->setValue("uuu", username);
		settings->setValue("ppp", password);
		settings->setValue("position", remeberPasswd);
		qDebug() << password;
		delete settings;
	}
	else
	{
		//ui->passwordLine->clear();
		remeberPasswd = false;
		QSettings *setting = new  QSettings("E:/test.ini", QSettings::IniFormat);
		QString username = ui->nameLine->text();
		setting->setValue("uuu", username);
		setting->setValue("ppp", "");
		setting->setValue("position", remeberPasswd);
	}
}
void Login::lower()//�Զ���¼
{
	if (ui->auto_login->isChecked())
	{
		autologin = true;
	}
	else
	{
		autologin = false;
	}
	QSettings *settingss = new  QSettings("E:/test.ini", QSettings::IniFormat);

	settingss->setValue("auto", autologin);
}