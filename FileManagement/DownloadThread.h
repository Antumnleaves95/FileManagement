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
	DownloadThread(QString,QString,int,bool, QObject *parent = 0);
	DownloadThread(int,QString,QString, qint64, qint64,int, bool, QObject *parent = 0);
	~DownloadThread();
	void run();
	DownloadFile *downloadFile;  // �ȶ���һ��DownloadFile
private:
	QString fileName;
	int index;
	QString filePath;  //�ļ�·��
	int fileId;
	qint64 breakPoint;
	bool keepOn;  //�ϵ����ػ��ǳ�ʼ����
	qint64 recordId;
	private slots:
	void quitThread();   //�˳��߳�

signals:
	void ThreadClosed(int);  //�������ڷ����̹߳ر��ź�
	void downloadAvailable(int);
};

#endif // !DOWNLOADTHREAD_H_



