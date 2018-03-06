#include "FileManagement.h"

#include "DownloadFile.h"
#include <QMessageBox> 
#include <QLabel>
#include <QMovie>
#include <QThread>
extern QString globalUserName;
FileManagement::FileManagement(QWidget *parent)
	: BaseWindow(parent), ui(new Ui::FileManagement)
{
	ui->setupUi(this);
	init();
	StackWindow();
	//initTitleBar();
	loadStyleSheet("mainWindow");

}

FileManagement::~FileManagement()
{
	delete ui;
}

void FileManagement::init()
{

	ui->titleLabel->setStyleSheet("background:transparent");//����label͸��ɫ
	ui->titleLabel->setStyleSheet("color:rgb(27, 41, 234)");//label������ɫΪ��ɫ
	QPalette Pal(palette());
	// set black background
	Pal.setColor(QPalette::Background, Qt::gray);
	ui->ToolButtonWidget->setAutoFillBackground(true);
	ui->ToolButtonWidget->setPalette(Pal);
	//ui->ToolButtonWidget->setStyleSheet("backgroud-color : black;");

	QIcon personico(":/Resource/FunIcon/userInfo.png"); //����QIcon����
	ui->personButton->setIcon(personico); //��ͼƬ���õ���ť��
	ui->personButton->setIconSize(QSize(80, 20));//����ʵ�ʵ���ͼƬ��С
	ui->personButton->setStyleSheet("border:none");   //����button�ı߿���(true);
													  //ui->personButton->setStyleSheet("QToolButton{image-align : left;}");

	ui->personButton->setCheckable(true);

	ui->personButton->setAutoExclusive(true);

	//ui->personButton->setAlignment(Qt::AlignLeft);
	//ui->personLayout->setAlignment(Qt::AlignLeft);
	//toolbutton���ͼƬ
	QIcon uploadico(":/Resource/FunIcon/upload.png"); //����QIcon����
	ui->uploadButton->setIcon(uploadico); //��ͼƬ���õ���ť��
	ui->uploadButton->setIconSize(QSize(80, 20));//����ʵ�ʵ���ͼƬ��С
	ui->uploadButton->setStyleSheet("border:none");   //����button�ı߿���(true);
	ui->uploadButton->setCheckable(true);

	ui->uploadButton->setAutoExclusive(true);

	QIcon downloadico(":/Resource/FunIcon/download.png"); //����QIcon����
	ui->downloadButton->setIcon(downloadico); //��ͼƬ���õ���ť��
	ui->downloadButton->setIconSize(QSize(80, 20));//����ʵ�ʵ���ͼƬ��С
	ui->downloadButton->setStyleSheet("border:none");

	//���check״̬
	ui->downloadButton->setCheckable(true);
	ui->downloadButton->setAutoExclusive(true);

	QIcon chatico(":/Resource/FunIcon/showFile.png"); //����QIcon����
	ui->showFileButton->setIcon(chatico); //��ͼƬ���õ���ť��
	ui->showFileButton->setIconSize(QSize(80, 20));//����ʵ�ʵ���ͼƬ��С
	ui->showFileButton->setStyleSheet("border:none");
	ui->showFileButton->setCheckable(true);

	ui->showFileButton->setAutoExclusive(true);

	QIcon deleteico(":/Resource/FunIcon/exit.png"); //����QIcon����
	ui->deleteButton->setIcon(deleteico); //��ͼƬ���õ���ť��
	ui->deleteButton->setIconSize(QSize(80, 20));//����ʵ�ʵ���ͼƬ��С
	ui->deleteButton->setStyleSheet("border:none");
	ui->deleteButton->setCheckable(true);
	ui->deleteButton->setAutoExclusive(true);

	QIcon reportEditicon(":/Resource/FunIcon/reportEdit.png"); //����QIcon����
	ui->reportEditButton->setIcon(reportEditicon); //��ͼƬ���õ���ť��
	ui->reportEditButton->setIconSize(QSize(80, 20));//����ʵ�ʵ���ͼƬ��С
	ui->reportEditButton->setStyleSheet("border:none");
	ui->reportEditButton->setCheckable(true);
	ui->reportEditButton->setAutoExclusive(true);

	QIcon reportLookicon(":/Resource/FunIcon/reportLook.png"); //����QIcon����
	ui->reportLookButton->setIcon(reportLookicon); //��ͼƬ���õ���ť��
	ui->reportLookButton->setIconSize(QSize(80, 20));//����ʵ�ʵ���ͼƬ��С
	ui->reportLookButton->setStyleSheet("border:none");
	ui->reportLookButton->setCheckable(true);
	ui->reportLookButton->setAutoExclusive(true);

	//QMovie *movie = new QMovie(":/Resource/MainWindow.gif");//����ͼƬgif
	//ui->photoLabel->setGeometry(500, 20, 200, 0);
	//movie->setScaledSize(QSize(900, 580));//����ͼƬ��С
	//ui->photoLabel->setMovie(movie);
	//	movie->start();
	setWindowIcon(QIcon(":/Resource/icon1.png"));//���ô������Ͻ�ͼ��
	setFixedSize(800, 600); // ��ֹ�ı䴰�ڴ�С
	ui->personButton->setToolTip(tr("upload your photo"));//�����ť��ʾ��Ӧ��Ϣ
	ui->uploadButton->setToolTip(tr("upload"));
	ui->downloadButton->setToolTip(tr("download"));
	ui->showFileButton->setToolTip(tr("chat"));
	ui->deleteButton->setToolTip(tr("�˳�ϵͳ"));



	

	////�Զ����˳���ť
	//QToolButton *closeButton = new QToolButton(this);
	//QPixmap quitPixmap = QPixmap(":/Resource/icon2.png");
	//closeButton->setStyleSheet("QToolButton{border:none}");
	//closeButton->setFixedSize(QSize(20, 20));
	//closeButton->setIcon(quitPixmap);
	//closeButton->setGeometry(this->width() - 20, 0, 20, 20);
	//connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));


	//���ô��ڱ���ɫ
	/*QPalette  palette(this->palette());
	palette.setColor(QPalette:: Background, QColor(255, 255,255, 255));
	this->setPalette(palette);*/
	//���ñ���label������ɫ
	//ui.titleLabel->setStyleSheet("background-color:rgb(255, 255, 255)");//label������ɫΪ��ɫ
	//

	//this->setAttribute(Qt::WA_TranslucentBackground);
	////�߿���ӰЧ��
	//QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	//effect->setBlurRadius(6);
	//effect->setColor(Qt::black);
	//effect->setOffset(0, 0);
	//this->setGraphicsEffect(effect);
	//this->update();
}
//
//void FileManagement::initTitleBar()
//{ 
//	//m_titleBar->move(1, 2);
//	//m_titleBar->raise();
//	//m_titleBar->setWindowBorderWidth(2);
//	//m_titleBar->setBackgroundColor(255, 255, 255);
//	//m_titleBar->setButtonType(MIN_BUTTON);
//	//m_titleBar->setTitleWidth(this->width());
//}

