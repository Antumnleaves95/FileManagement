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
	//������캯�������ضϵ��ļ�����һ���������ݣ��ڶ����Ƕϵ�λ��
	DownloadFile(int,QString,QString, qint64 ,qint64 ,int );
	~DownloadFile();
	bool insertRecord();
	bool updateRecord();
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
	QByteArray outBlock;  //�����ݸ�������
	QByteArray buffer;  //���������
	QString fileName;
	QString saveFileName;   //�ļ�·�����ļ���
	QString filePath;
	QString openFileName;  //ֱ�Ӵ򿪵��ļ���

	qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	qint64 byteReceived;  //�Ѿ����յĴ�С  ����

	qint64 receiveStatus = -1;  //Ĭ��Ϊ��������
	qint64 sumBlock;
	qint64 breakPoint;
	qint64 cbreakPoint;
	qint64 recordId;
	int breakFileId;
	QString breakFileName;
	QString breakFilePath;

	qint64 fileId;
	int receiveTime;	//���ܵĴ���
	double speed;

	QString leftTime;  //ʣ��ʱ��
	bool isBreakFile = false;
	void init();

	bool keepOn;
	void countLeftTime(float);
	

private slots:

	void receiveFile();  //�ӷ����������ļ������ء�
	void receiveBreakFile();
	void updateSpeed();
	void stopReceive(int);
	

	
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

	void addToBreakFile(int, qint64 recordId,int fileId,
		QString fileName,QString filePath,
	qint64 breakPoint);

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

