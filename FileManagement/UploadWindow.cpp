#include "UploadWindow.h"  
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <Qtime>
#include <QtWin>
#include <QFileIconProvider>
#include "MyMessageBox.h"

//����1�� �û���ҷ���ļ�����û�е�����͵Ļ����������Ѿ������ˡ������ļ������źš����ʱ��
//�ͻ�����⣬�����������������ճ�������
int sendtimes = 0;
UploadWindow::UploadWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UploadWindow)
{
	fileDialog = new QFileDialog();
	fileProgressBarMap = new QMap<QString, QProgressBar*>;   //��Ҫ��ʼ��
	ui->setupUi(this);
	init();  //��ʼ��
}

UploadWindow::~UploadWindow()
{
	delete ui;
}

//��Ҫ�ǳ�ʼ������
void UploadWindow::init()
{
	//����ص���ʾҪ�����ļ�
	//QStackedLayout *stackLayout = new QStackedLayout();
	////stackLayout->addWidget(ui->mFileIcon);
	////stackLayout->addWidget(ui->mFileName);
	////�ص���ʾ
	//stackLayout->addWidget(ui->horizontalWidget);
	//stackLayout->addWidget(ui->sendProgressBar);
	//ui->sendProgressBar->setTextVisible(false);   //����ʾ�ٷֱ�
	//stackLayout->setStackingMode(QStackedLayout::StackAll);
	//ui->horizontalLayout_3->addLayout(stackLayout);

	////�ٶȰ�����ǩʲô�Ķ�����������
	//ui->uploadSpeedLabel->hide();
	//ui->mFileIcon->hide();
	//ui->mFileName->hide();
	//ui->sendProgressBar->hide();


	setAcceptDrops(true);
	QPixmap DragDrop(":/Resource/FunIcon/Drag-Drop.png");  //�������ļ���ͼ��
														   //Drag = fil.scaled(QSize(50, 60), Qt::KeepAspectRatio);
	ui->Drag->setPixmap(DragDrop);
	//ui->Drag->setAlignment(Qt::AlignCenter);
	//connect(tcp, SIGNAL(connected()), this, SLOT(send()));  //�����ӳɹ�ʱ���Ϳ�ʼ�����ļ� 

	loadStyleSheet("UploadFile");  //

}

