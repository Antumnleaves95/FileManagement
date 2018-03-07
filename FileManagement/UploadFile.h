#pragma once
#ifndef UPLOADFILE_H  
#define UPLOADFILE_H   
#include "stdafx.h"
#include <QObject>  
#include <QFile>  
#include <QTime>
#include <QTGui>
#include <string>  
//extern TcpClient * tcp;
extern QString globalUserName;
extern QString ip;
extern int port;
class UploadFile : public QObject
{
public:
	explicit UploadFile(QFile);
	~UploadFile();

private:

	QTcpSocket *tcpSocket;
	/*QThread *uploadThread;*/

	QFile *localFile;
	QString fileName;  //�ļ���  
	QTime sendTime;  //����ʱ��
	QByteArray outBlock;  //�ִδ�  
	qint64 loadSize;  //ÿ�η������ݵĴ�С  
	qint64 byteToWrite;  //ʣ�����ݴ�С  
	qint64 totalSize;  //�ļ��ܴ�С  
	

	void init();
	int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ�����  

	void initFile();


	
private slots:
	void send();  //�����ļ�ͷ��Ϣ  
	void goOnSend(qint64);  //�����ļ�����  

	void receiveSendSignal();

};

#endif // UPLOADFILE_H  