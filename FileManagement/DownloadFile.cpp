#include "DownloadFile.h"
#include <QFileDialog>
#include <QtWin>
#include "MyMessageBox.h"
#include "Database.h"
DownloadFile::DownloadFile(QString mFileName, QString mFilePath,int num)
{
	fileName = mFileName;
	filePath = mFilePath;   //���ļ����� filePath + fileName��
	index = num;
	keepOn = false;
	init();

	qDebug() << "new downloadFile :" <<QThread::currentThreadId()
		<< keepOn;
}
//fileId�����ֿ��ܺ�filename��һ��
DownloadFile::DownloadFile(int mFileId,QString mFileName,QString mFilePath, qint64 mBreakPoint,qint64 mRecordId, int num)
{
	breakFileName = mFileName;
	breakFilePath = mFilePath;   //���ļ����� filePath + fileName��
	breakFileId = mFileId;
	cbreakPoint = mBreakPoint;
	recordId = mRecordId;
	index = num;
	keepOn = true;
	init();
}

DownloadFile::~DownloadFile()
{
	//delete ui;
}


//��ʼ������  
void DownloadFile::init()
{
	QString bs;
	QString data;
	tcpSocket = new QTcpSocket();  //�½�һ��socket
	tcpSocket->abort();   //ȡ�����е�����
	tcpSocket->connectToHost(ip, port);  //�����socket���ӵ�������
	byteReceived = 0;
	receiveTime = 0;
	if (!keepOn)
	{
		 bs = "downloadFile";
		 data = bs + "#" + fileName + "#" + globalUserName;
	}
	else
	{

		 bs = "downloadBreakFile";
		 //cbreakPoint�ϵ㴦����Ϊ����sumblock��ֵ
		 data = bs + "#" + QString::number(breakFileId) + "#" + QString::number(cbreakPoint) + "#" + globalUserName ;
	}

	QByteArray datasend = data.toUtf8();  //����UTF8��ȥ
	if (tcpSocket->write(datasend))
	{
		qDebug() << "send data to server: " << data;
		
		connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()),Qt::DirectConnection);  //���źŲ�

	}
	else
	{
		qDebug() << "send filed :" << data;
	}
}