void FileManagement::StackWindow()
{
	m_pStackedLayout = new QStackedLayout();
	
	//������Ҫ�Ƕ�������л���������Щ�඼ʵ����
	uploadFile = new UploadFile(this);
	downloadFile = new DownloadFile(this);
	userInformation = new UserInformation(this);
	reportEdit = new ReportEdit(this);
	reportLook = new ReportLook(this);
	//�����
	m_pStackedLayout->addWidget(uploadFile);
	m_pStackedLayout->addWidget(downloadFile);
	m_pStackedLayout->addWidget(userInformation);
	m_pStackedLayout->addWidget(reportEdit);
	m_pStackedLayout->addWidget(reportLook);
	ui->SubLayout->insertLayout(1, m_pStackedLayout);

}
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
	m_pStackedLayout->setCurrentWidget(uploadFile);

}

//��������ļ���ť��ʾ���ؽ���
void FileManagement::ClickDownloadFile()
{
	m_pStackedLayout->setCurrentWidget(downloadFile);
	downloadFile->sendFileInfo();
}

void FileManagement::ClickUserInformation()
{
	m_pStackedLayout->setCurrentWidget(userInformation);
	userInformation->sendUserInfo();
}

//���ReportEdit
void FileManagement::ClickReportEdit()
{
	m_pStackedLayout->setCurrentWidget(reportEdit);
	reportEdit->sendReportEdit();
}

void FileManagement::ClickReportLook()
{
	m_pStackedLayout->setCurrentWidget(reportLook);
	reportLook->sendReportLook();
}

void FileManagement::ClickReturn()
{
	this->hide();
	emit showLogin();
}