#include "Login.h"

#include "Database.h"
//#include "tcpclient.h"
#include <QMessageBox> 
#include <MyMessageBox.h>
#include <QDialog> 
#include <qdebug.h>
#include <QMovie>

//����һ��ȫ�ֱ���
 
Login::Login(QWidget *parent)
	: BaseWindow(parent), ui(new Ui::Login)
{
	tcp = new TcpClient();
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	QMovie *movie = new QMovie("C:/Users/44562/Desktop/Login/Login/Resources/timg.gif");
	ui->label->setMovie(movie);
	movie->start();
	setFixedSize(420, 350);
	ui->passwordLine->setEchoMode(QLineEdit::Password);//����������ʱ����ʾΪ*******
	initTitleBar();
	connect(tcp, SIGNAL(sendDataToLogin(QString)), this, SLOT(receiveDataFromClient(QString)));
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


	QString sql = "select userName, userPassword from user where userName = '"
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