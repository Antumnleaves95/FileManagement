#pragma once
#ifndef DOWNLOADFILE_H_
#define DOWNLOADFILE_H_
#include "stdafx.h"
#include "ui_DownloadWindow.h"
#include <QFile>  
#include <QString> 
#include "FileInfo.h"
//extern TcpClient * tcp;
extern QString ip;
extern int port;
extern QString globalUserName;


//ֻ�������ؾ�����
class DownloadFile : public QObject
{
	Q_OBJECT

public:
	explicit DownloadFile(QString,QString,int);
	~DownloadFile();

	//���Ͳ鿴���û����ϴ��ļ�����Ϣ��������
	//void sendFileInfo(QString condition = "all");
	// ������ʽ�ļ�;
	//void loadStyleSheet(const QString &sheetName);

	//��ȡ�ļ���ͼ��
	//QIcon fileIcon(const QString &extension) const;
	//��ȡ�ļ������ͣ�
	//QString fileType(const QString &extension) const;

	// �����ļ��Ĵ�С�����������MB����KB
	//QString countFileSize(QString);   

private:
	//Ui::DownloadFile *ui;
	int index;  //���
	QTcpSocket *tcpSocket;   //���ӷ�������socket
	QTime downloadTime;
	QFile *newFile;
	QByteArray inBlock;
	QByteArray buffer;  //���������
	QString fileName;
	QString saveFileName;   //�ļ�·�����ļ���
	QString filePath;
	QString openFileName;  //ֱ�Ӵ򿪵��ļ���

	qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	qint64 byteReceived;  //�Ѿ����յĴ�С  
	int receiveTime;	//���ܵĴ���
	double speed;

	QString leftTime;  //ʣ��ʱ��

	void init();
	void countLeftTime(float);

private slots:

	void receiveFile();  //�ӷ����������ļ������ء�
						 //void send();  //�����ļ�ͷ��Ϣ  
						 //void goOnSend(qint64);  //�����ļ����� 
	void updateSpeed();

	//void ClickDownloadButton();
	//void ClickDeleteButton();  // ɾ����ť
	//bool saveFilePath(QString);   //ѡ������·��
	//void showFileInfo();

	//void initModel(); // ��ʼ��qtableview�����ǳ�ʼ��model

	//void receiveDataFromClient(QString);

	
	//void showToolTip(const QModelIndex &index);
	//void ClickSendButton();
	//void receiveMainwindow();

	//���������ѯ��ť
	//void ClickFindButton();

signals:
	//void sendDisconnect(QString);  //����ʲô
	void downloadOver(int);
	void updateProgress(int, qint64, qint64);
	void updateSpeedLabel(int, double,QString);

};

//inline QDataStream &operator>>(QDataStream &in, FileInfo &fileInfo)
//{
//	int fileId;
//	QString fileName;
//	QString fileSize;
//	QString fileType;
//	QString fileTime;
//	QString fileUser;
//	int userId;
//
//	in >> fileId >> fileName >> fileSize
//		>> fileTime >> fileType >> fileUser >> userId;
//
//
//	fileInfo.fileId = fileId;
//	fileInfo.fileName = fileName;
//	fileInfo.fileSize = fileSize;
//	fileInfo.fileType = fileType;
//	fileInfo.fileTime = fileTime;
//	fileInfo.fileUser = fileUser;
//	fileInfo.userId = userId;
//
//	return in;
//}
#endif // !DOWNLOADFILE_H_ 

