#pragma once
#ifndef DOWNLOADFILE_H_
#define DOWNLOADFILE_H_
#include "stdafx.h"
#include "ui_DownloadFile.h"
#include <QFile>  
#include <QString> 
#include <QStandardItemModel>
extern TcpClient * tcp;
extern QString globalUserName;


struct FileInfo
{
	int fileId;
	QString fileName;
	QString fileSize;
	QString fileType;
	int userId;
};
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
	QString saveFileName;   //�����·����������ֱ�����ļ�����

	QString openFileName;  //ֱ�Ӵ򿪵��ļ���
	qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	qint64 byteReceived;  //�Ѿ����յĴ�С  
	int receiveTime;	//���ܵĴ���
	QStandardItemModel *model;
	QPushButton *mDownloadButton;


private slots:

	void receiveFile();  //�ӷ����������ļ������ء�
						 //void send();  //�����ļ�ͷ��Ϣ  
						 //void goOnSend(qint64);  //�����ļ����� 

	void ClickDownloadButton();
	void saveFilePath(QString);   //ѡ������·��
	void showFileInfo();

	//void ClickSendButton();
	//void receiveMainwindow();

signals:
	void sendDisconnect(QString);

};
#endif // !DOWNLOADFILE_H_

