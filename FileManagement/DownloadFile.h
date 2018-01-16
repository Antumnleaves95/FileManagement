#pragma once
#ifndef DOWNLOADFILE_H_
#define DOWNLOADFILE_H_
#include "stdafx.h"
#include "ui_DownloadFile.h"
#include <QFile>  
#include <QString> 
extern TcpClient * tcp;
extern QString globalUserName;

class DownloadFile : public QWidget
{
	Q_OBJECT

public:
	explicit DownloadFile(QWidget *parent = 0);
	~DownloadFile();

	//���Ͳ鿴���û����ϴ��ļ�����Ϣ��������
	void sendFileInfo();

private:
	Ui::DownloadFile *ui;

	//QTcpSocket *tcpClient;
	QTime downloadTime;
	QFile *newFile;
	QByteArray inBlock;
	QString fileName;
	qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	qint64 byteReceived;  //�Ѿ����յĴ�С  
	int receiveTime;	//���ܵĴ���

private slots:

	void receiveFile();  //�ӷ����������ļ������ء�
						 //void send();  //�����ļ�ͷ��Ϣ  
						 //void goOnSend(qint64);  //�����ļ����� 

	void ClickDownloadButton();

	void showFileInfo();

	//void ClickSendButton();
	//void receiveMainwindow();

signals:
	void sendDisconnect(QString);

};
#endif // !DOWNLOADFILE_H_

