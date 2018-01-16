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

private:
	Ui::DownloadFile *ui;

	//QTcpSocket *tcpClient;
	QTime downloadTime;
	QFile *newFile;
	QByteArray inBlock;
	QString fileName;
	qint64 RtotalSize;  //总共需要发送的文件大小（文件内容&文件名信息）  
	qint64 byteReceived;  //已经接收的大小  
	int receiveTime;	//接受的次数

private slots:

	void receiveFile();  //从服务器下载文件到本地。
						 //void send();  //传送文件头信息  
						 //void goOnSend(qint64);  //传送文件内容 

	void ClickDownloadButton();
	//void ClickSendButton();
	//void receiveMainwindow();

signals:
	void sendDisconnect(QString);

};
#endif // !DOWNLOADFILE_H_

