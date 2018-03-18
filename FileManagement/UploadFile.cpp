#include "UploadFile.h"  
#include <QByteArray>
#include <QDataStream>
#include <QFileDialog>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <Qtime>
#include <QtWin>
#include <QFileIconProvider>
#include <QStackedLayout>

//����1�� �û���ҷ���ļ�����û�е�����͵Ļ����������Ѿ������ˡ������ļ������źš����ʱ��
//�ͻ�����⣬�����������������ճ�������

UploadFile::UploadFile(QString m_fileName,int num) 
{
	index = num; //��¼���Id
	fileName = m_fileName;
	initFile();  //��ʼ����ֱ���½�һ��TCP��
}

UploadFile::~UploadFile()
{
	delete tcpSocket;
}

//void UploadFile::init(QString fileName)
//{
//	//QStackedLayout *stackLayout = new QStackedLayout();
//	////stackLayout->addWidget(ui->mFileIcon);
//	////stackLayout->addWidget(ui->mFileName);
//	////�ص���ʾ
//	//stackLayout->addWidget(ui->horizontalWidget);
//	//stackLayout->addWidget(ui->sendProgressBar);
//	//ui->sendProgressBar->setTextVisible(false);   //����ʾ�ٷֱ�
//	//stackLayout->setStackingMode(QStackedLayout::StackAll);
//	//ui->horizontalLayout_3->addLayout(stackLayout);
//	////�ٶȰ�����ǩʲô�Ķ�����������
//	//ui->uploadSpeedLabel->hide();
//	//ui->mFileIcon->hide();
//	//ui->mFileName->hide();
//	//ui->sendProgressBar->hide();
//
//
//	//setAcceptDrops(true);
//	//QPixmap DragDrop(":/Resource/FunIcon/Drag-Drop.png");  //�������ļ���ͼ��
//	//										 //Drag = fil.scaled(QSize(50, 60), Qt::KeepAspectRatio);
//	//ui->Drag->setPixmap(DragDrop);
//	////ui->Drag->setAlignment(Qt::AlignCenter);
//	////connect(tcp, SIGNAL(connected()), this, SLOT(send()));  //�����ӳɹ�ʱ���Ϳ�ʼ�����ļ� 
//
//	//loadStyleSheet("UploadFile");  //
//
//}
//void UploadFile::loadStyleSheet(const QString &sheetName)
//{
//	QFile file("Resource/qss/" + sheetName + ".qss");
//	qDebug() << sheetName + ".qss";
//	file.open(QFile::ReadOnly);
//	if (file.isOpen())
//	{
//		qDebug() << "is  css";
//		QString styleSheet = this->styleSheet();
//		styleSheet += QLatin1String(file.readAll());
//		this->setStyleSheet(styleSheet);
//	}
//	else
//		qDebug() << "is not css";
//}
//void UploadFile::dragEnterEvent(QDragEnterEvent *event)
//{
//	//���Ϊ�ļ�����֧���Ϸ�
//	if (event->mimeData()->hasFormat("text/uri-list"))
//		event->acceptProposedAction();
//}

//void UploadFile::dropEvent(QDropEvent *event)
//{
//	//ע�⣺��������ж��ļ����ڣ���˼���û�һ�����϶��˶���ļ����������϶�һ��Ŀ¼
//	//������ȡ����Ŀ¼�����ڲ�ͬ�Ĳ���ƽ̨�£��Լ���д����ʵ�ֶ�ȡ����Ŀ¼�ļ���
//	QList<QUrl> urls = event->mimeData()->urls();
//	if (urls.isEmpty())
//		return;
//	//���ı�����׷���ļ���
//	foreach(QUrl url, urls) {
//		 fileName = url.toLocalFile();
//		//ui->sendStatusLabel->setText(fileName);
//		initFile();
//		setFileIcon(fileName);  //�����ļ�ͼ�� 
//		localFile = new QFile(fileName);
//		localFile->open(QFile::ReadOnly);
//
//		//ui->sendStatusLabel->setText(QString::fromLocal8Bit("�Ѵ��ļ� %1").arg(fileName));
//	}
//}

//�������Ҫ�������ʵ�ֶ��߳����أ���һ����newһ������
void UploadFile::initFile()
{
	
	tcpSocket = new QTcpSocket();
	qDebug() <<"the tcp thread"<< QThread::currentThreadId();
	tcpSocket->abort();   //ȡ�����е�����
	tcpSocket->connectToHost(ip, port);  //�����socket���ӵ�������

	tcpSocket->write("uploadFile");
	//qDebug() << globalUserName << " will send a file to server";
	loadSize = 0;
	byteToWrite = 0;
	totalSize = 0;
	sendTimes = 0;
	outBlock.clear();
	if (!fileName.isNull())
	{
		localFile = new QFile(fileName);
		if (!localFile->open(QFile::ReadOnly))
		{
			qDebug() << "this file is not exits: " << fileName;
		}
	}

	//������ͣһ��

	/*send();*/
}