//�����ͼ��
void UploadWindow::insertFile(QString m_fileName)
{
	QProgressBar *fileProgressBar = new QProgressBar(this);
	fileProgressBar->setTextVisible(false);   //����ʾ�ٷֱ�
	
	fileProgressBarMap->insert(m_fileName, fileProgressBar);  //���������������map

	 stackLayout = new QStackedLayout();
	 
	 fileWidget = new QWidget(this);
	 //fileWidget->setLayout(fileInfoLayout);  //����ˮƽlayout
	 //�����ļ�������
	 fileInfoLayout = new QHBoxLayout(fileWidget);
	 mFileIcon = new QLabel(fileWidget);
	 mFileName = new QLabel(fileWidget);
	 fileInfoLayout->addWidget(mFileIcon);
	 fileInfoLayout->addWidget(mFileName);
	 
	 
	 setFileIcon(m_fileName);	//���ø��ļ������ֺ�ͼ��
	 

	 //fileInfoLayout->insertWidget(0, mFileIcon);
	 //fileInfoLayout->insertWidget(1, mFileName);
	 //�ҵ����ļ���������
	 
	 
	
	 //stackLayout->addItem(fileInfoLayout);
	 stackLayout->addWidget(fileWidget);
	 stackLayout->addWidget(fileProgressBar);

	 //fileWidget->raise();
	 fileProgressBar->setTextVisible(false);
	 //fileProgressBar->hide();
	

	 stackLayout->setStackingMode(QStackedLayout::StackAll);
	 ui->FileListLayout->addLayout(stackLayout);  //��ֱ�ķ��ö���ļ�

}
void UploadWindow::loadStyleSheet(const QString &sheetName)
{
	QFile file("Resource/qss/" + sheetName + ".qss");
	qDebug() << sheetName + ".qss";
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		qDebug() << "is  css";
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	else
		qDebug() << "is not css";
}
void UploadWindow::dragEnterEvent(QDragEnterEvent *event)
{
	//���Ϊ�ļ�����֧���Ϸ�
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void UploadWindow::dropEvent(QDropEvent *event)
{
	//ע�⣺��������ж��ļ����ڣ���˼���û�һ�����϶��˶���ļ����������϶�һ��Ŀ¼
	//������ȡ����Ŀ¼�����ڲ�ͬ�Ĳ���ƽ̨�£��Լ���д����ʵ�ֶ�ȡ����Ŀ¼�ļ���
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;
	//���ı�����׷���ļ���
	foreach(QUrl url, urls) {
		fileName = url.toLocalFile();
		qDebug() << fileName;
		//ui->sendStatusLabel->setText(fileName);
		fileNameList.append(fileName);
		insertFile(fileName);
		//initFile();
		////setFileIcon(fileName);  //�����ļ�ͼ�� 
		//localFile = new QFile(fileName);
		//localFile->open(QFile::ReadOnly);

		//ui->sendStatusLabel->setText(QString::fromLocal8Bit("�Ѵ��ļ� %1").arg(fileName));
	}
}

void UploadWindow::initFile()
{


	//tcpSocket = new QTcpSocket();
	//tcpSocket->abort();   //ȡ�����е�����
	//tcpSocket->connectToHost(ip, port);  //�����socket���ӵ�������

	//tcpSocket->write("UploadWindow");
	//qDebug() << globalUserName << " will send a file to server";
	//loadSize = 0;
	//byteToWrite = 0;
	//totalSize = 0;
	//sendtimes = 0;
	//ui->sendProgressBar->setValue(0);  //�ǵ�һ�η���  
	//outBlock.clear();
}

//�򿪰�ť
void UploadWindow::ClickOpenButton()  //���ļ�����ȡ�ļ���������·����  
{
	//ui->sendStatusLabel->setText(QString::fromLocal8Bit("���ڴ��ļ�..."));
	//���߷���������Ҫ�����ļ���
	//����򿪰�ť��ʱ���ʼ������

	//ֱ�Ӽ���
	
	//fileDialog->open();
	//fileDialog->selectedFiles();
	
	fileNameList.append(QFileDialog::getOpenFileNames());
	for (int i = 0; i < fileNameList.size(); i++)
	{
		qDebug()<<"the file list:" << fileNameList.at(i);
	}
	//fileName = QFileDialog::getOpenFileName(this);
	if (!fileNameList.isEmpty())
	{
		for (int i = 0; i < fileNameList.size(); i++)
		{
			insertFile(fileNameList.at(i));
		}
		
		//setFileIcon(fileName);  //�����ļ�ͼ�� 
		//localFile = new QFile(fileName);
		//localFile->open(QFile::ReadOnly);
		//ui->sendStatusLabel->setText(QString::fromLocal8Bit("�Ѵ��ļ� %1").arg(fileName));
	}
}

//void UploadWindow::send()  //�����ļ�ͷ��Ϣ  
//{
//	if (!fileName.isNull())
//	{
//		//��һ�η��͵�ʱ��������
//		connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//		byteToWrite = localFile->size();  //ʣ�����ݵĴ�С  
//		qDebug() << "the file bytetowrite: " << byteToWrite;
//		totalSize = localFile->size();
//
//		loadSize = 4 * 1024;  //ÿ�η������ݵĴ�С  
//
//		QDataStream out(&outBlock, QIODevice::WriteOnly);
//		//��ȡ�ļ�����
//		QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
//		//ռλ��,�������Ҫ������ռλ��Ȼ�������������峤�Ⱥ��ڲ���
//		//ǰ���������ļ���С�ͷ����ļ�ͷ�Ĵ�С��Ϊʲô��qint64�أ������������ļ������û���
//		out << qint64(0) << qint64(0) << currentFileName << globalUserName;
//
//		totalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
//		byteToWrite += outBlock.size();
//		qDebug() << "the total bytetowrite: " << byteToWrite;
//		out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
//		out << totalSize << qint64(outBlock.size());   //������Ƕ�Ӧǰ���qint64��
//		qDebug() << "the file head:" << outBlock;
//		tcpSocket->write(outBlock);  //���������ļ���Ϣ���͵��׽���  
//
//		sendTime.start();  //����ʱ�俪ʼ��ʱ
//						   //UI��Ϣ
//						   //ui->progressLabel->show();
//		ui->sendProgressBar->setMaximum(totalSize);
//		ui->sendProgressBar->setValue(totalSize - byteToWrite);
//	}
//	else
//	{
//		//������������ļ���ʾ��
//		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��������ļ�!"), MESSAGE_INFORMATION, BUTTON_OK, true);
//	}
//
//}


//��������
//void UploadWindow::goOnSend(qint64 numBytes)
//{
//	sendtimes++;
//	qDebug() << sendtimes << " get in goOnSend";
//	qDebug() << "the numBytes: " << numBytes << "the loadSize:" << loadSize
//		<< "the " << sendtimes << "  left byteTowrite: " << byteToWrite;
//
//	byteToWrite -= numBytes;  //ʣ�����ݴ�С  
//	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //���ʣ�����ݱ�ÿ�η��͵�С����ʣ���
//
//	tcpSocket->write(outBlock);    //�������Ϣд��socket
//
//								   //qDebug() << "information:" <<outBlock;
//
//								   //�������ٶȴ�����ʾ�ٶȺ�������
//	float useTime = sendTime.elapsed();
//	double speed = (totalSize - byteToWrite) / useTime;
//	ui->uploadSpeedLabel->show();
//	ui->uploadSpeedLabel->setText(QString::fromLocal8Bit("�ѷ��� %1MB (%2MB/s) ��%3MB ����ʱ:%4��\n����ʣ��ʱ�䣺%5��")
//		.arg((totalSize - byteToWrite) / (1024 * 1024))//�ѽ���
//		.arg(speed * 1000 / (1024 * 1024), 0, 'f', 2)//�ٶ�
//		.arg(totalSize / (1024 * 1024))//�ܴ�С
//		.arg(useTime / 1000, 0, 'f', 0)//��ʱ
//		.arg(totalSize / speed / 1000 - useTime / 1000, 0, 'f', 0));//ʣ��ʱ��
//
//
//																	//����UI�Ľ�������������Կ�����������
//	ui->sendProgressBar->setMaximum(totalSize);
//	ui->sendProgressBar->setValue(totalSize - byteToWrite);
//
//	if (byteToWrite == 0)  //�������  
//	{
//		//��������ҵ���˵����ȷ����ť��
//		if (!MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"),
//			QString::fromLocal8Bit("�ϴ����!"), MESSAGE_INFORMATION, BUTTON_OK, true))
//		{
//			ui->sendBtn->setEnabled(true);
//		}
//		//ui->sendStatusLabel->setText(QString::fromLocal8Bit("�ļ��������!"));
//		//������ϾͶϿ����д�ֽڵĲۺ�����
//		disconnect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//		sendtimes = 0;
//
//		//�������ȡ�����ӣ����߳��˳���
//		uploadThread->quit();
//		tcpSocket->abort();
//		qDebug() << "the file had send";
//	}
//}


void UploadWindow::ClickSendButton()
{
	//�����ļ���Ϣ
	emit sendFileSignal();
	ui->sendBtn->setEnabled(false); //��ťʧЧ

	//connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
	//ui->sendStatusLabel->setText(QString::fromLocal8Bit("���ڷ����ļ� %1").arg(fileName));
}


void UploadWindow::setFileIcon(QString fileName)
{

	QString suffix = (fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)).left(3); //��ȡ�ļ���׺ǰ�����ַ�
	QString allName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1); //��ȡ�ļ�����
	qDebug() << "the suffix :" << suffix << " and the allName:" << allName;
	// ��ȡͼ�ꡢ����
	QIcon icon = fileIcon(suffix);
	QString strType = fileType(suffix);
	qDebug() << strType;
	QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(30, 40)));
	//QPixmap fileIcon(icon.);  //�������ļ���ͼ��
	//fileIcon = fileIcon.scaled(QSize(30, 40), Qt::KeepAspectRatio);
	mFileIcon->setPixmap(pixmap);
	mFileIcon->setAlignment(Qt::AlignCenter);
	
	mFileName->setText(allName);
	mFileIcon->show();
	mFileName->show();
	//���ļ���ʱ�������ٶȺͱ�ǩ
	//ui->uploadSpeedLabel->hide();
	//ui->sendProgressBar->show();
}

void UploadWindow::receiveMainwindow()
{
	this->show();
}

QIcon UploadWindow::fileIcon(const QString &extension) const
{
	QFileIconProvider provider;
	QIcon icon;
	QString strTemplateName = QDir::tempPath() + QDir::separator() +
		QCoreApplication::applicationName() + "_XXXXXX." + extension;
	QTemporaryFile tmpFile(strTemplateName);
	tmpFile.setAutoRemove(false);

	if (tmpFile.open())
	{
		tmpFile.close();
		icon = provider.icon(QFileInfo(fileName));
		// tmpFile.remove();
	}
	else
	{
		qDebug() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
	}

	return icon;
}

QString UploadWindow::fileType(const QString &extension) const
{
	QFileIconProvider provider;
	QString strType;
	QString strFileName = QDir::tempPath() + QDir::separator() +
		QCoreApplication::applicationName() + "_XXXXXX." + extension;
	QTemporaryFile tmpFile(strFileName);
	tmpFile.setAutoRemove(false);

	if (tmpFile.open())
	{
		tmpFile.close();
		strType = provider.type(QFileInfo(tmpFile.fileName()));
		// tmpFile.remove();
	}
	else
	{
		qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
	}

	return strType;
}

