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
	Q_OBJECT
public:
	explicit UploadFile(QString,int);
	~UploadFile();
	bool isOver = false;

	void send();  //�����ļ�ͷ��Ϣ  
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
	QString currentFileName;

	//void init(QString);
	int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ�����  

	void initFile();

	int index;  //�����IDֵ
	
private slots:
	
	void goOnSend(qint64);  //�����ļ�����  
	void receiveSendSignal();  //�����ź�

	//void receiveSendSignal();
signals:
	//���͵�ǰ���ݺ�������
	void sendOver(int);  //���ͽ���
	void updateProgress(int,qint64, qint64,double);

	
};

#endif // UPLOADFILE_H   