void DownloadFile::receiveFile()
{
	outBlock.clear();
	QDataStream out(&outBlock, QIODevice::WriteOnly);
	if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
	{
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateSpeed())); // ***��������˵����Ӧ����
		timer->start(1000); // ÿ��1s�����ٶȱ�ǩ
 
		QDataStream in(tcpSocket);
		in >> RtotalSize >> byteReceived >> fileName>> fileId;
		sumBlock = RtotalSize / (4 * 1024) + 1;   //�ļ����������ܴ�С����ÿ�������
		qDebug() << "the file sumBlock :" << sumBlock;
		//filePath = "files/";
		saveFileName = filePath + fileName;
		qDebug() << "the file of head: " << fileName 
			<< "totalSize: " << RtotalSize 
			<< " first byteReceived: " << byteReceived
			<< "start saveFileName:" << saveFileName <<"fileId: "<<fileId;

		if(!keepOn)
		{
			qDebug() << "nimabi" << keepOn;
			//���һ���ļ���д���ظ��Ĺ���
			QString suffix = fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1); //��ȡ�ļ���׺;																	 //����ͷ������Ĳ�һ��
			QString fullName = fileName.left(fileName.size() - (fileName.size() - fileName.lastIndexOf('.'))); //��ȡ�ļ���

			int i = 0;																											   //��ѯ�ļ����Ƿ��ظ�
			while (1)
			{
				QFile *existFile = new QFile(filePath + fileName);  //�����ж��Ƿ��ظ�
																	//qDebug() << "exitsFile" << mFilePath + mFileName;
				if (existFile->exists())   //����ļ�����
				{
					i++;
					fileName = fullName + "(" + QString::number(i, 10) + ")." + suffix;

					//qDebug() << "the fileName :" << mFileName;
				}
				else
					break;
			}
			newFile = new QFile(filePath + fileName);  //�½�һ���ļ���д��
			newFile->open(QFile::WriteOnly);
			//
			//qint64 fileWrited = newFile->size();

			breakPoint = receiveTime;
		}
		//�����д�ϵ㣬��ֱ�Ӵ򿪲�Ҫ�½�
		else
		{
			qDebug() << "gogon!!!!!!!!!!!!!";
			newFile = new QFile(breakFilePath + breakFileName);  
			//appendģʽ�����ļ��ĺ���
			newFile->open(QFile::WriteOnly| QFile::Append);
			newFile->atEnd();
			receiveTime = cbreakPoint - 1;
			byteReceived += (cbreakPoint-1) * 4 * 1024;
			breakPoint = cbreakPoint - 1;
		}

		
		//��ʼ��ʱ��
		downloadTime.start();
		outBlock.clear();
		//qDebug() << outBlock;
		qDebug() << "the status:" << receiveStatus << "the sumBlock:" << sumBlock << "the breakPoint:" << breakPoint;
		out << receiveStatus << sumBlock << breakPoint;
		//qDebug() << outBlock;
		tcpSocket->write(outBlock);
		//tcpSocket->waitForBytesWritten();

	}
	else  //��ʽ��ȡ�ļ�����  
	{
		while (tcpSocket->bytesAvailable() > 0)
		{
			receiveTime++;
			breakPoint = receiveTime;
			if (tcpSocket->bytesAvailable() < 4 * 1024)
				inBlock = tcpSocket->read(tcpSocket->bytesAvailable());
			else
				inBlock = tcpSocket->read(4 * 1024);
			byteReceived += inBlock.size();
			float useTime = downloadTime.elapsed();
			speed = (byteReceived) / useTime;
			//����ʱ�䣬����һ��float
			countLeftTime(RtotalSize / speed / 1000 - useTime / 1000);

			//Ҫ���źŴ���0�����ֹͣ���䣬���Ͷϵ�λ�ø�������

			if (receiveStatus > 0)
			{
				qDebug() << "stop receive!";
				outBlock.clear();
				out << receiveStatus<<sumBlock<<breakPoint;
				if (insertRecord())
				{
					qDebug() << "insert downloadRecord success!";
				}
				else
				{
					qDebug() << "insert failed!";
				}
			}
			else
			{
				//���д����������������һ�����,-1Ϊ��������
				if (newFile->write(inBlock))
				{
					qDebug() << "write sucess!";
					newFile->flush();
					emit updateProgress(index, byteReceived, RtotalSize);
					receiveStatus = -1;
					outBlock.clear();

					qDebug() << "the status:" << receiveStatus << "the sumBlock:" << sumBlock << "the breakPoint:" << breakPoint;
					out << receiveStatus << sumBlock << breakPoint;
					qDebug() << "ThreadId: " << QThread::currentThreadId()
						<< fileName << " the " << receiveTime << "recv block " << inBlock.size()
						<< "current receive the byte is :" << byteReceived
						<< " and the total " << RtotalSize;
				}
				else
				{
					qDebug() << "write fail!";
					receiveStatus = -2;
					outBlock.clear();
					qDebug() << "the status:" << receiveStatus << "the sumBlock:" << sumBlock << "the breakPoint:" << breakPoint;
					out << receiveStatus << sumBlock << breakPoint;
				}
			}
			//qDebug() << "outBlock" << outBlock;
			tcpSocket->write(outBlock);  //д���ݸ�������
			tcpSocket->waitForBytesWritten();
		}
	}

	if (byteReceived == RtotalSize)
	{
		qDebug() << "receive is done, and the byteReceived is :" << byteReceived <<" the RtotalSize is :"<< RtotalSize;
		/*qDebug() << "the file name:" << file.getFileName();
		qDebug() << "the userid :" << user.getUserId();
		qDebug() << "the userid from file:" << file.getUserId();*/
		if (!keepOn)
		{
			if (insertRecord())
			{
				qDebug() << "insert downloadRecord success!";
			}
			else
			{
				qDebug() << "insert failed!";
			}
		}
		else
		{
			updateRecord();
		}


		inBlock.clear();
		byteReceived = 0;
		RtotalSize = 0;
		receiveTime = 0;
		newFile->close();
		timer->stop();
		disconnect(timer, SIGNAL(timeout()), this, SLOT(updateSpeed())); // ***��������˵����Ӧ����
		//receiveStatus = -3;
		//out << receiveStatus << sumBlock << breakPoint;
		//tcpSocket->write(outBlock);  //д���ݸ�������,˵�������
		disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
		//connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveBreak()));
		emit downloadOver(index);
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


//���յ�ֹͣ���͵��ź�
void DownloadFile::stopReceive(int)
{
	receiveStatus = receiveTime;
}

bool DownloadFile::insertRecord()
{
	QString sql = "insert into DownloadRecord(r_fileName,r_filePath,r_fileSize,r_sumBlock,r_breakPoint,r_fileId)values('"
		+ fileName + "','" + filePath + "','" 
		+ QString::number(RtotalSize) + "'," + QString::number(sumBlock) + "," 
		+ QString::number(breakPoint)+"," + QString::number(fileId) +  ")";
	qDebug() << sql;
	QSqlQuery insert;
	insert.exec(sql);
	if (insert.isActive())
	{
		qDebug() << sql;
		//��ѯ���µ�downloadRecord��¼
		QString sql1 = "select * from DownloadRecord order by r_Id desc limit 0,1";
		QSqlQuery query;
		query.exec(sql1);
		BreakFile breakFile;
		if (query.next())
		{
			breakFile.recordId = query.value(0).toInt();
		}
		
		breakFile.fileId = fileId;
		breakFile.fileName = fileName;
		breakFile.breakPoint = breakPoint;
		breakFile.filePath = filePath;
		////�����breakFile���͹�ȥ
		//qint64 recordId, int fileId,
		//	QString fileName, QString filePath,
		//	qint64 breakPoint
		emit addToBreakFile(index, breakFile.recordId, breakFile.fileId,
			breakFile.fileName, breakFile.filePath, breakFile.breakPoint);

		return true;
	}

	return false;
}

bool DownloadFile::updateRecord()
{
	QString sql = "update DownloadRecord set r_breakPoint = " + QString::number(breakPoint) + " where r_Id = " + QString::number(recordId) + ";";
	qDebug() << sql;
	QSqlQuery insert;
	insert.exec(sql);
	if (insert.isActive())
	{
		qDebug() << sql;
		return true;
	}
	return false;
}

