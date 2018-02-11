
#include "Register.h"

#include <QMessageBox> 
#include <qdebug.h>
#include <MyMessageBox.h>
#include <QDialog> 
#include <QLabel>
#include<qpixmap.h>
#include<QRegExp>
#include <QCryptographicHash>


Register::Register(QWidget *parent)
	: BaseWindow(parent), ui(new Ui::Register)   //�������Ҫ��Ҫ�Թ��캯����ʼ����
{
	ui->setupUi(this);
	//this->setStyleSheet("background:#ffff");//����
											//this->setStyleSheet("background:D:/FileManagement-master/FileManagement/Resource/back.jpg");
	isAvailableUserName = false;     //һ��ʼ�û����ǲ����õ�
	isAvailablePassword = false;		//�����Ƿ�һ��
	isokPassword = false;//�����ʽ�Ƿ���ȷ

	setWindowFlags(Qt::FramelessWindowHint);

	//setFixedSize(340, 350);
	//this->setWindowTitle("LOGIN");
	ui->userPassword->setEchoMode(QLineEdit::Password);//����������ʱ����ʾΪ*******
	ui->userPassword2->setEchoMode(QLineEdit::Password);//����������ʱ����ʾΪ*******

	initTitleBar();
	init();

	loadStyleSheet("Register");
	//���û�������������Ƿ��ظ�
	connect(ui->userName, SIGNAL(editingFinished()), this, SLOT(checkIsAvailableUserName()));

	//��������Ƿ����Ҫ��
	connect(ui->userPassword2, SIGNAL(editingFinished()), this, SLOT(checkIsAvailablePassword()));
	connect(ui->trueName, SIGNAL(editingFinished()), this, SLOT(checkIsOkTrueName()));
	connect(ui->teacher, SIGNAL(editingFinished()), this, SLOT(checkIsOkTeacher()));


	connect(tcp, SIGNAL(sendDataToRegister(QString)), this, SLOT(receiveDataFromServer(QString)));
}

Register::~Register()
{
	delete ui;
}

void Register::initTitleBar()
{
	m_titleBar->move(1, 2);
	m_titleBar->raise();
	m_titleBar->setWindowBorderWidth(2);
	m_titleBar->setBackgroundColor(255, 255, 255);
	m_titleBar->setButtonType(MIN_BUTTON);
	m_titleBar->setTitleWidth(this->width());
}
//void Register::setWindowTitle(QString title, int titleFontSize)
//{
//	m_titleBar->setTitleContent(title, titleFontSize);
//}


