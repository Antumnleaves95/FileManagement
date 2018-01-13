#pragma once
#ifndef UPLOADFILE_H  
#define UPLOADFILE_H   
#include "stdafx.h"
#include <QWidget>  
#include <QFile>  
#include <string>  
#include "ui_UploadFile.h";


extern TcpClient * tcp;
class UploadFile : public QWidget
{
	Q_OBJECT

public:
	explicit UploadFile(QWidget *parent = 0);
	~UploadFile();

private:
	Ui::UploadFile *ui;

	//QTcpSocket *tcpClient;
	QFile *localFile;
	QString fileName;  //�ļ���  

	QByteArray outBlock;  //�ִδ�  
	qint64 loadSize;  //ÿ�η������ݵĴ�С  
	qint64 byteToWrite;  //ʣ�����ݴ�С  
	qint64 totalSize;  //�ļ��ܴ�С  

	int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ�����  

	private slots:
	void send();  //�����ļ�ͷ��Ϣ  
	void goOnSend(qint64);  //�����ļ�����  
	void ClickOpenButton();
	void ClickSendButton();

	void receiveMainwindow();
};

#endif // UPLOADFILE_H  