void UploadFile::send()  //�����ļ�ͷ��Ϣ  
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateSpeed())); // ***��������˵����Ӧ����
	timer->start(1000); // ÿ��1s�����ٶȱ�ǩ

	qDebug() << "the send thread" << QThread::currentThreadId();
	if (!fileName.isNull())
	{
		//��һ�η��͵�ʱ��������
		connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		byteToWrite = localFile->size();  //ʣ�����ݵĴ�С  
		qDebug() << "the file bytetowrite: " << byteToWrite;
		totalSize = localFile->size();

		loadSize = 4 * 1024;  //ÿ�η������ݵĴ�С  

		QDataStream out(&outBlock, QIODevice::WriteOnly);
		//out.setVersion(QDataStream::Qt_4_8);

		//��ȡ�ļ�����
		currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
		//ռλ��,�������Ҫ������ռλ��Ȼ�������������峤�Ⱥ��ڲ���
		//ǰ���������ļ���С�ͷ����ļ�ͷ�Ĵ�С��Ϊʲô��qint64�أ������������ļ������û���
		out << qint64(0) << qint64(0) << currentFileName << globalUserName;

		totalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
		byteToWrite += outBlock.size();
		qDebug() << "the total bytetowrite: " << byteToWrite;
		out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
		out << totalSize << qint64(outBlock.size());   //������Ƕ�Ӧǰ���qint64��
		qDebug() << "the file head:" << outBlock;
		tcpSocket->write(outBlock);  //���������ļ���Ϣ���͵��׽��� 

		//���½�����
		//emit updateProgress(index, byteToWrite,totalSize,speed);

		sendTime.start();  //����ʱ�俪ʼ��ʱ
						   //UI��Ϣ
	}
}


//��������
void UploadFile::goOnSend(qint64 numBytes)  
{
	
	sendTimes++;  
		qDebug()  <<" threadId:"<< QThread::currentThreadId() << currentFileName << "the " << sendTimes<<"the numBytes: " << numBytes <<"the loadSize:" <<loadSize
			 << "  left byteTowrite: " << byteToWrite;
	byteToWrite -= numBytes;  //ʣ�����ݴ�С  
		
	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //���ʣ�����ݱ�ÿ�η��͵�С����ʣ���
	/*qDebug() << "byteToWrite:" << byteToWrite;*/
	tcpSocket->write(outBlock);    //�������Ϣд��socket

	//�������ٶȴ�����ʾ�ٶȺ�������
	float useTime = sendTime.elapsed();
	 speed = (totalSize - byteToWrite) / useTime;
	//speed = (speed * 1000 / (1024 * 1024));
	//ui->uploadSpeedLabel->setText(QString::fromLocal8Bit("�ѷ��� %1MB (%2MB/s) ��%3MB ����ʱ:%4��\n����ʣ��ʱ�䣺%5��")
	//	.arg((totalSize - byteToWrite) / (1024 * 1024))//�ѽ���
	//	.arg(speed * 1000 / (1024 * 1024), 0, 'f', 2)//�ٶ�
	//	.arg(totalSize / (1024 * 1024))//�ܴ�С
	//	.arg(useTime / 1000, 0, 'f', 0)//��ʱ
	//	.arg(totalSize / speed / 1000 - useTime / 1000, 0, 'f', 0));//ʣ��ʱ��
	emit updateProgress(index, byteToWrite, totalSize);
	//
	if (byteToWrite == 0)  //�������  
	{
		//�����������װ�����ͬʱ���ʹ��ļ���С�ļ�ʱ�����С�ļ��������˻�ִ�е���һ��Ȼ��Ͽ�TCP�����ӡ�
		
		//disconnect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		sendTimes = 0;
		isOver = true;   //�������
		emit sendOver(index);
		emit shutDownThread();
		qDebug() <<"ThreadId"<<QThread::currentThreadId<< " send index " << index;
		//�������ȡ�����ӣ����߳��˳���
		//uploadThread->quit();
		tcpSocket->abort();
		qDebug() << currentFileName << "had sended!";
	}
}
//���յ����͵��ź�
void UploadFile::receiveSendSignal()
{
	qDebug() << "receive the send signal";
	send();
}

//ÿ��һ�����һ���ٶ�
void UploadFile::updateSpeed()
{
	emit updateSpeedLabel(index,speed);
}
//void UploadFile::sendOver(int)
//{
//}


//void UploadFile::ClickSendButton()
//{
//	send();  //��һ�η��͵�ʱ������connectToHost����connect�źŲ��ܵ���send���ڶ���֮�����Ҫ����send��  
//	ui->sendBtn->setEnabled(false);
//	//connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
//	//ui->sendStatusLabel->setText(QString::fromLocal8Bit("���ڷ����ļ� %1").arg(fileName));
//}


//void UploadFile::setFileIcon(QString fileName)
//{
//
//	QString suffix = (fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)).left(3); //��ȡ�ļ���׺ǰ�����ַ�
//	QString allName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1); //��ȡ�ļ�����
//	qDebug() << "the suffix :" << suffix << " and the allName:" << allName;
//		// ��ȡͼ�ꡢ����
//	QIcon icon = fileIcon(suffix);
//	QString strType = fileType(suffix);
//	qDebug() << strType;
//	QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(30, 40)));
//	//QPixmap fileIcon(icon.);  //�������ļ���ͼ��
//	//fileIcon = fileIcon.scaled(QSize(30, 40), Qt::KeepAspectRatio);
//	ui->mFileIcon->setPixmap(pixmap);
//	ui->mFileIcon->setAlignment(Qt::AlignCenter);
//	//QPixmap fileIcon("Resource/fileIcon/" + suffix);  //�������ļ���ͼ��
//	//fileIcon = fileIcon.scaled(QSize(30, 40), Qt::KeepAspectRatio);
//	//ui->mFileIcon->setPixmap(fileIcon);
//	//ui->mFileIcon->setAlignment(Qt::AlignCenter);
//	ui->mFileName->setText(allName);
//	ui->mFileIcon->show();
//	ui->mFileName->show();
//	//���ļ���ʱ�������ٶȺͱ�ǩ
//	ui->uploadSpeedLabel->hide();
//	ui->sendProgressBar->show();
//}

//�����ļ��ź�
//void UploadFile::receiveSendSignal()
//{
//	send();
//}



////��ͼ������һ�������͵�ǰ�Ľ���
//void UploadFile::drawProgress(qint64 total, qint64 now)
//{
//
//}