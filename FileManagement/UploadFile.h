#pragma once
#ifndef UPLOADFILE_H  
#define UPLOADFILE_H   
#include "stdafx.h"
#include <QWidget>  
#include <QFile>  
#include <QTime>
#include <QTGui>
#include <string>  

#include "ui_UploadFile.h"


extern TcpClient * tcp;
extern QString globalUserName;
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
	QTime sendTime;  //����ʱ��
	QByteArray outBlock;  //�ִδ�  
	qint64 loadSize;  //ÿ�η������ݵĴ�С  
	qint64 byteToWrite;  //ʣ�����ݴ�С  
	qint64 totalSize;  //�ļ��ܴ�С  
	
	//��ȡ�ļ���ͼ��
	QIcon fileIcon(const QString &extension) const;
	//��ȡ�ļ������ͣ�
	QString fileType(const QString &extension) const;

	void init();
	int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ�����  

	void initFile();
	// ������ʽ�ļ�;
	void loadStyleSheet(const QString &sheetName);
	// �ļ��ϷŹ���ʵ��
	void dragEnterEvent(QDragEnterEvent* event = NULL);
	void dropEvent(QDropEvent* event = NULL);

	void setFileIcon(QString);  //����׺�Ž�ȥ

	
private slots:
	void send();  //�����ļ�ͷ��Ϣ  
	void goOnSend(qint64);  //�����ļ�����  
	void ClickOpenButton();
	void ClickSendButton();
	void receiveMainwindow();
};

#endif // UPLOADFILE_H  