void Register::init()
{

	//�����û���������
	ui->userName->setMinimumHeight(30);
	ui->userName->setTextMargins(22, 0, 0, 0);
	ui->userName->setPlaceholderText(QString::fromLocal8Bit("�������û���"));


	QLabel *userLogo = new QLabel(this);
	userLogo->setMaximumSize(21, 21);
	userLogo->setCursor(QCursor(Qt::ArrowCursor));
	userLogo->setPixmap(QPixmap("Resource/ion/user.png"));

	QSpacerItem *spaceItem1 = new QSpacerItem(150, 20, QSizePolicy::Expanding);
	QHBoxLayout *editLayout1 = new QHBoxLayout();
	editLayout1->setContentsMargins(1, 0, 1, 0);
	editLayout1->addWidget(userLogo);
	editLayout1->addSpacerItem(spaceItem1);
	ui->userName->setLayout(editLayout1);

	//��������������
	ui->userPassword->setMinimumHeight(30);
	ui->userPassword->setTextMargins(22, 0, 0, 0);

	ui->userPassword->setPlaceholderText(QString::fromLocal8Bit("����������"));

	QLabel *pwd = new QLabel(this);
	pwd->setMaximumSize(21, 21);
	pwd->setCursor(QCursor(Qt::ArrowCursor));
	pwd->setPixmap(QPixmap("Resource/ion/pwd.png"));

	QSpacerItem *spaceItem2 = new QSpacerItem(150, 20, QSizePolicy::Expanding);
	QHBoxLayout *editLayout2 = new QHBoxLayout();
	editLayout2->setContentsMargins(1, 0, 1, 0);
	editLayout2->addWidget(pwd);
	editLayout2->addSpacerItem(spaceItem2);
	ui->userPassword->setLayout(editLayout2);

	//��������ȷ����
	ui->userPassword2->setMinimumHeight(30);
	ui->userPassword2->setTextMargins(22, 0, 0, 0);

	ui->userPassword2->setPlaceholderText(QString::fromLocal8Bit("���ٴ���������"));

	QLabel *pwd2 = new QLabel(this);
	pwd2->setMaximumSize(21, 21);
	pwd2->setCursor(QCursor(Qt::ArrowCursor));
	pwd2->setPixmap(QPixmap("Resource/ion/pwd2.png"));

	QSpacerItem *spaceItem3 = new QSpacerItem(150, 20, QSizePolicy::Expanding);
	QHBoxLayout *editLayout3 = new QHBoxLayout();
	editLayout3->setContentsMargins(1, 0, 1, 0);
	editLayout3->addWidget(pwd2);
	editLayout3->addSpacerItem(spaceItem3);
	ui->userPassword2->setLayout(editLayout3);

	//������ʵ������
	ui->trueName->setMinimumHeight(30);
	ui->trueName->setTextMargins(22, 0, 0, 0);

	ui->trueName->setPlaceholderText(QString::fromLocal8Bit("��������ʵ����"));

	QLabel *trueNameLogo = new QLabel(this);
	trueNameLogo->setMaximumSize(21, 21);
	trueNameLogo->setCursor(QCursor(Qt::ArrowCursor));
	trueNameLogo->setPixmap(QPixmap("Resource/ion/truename.png"));

	QSpacerItem *spaceItem4 = new QSpacerItem(150, 20, QSizePolicy::Expanding);
	QHBoxLayout *editLayout4 = new QHBoxLayout();
	editLayout4->setContentsMargins(1, 0, 1, 0);
	editLayout4->addWidget(trueNameLogo);
	editLayout4->addSpacerItem(spaceItem4);
	ui->trueName->setLayout(editLayout4);

	//���õ�ʦ������
	ui->teacher->setMinimumHeight(30);
	ui->teacher->setTextMargins(22, 0, 0, 0);

	ui->teacher->setPlaceholderText(QString::fromLocal8Bit("�����뵼ʦ����"));
	QLabel *teacherLogo = new QLabel(this);
	teacherLogo->setMaximumSize(21, 21);
	teacherLogo->setCursor(QCursor(Qt::ArrowCursor));
	teacherLogo->setPixmap(QPixmap("Resource/ion/teacher.png"));

	QSpacerItem *spaceItem5 = new QSpacerItem(150, 20, QSizePolicy::Expanding);
	QHBoxLayout *editLayout5 = new QHBoxLayout();
	editLayout5->setContentsMargins(1, 0, 1, 0);
	editLayout5->addWidget(teacherLogo);
	editLayout5->addSpacerItem(spaceItem5);
	ui->teacher->setLayout(editLayout5);


	ui->userLogo->hide();
	ui->passLogo->hide();
	ui->passLogo2->hide();
	ui->trueNameLogo->hide();
	ui->teacherLogo->hide();
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
		QString trueName = this->ui->trueName->text();
		QString teacher = this->ui->teacher->text();
		QString level = this->ui->level->currentText();
		QString term = this->ui->term->currentText();

		QString md5;
		QByteArray qc, cc;
		QCryptographicHash md(QCryptographicHash::Md5);
		qc.append(password2);
		md.addData(qc);
		cc = md.result();
		md5.append(cc.toHex());


		QString sql = "insert into user (userName,userPassword,trueName,term,level,teacher)values('" + name + "','" +
			md5+ "','" + trueName + "','" + term + "','" + level + "','" + teacher + "')";
		QString bs = "R";
		QString data = bs + "#" + sql;
		tcp->tcpSocket->write(data.toUtf8());//����Ϣд��socket
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
		//ui->passLogo->show();
		//ui->passLogo2->show();
		//ui->passwordMessage2->hide();
		ui->passwordMessage2->setText(QString::fromLocal8Bit("����һ�£�"));
		ui->passwordMessage2->setStyleSheet("color:green;");
		QPixmap Pix("Resource/ion/correct.png");
		ui->passLogo2->setPixmap(Pix);
		ui->passLogo2->show();
	}
	else
	{
		
		qDebug() << "the password is not equal";
		//��textlabel����ʾ������Ϣ
		ui->passwordMessage2->setText(QString::fromLocal8Bit("�������벻һ�£�"));
		ui->passwordMessage2->setStyleSheet("color:red;");
		QPixmap Pix("Resource/ion/error.png");
		ui->passLogo2->setPixmap(Pix);
		ui->passLogo2->show();
	}


}

void Register::checkIsokPassword()
{
	QRegExp rx;
	QString passw=ui->userPassword->text();
	rx.setPatternSyntax(QRegExp::RegExp);
	rx.setCaseSensitivity(Qt::CaseInsensitive);//�Դ�Сд��ĸ����
	rx.setPattern(QString("^[A-Za-z]+[0-9]+$"));
	//ƥ���ʽΪ���д�Сд��ĸ��������ɵ��ַ�����λ������
	if (rx.exactMatch(passw) == true)
	{
		isokPassword = true;
		ui->passwordMessage->setText(QString::fromLocal8Bit("�����ʽ��ȷ��"));
		ui->passwordMessage->setStyleSheet("color:green;");
		ui->passwordMessage->show();

		QPixmap Pix("Resource/ion/correct.png");
		ui->passLogo->setPixmap(Pix);
		ui->passLogo->show();
	}
	else
	{
		isokPassword = false;
		ui->passwordMessage->setText(QString::fromLocal8Bit("�����������ĸΪ��λ��"));
		ui->passwordMessage->setStyleSheet("color:red;");
		ui->passwordMessage->show();

		QPixmap Pix("Resource/ion/error.png");
		ui->passLogo->setPixmap(Pix);
		ui->passLogo->show();
	}

}

void Register::checkIsOkTrueName()
{

	//��ʱ��������У����

	QPixmap Pix("Resource/ion/correct.png");
	ui->trueNameLogo->setPixmap(Pix);
	ui->trueNameLogo->show();
}
void Register::checkIsOkTeacher()
{
	QPixmap Pix("Resource/ion/correct.png");
	ui->teacherLogo->setPixmap(Pix);
	ui->teacherLogo->show();
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

		//���ô���ͼƬ
		QPixmap Pix("Resource/ion/error.png");
		ui->userLogo->setPixmap(Pix);
		ui->userLogo->show();


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
		ui->userLogo->show();
		////����UI��ʾ�û������ã�

		//����ͼ��
		QPixmap Pix("Resource/ion/correct.png");
		ui->userLogo->setPixmap(Pix);
		ui->userLogo->show();
	}
}

//�����ź�
void Register::receiveLogin()
{
	this->show();   //��ʾע�ᴰ��
}
