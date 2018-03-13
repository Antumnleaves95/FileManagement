#include "DownloadThread.h"

DownloadThread::DownloadThread(QString mFileName,QString mFilePath ,int num, QObject *parent) :
	QThread(parent)
{
	fileName = mFileName;
	filePath = mFilePath;
	index = num;
	qDebug() << "download Thread gou zao :" << currentThreadId();
}


DownloadThread::~DownloadThread()
{
	requestInterruption();
	quit();
	wait();
	emit ThreadClosed(index);
	qDebug() << "the downloadThread has closed,the id is: " << currentThreadId();
}

void DownloadThread::run()
{
	while (!isInterruptionRequested())
	{
		qDebug() << "download Thread run :" << currentThreadId();
		downloadFile = new DownloadFile(fileName, filePath, index);
		emit downloadAvailable(index);  //��window����һ���߳��Ѿ��������ź�
		exec();
	}
}

void DownloadThread::quitThread()
{
	//exit();
	//qDebug() << "this Thread is closed!";
	//if (this->wait())
	//{
	//	qDebug() << "this Thread is closed!";
	//}
	this->~DownloadThread();
}