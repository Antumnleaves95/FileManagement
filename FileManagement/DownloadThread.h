#pragma once
#ifndef DOWNLOADTHREAD_H_
#define DOWNLOADTHREAD_H_
#include "stdafx.h"
#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include "DownloadFile.h"
class QTcpSocket;
class DownloadThread : public QThread
{
	Q_OBJECT
public:
	DownloadThread(QString,QString,int, QObject *parent = 0);
	~DownloadThread();
	void run();
	DownloadFile *downloadFile;  // �ȶ���һ��DownloadFile
private:
	QString fileName;
	int index;
	QString filePath;  //�ļ�·��

signals:
	void ThreadClosed(int);  //�������ڷ����̹߳ر��ź�
};

#endif // !DOWNLOADTHREAD_H_



