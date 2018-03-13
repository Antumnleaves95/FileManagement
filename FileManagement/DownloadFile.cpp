#include "DownloadFile.h"
#include <QFileDialog>
#include <QtWin>
#include "MyMessageBox.h"
DownloadFile::DownloadFile(QString mFileName, QString mFilePath,int num)
{
	fileName = mFileName;
	filePath = mFilePath;   //���ļ����� filePath + fileName��
	index = num;
	init();
	qDebug() << "new downloadFile :" <<QThread::currentThreadId();
}

DownloadFile::~DownloadFile()
{
	//delete ui;
}


//��ʼ������
void DownloadFile::init()
{
	byteReceived = 0;
	receiveTime = 0;
	//initModel();

	tcpSocket = new QTcpSocket();  //�½�һ��socket
	tcpSocket->abort();   //ȡ�����е�����
	tcpSocket->connectToHost(ip, port);  //�����socket���ӵ�������

	QString bs = "downloadFile";
	QString data = bs + "#" + fileName;

	QByteArray datasend = data.toUtf8();  //����UTF8��ȥ
	if (tcpSocket->write(datasend))
	{
		qDebug() << "send data to server: " << data;
		connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));  //���źŲ�
	}
	else
	{
		qDebug() << "send filed :" << data;
	}
}

void DownloadFile::receiveFile()
{
	if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
	{
		QTimer *timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateSpeed())); // ***��������˵����Ӧ����
		timer->start(1000); // ÿ��1s�����ٶȱ�ǩ
 
		qDebug() << "receive the file of head��";
		//ui->receivedProgressBar->setValue(0);
		QDataStream in(tcpSocket);
		//QString globalUserName;
		in >> RtotalSize >> byteReceived >> fileName;
		//user.queryUserByName(globalUserName);
		//qDebug() << "the user :" << user.getUserName();
		
		qDebug() << "the file of head: " << fileName;
		qDebug() << "totalSize: " << RtotalSize;
		qDebug() << " first byteReceived: " << byteReceived;
		saveFileName = filePath + fileName;
		qDebug() << "start saveFileName:" << saveFileName;

		//���һ���ļ���д���ظ��Ĺ���
		int i = 0;
		QString suffix = fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1); //��ȡ�ļ���׺;
		//����ͷ������Ĳ�һ��
		QString fullName = saveFileName.left(saveFileName.size() - (saveFileName.size() - saveFileName.lastIndexOf('.'))); //��ȡ�ļ���
		while (1)
		{
			QFile *existFile = new QFile(saveFileName);  //�����ж��Ƿ��ظ�
			if (existFile->exists())   //����ļ�����
			{
				i++;
				saveFileName = fullName + "(" + QString::number(i, 10) + ")." + suffix;
				qDebug() << "the fileName :" << saveFileName;
			}
			else
				break;
		}

		newFile = new QFile(saveFileName);  //�½�һ���ļ���д��
		newFile->open(QFile::WriteOnly);
		//��ʼ��ʱ��
		downloadTime.start();
	}
	else  //��ʽ��ȡ�ļ�����  
	{
		while (tcpSocket->bytesAvailable() > 0)
		{
			receiveTime++;
			qDebug()<< "ThreadId: " << QThread::currentThreadId() 
				<< fileName<< " the " << receiveTime << "recv block "<< inBlock.size()
				<< "current receive the byte is :" << byteReceived 
				<< " and the total " << RtotalSize;
			inBlock = tcpSocket->read(4 * 1024);
			//qDebug() << "the inBlock size is " << inBlock.size();   //��tmճ�����ֵ�
			//qDebug() << "current received the byte is :" << byteReceived;
			//qDebug() << "the block: " << inBlock;
			byteReceived += inBlock.size();
			float useTime = downloadTime.elapsed();
			speed = (byteReceived) / useTime;
			//speed = (speed * 1000 / (1024 * 1024));

			//����ʱ�䣬����һ��float
			countLeftTime(RtotalSize / speed / 1000 - useTime / 1000); 
			emit updateProgress(index, byteReceived, RtotalSize);
			//ui->downloadSpeedLabel->show();
			//ui->downloadSpeedLabel->setText(QString::fromLocal8Bit("������ %1MB (%2MB/s) ��%3MB ����ʱ:%4��\n����ʣ��ʱ�䣺%5��")
			//	.arg((byteReceived) / (1024 * 1024))//�ѽ���
			//	.arg(speed * 1000 / (1024 * 1024), 0, 'f', 2)//�ٶ�
			//	.arg(RtotalSize / (1024 * 1024))//�ܴ�С
			//	.arg(useTime / 1000, 0, 'f', 0)//��ʱ
			//	.arg(RtotalSize / speed / 1000 - useTime / 1000, 0, 'f', 0));//ʣ��ʱ��

			//ui->progressBar->setMaximum(RtotalSize);
			//ui->progressBar->setValue(byteReceived);
			//qDebug() << "begin to write";

			newFile->write(inBlock);
			newFile->flush();
		}
	}

	if (byteReceived == RtotalSize)
	{
		//MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�������!"), MESSAGE_INFORMATION, BUTTON_OK);
		//ui->downloadSpeedLabel->setText(QString::fromLocal8Bit("������ɣ�"));
		qDebug() << "receive is done, and the byteReceived is :" << byteReceived <<" the RtotalSize is :"<< RtotalSize;
		/*qDebug() << "the file name:" << file.getFileName();
		qDebug() << "the userid :" << user.getUserId();
		qDebug() << "the userid from file:" << file.getUserId();*/
		emit downloadOver(index);
		inBlock.clear();
		byteReceived = 0;
		RtotalSize = 0;
		receiveTime = 0;
		newFile->close();
		disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
		//connect(tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	}
}

void DownloadFile::updateSpeed()
{
	emit updateSpeedLabel(index, speed, leftTime);
}

void DownloadFile::countLeftTime(float mleftTime)
{
	int hour = 0, min = 0, second = mleftTime;
	hour = second / 3600;
	second %= 3600;
	min = second / 60;
	second %= 60;
	leftTime = QString::number(hour).sprintf("%02d", hour) + ":"
		+ QString::number(min).sprintf("%02d", min) + ":"
		+ QString::number(second).sprintf("%02d", second);
}