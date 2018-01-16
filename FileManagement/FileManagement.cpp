#include "FileManagement.h"

#include "DownloadFile.h"
#include <QMessageBox> 
#include <QLabel>
#include <QMovie>
extern QString globalUserName;
FileManagement::FileManagement(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::FileManagement)
{
	ui->setupUi(this);

	QIcon personico("Resource/person1.png"); //����QIcon����
	ui->personButton->setIcon(personico); //��ͼƬ���õ���ť��
	ui->personButton->setIconSize(QSize(70, 58));//����ʵ�ʵ���ͼƬ��С
	ui->personButton->setStyleSheet("border:none");   //����button�ı߿���(true);
	//toolbutton���ͼƬ
	QIcon uploadico("Resource/upload.png"); //����QIcon����
	ui->uploadButton->setIcon(uploadico); //��ͼƬ���õ���ť��
	ui->uploadButton->setIconSize(QSize(70, 55));//����ʵ�ʵ���ͼƬ��С
	ui->uploadButton->setStyleSheet("border:none");   //����button�ı߿���(true);
	QIcon downloadico("Resource/download.png"); //����QIcon����
	ui->downloadButton->setIcon(downloadico); //��ͼƬ���õ���ť��
	ui->downloadButton->setIconSize(QSize(70, 55));//����ʵ�ʵ���ͼƬ��С
	ui->downloadButton->setStyleSheet("border:none");
	QIcon chatico("Resource/chat.png"); //����QIcon����
	ui->chatButton->setIcon(chatico); //��ͼƬ���õ���ť��
	ui->chatButton->setIconSize(QSize(70, 55));//����ʵ�ʵ���ͼƬ��С
	ui->chatButton->setStyleSheet("border:none");
	QIcon deleteico("Resource/delete.png"); //����QIcon����
	ui->deleteButton->setIcon(deleteico); //��ͼƬ���õ���ť��
	ui->deleteButton->setIconSize(QSize(70, 55));//����ʵ�ʵ���ͼƬ��С
	ui->deleteButton->setStyleSheet("border:none");
	
	//QMovie *movie = new QMovie("Resource/MainWindow.gif");//����ͼƬgif
	//ui->photoLabel->setGeometry(500, 20, 200, 0);
	//movie->setScaledSize(QSize(900, 580));//����ͼƬ��С
	//ui->photoLabel->setMovie(movie);
	//	movie->start();
		setWindowIcon(QIcon("Resource/icon1.png"));//���ô������Ͻ�ͼ��
		setFixedSize(700, 600); // ��ֹ�ı䴰�ڴ�С
		ui->personButton->setToolTip(tr("upload your photo"));//�����ť��ʾ��Ӧ��Ϣ
		ui->uploadButton->setToolTip(tr("upload"));
		ui->downloadButton->setToolTip(tr("download"));
		ui->chatButton->setToolTip(tr("chat"));
		ui->deleteButton->setToolTip(tr("delete"));
		//�Զ����˳���ť
		QToolButton *closeButton = new QToolButton(this);
		QPixmap quitPixmap = QPixmap("Resource/icon2.png");
		closeButton->setStyleSheet("QToolButton{border:none}");
		closeButton->setFixedSize(QSize(20, 20));
		closeButton->setIcon(quitPixmap);
		closeButton->setGeometry(this->width() - 20, 0, 20, 20);
		connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
		//���ô��ڱ���ɫ
		QPalette  palette(this->palette());
		palette.setColor(QPalette::Background, QColor(255, 255,255, 255));
		this->setPalette(palette);
	    //���ñ���label������ɫ
		ui->titleLabel->setStyleSheet("background:transparent");//����label͸��ɫ
		//ui.titleLabel->setStyleSheet("background-color:rgb(255, 255, 255)");//label������ɫΪ��ɫ
		ui->titleLabel->setStyleSheet("color:rgb(27, 41, 234)");//label������ɫΪ��ɫ
		//

	qDebug() << globalUserName << " welcome! ";

}

FileManagement::~FileManagement()
{
	delete ui;
}
//
//void FileManagement::initTitleBar()
//{
//	m_titleBar->move(1, 2);
//	m_titleBar->raise();
//	m_titleBar->setWindowBorderWidth(2);
//	m_titleBar->setBackgroundColor(255, 255, 255);
//	m_titleBar->setButtonType(MIN_BUTTON);
//	m_titleBar->setTitleWidth(this->width());
//}

void FileManagement::receiveLogin()
{
	this->show();
}
void FileManagement::Btn_Click()
{
	QMessageBox msg;
	msg.setText("hello world!");
	msg.exec();
}


//����ϴ��ļ���ť��ʾ�ϴ�����
void FileManagement::ClickUploadFile()
{
	//����Լ�
	if (uploadFileWindowIsOpen)
		uploadFile->show();
	//�����������
	if (downloadFileWindowIsOpen)
		downloadFile->hide();
	if (userInformationWindowIsOpen)
		userInformation->hide();
	if (!uploadFileWindowIsOpen)
	{
		uploadFile = new UploadFile(this);  //��ָ��ʵ����
		ui->SubLayout->insertWidget(1, uploadFile);
		uploadFile->show();
		
		uploadFileWindowIsOpen = true;
	}
	else
	{
		qDebug() << "uploadFileWindowIsOpen!";
	}

}

//��������ļ���ť��ʾ���ؽ���
void FileManagement::ClickDownloadFile()
{

	//�����ʾ�ļ���Ϣ
	if (downloadFileWindowIsOpen)
	{
		downloadFile->show();
		downloadFile->sendFileInfo();
	}
	if (userInformationWindowIsOpen)
		userInformation->hide();
	if (uploadFileWindowIsOpen)
		uploadFile->hide();
	if (!downloadFileWindowIsOpen)
	{
		downloadFile = new DownloadFile(this);  //��ָ��ʵ����
		downloadFile->sendFileInfo();
		ui->SubLayout->insertWidget(1, downloadFile);
		downloadFile->show();
		downloadFileWindowIsOpen = true;
	}
	else
	{
		qDebug() << "uploadFileWindowIsOpen!";
	}
	//emit showUploadFile();
}

void FileManagement::ClickUserInformation()
{
	if (userInformationWindowIsOpen)
	{
		userInformation->show();
		userInformation->sendUserInfo();
	}
	if (downloadFileWindowIsOpen)
		downloadFile->hide();
	if (uploadFileWindowIsOpen)
		uploadFile->hide();
	if (!userInformationWindowIsOpen)
	{
		userInformation = new UserInformation(this);
		//�����˷����û���Ϣ
		userInformation->sendUserInfo();
		ui->SubLayout->insertWidget(1, userInformation);
		userInformation->show();
		userInformationWindowIsOpen = true;
	}
	else
		qDebug() << "userInformationWindowIsOpen!";

}

void FileManagement::ClickReturn()
{
	this->hide();
	emit showLogin();
}