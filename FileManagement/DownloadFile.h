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


//��Ԫ��
class TableModel;
class ButtonDelegate;

struct FileInfo
{
	int fileId;
	QString fileName;
	QString fileSize;
	QString fileType;
	QString fileTime;
	QString fileUser;
	int userId;
};
class DownloadFile : public QWidget
{
	Q_OBJECT

public:
	explicit DownloadFile(QWidget *parent = 0);
	~DownloadFile();

	//���Ͳ鿴���û����ϴ��ļ�����Ϣ��������
	void sendFileInfo(QString condition = "all");
	// ������ʽ�ļ�;
	void loadStyleSheet(const QString &sheetName);

	//��ȡ�ļ���ͼ��
	QIcon fileIcon(const QString &extension) const;
	//��ȡ�ļ������ͣ�
	QString fileType(const QString &extension) const;

	// �����ļ��Ĵ�С�����������MB����KB
	QString countFileSize(QString);   
private:
	Ui::DownloadFile *ui;

	//QTcpSocket *tcpClient;
	QTime downloadTime;
	QFile *newFile;
	QByteArray inBlock;
	QByteArray buffer;  //���������
	QString fileName;
	QString saveFileName;   //�����·����������ֱ�����ļ�����

	QString openFileName;  //ֱ�Ӵ򿪵��ļ���
	qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	qint64 byteReceived;  //�Ѿ����յĴ�С  
	int receiveTime;	//���ܵĴ���

	QStandardItemModel *model;
	QToolButton *m_download;
	QToolButton *m_delete;
	
	//TableModel *m_model;
	ButtonDelegate *m_buttonDelegate;


	void init();
private slots:

	void receiveFile();  //�ӷ����������ļ������ء�
						 //void send();  //�����ļ�ͷ��Ϣ  
						 //void goOnSend(qint64);  //�����ļ����� 

	void ClickDownloadButton();
	void ClickDeleteButton();  // ɾ����ť
	bool saveFilePath(QString);   //ѡ������·��
	void showFileInfo();

	void initModel(); // ��ʼ��qtableview�����ǳ�ʼ��model

	void receiveDataFromClient(QString);

	
	void showToolTip(const QModelIndex &index);
	//void ClickSendButton();
	//void receiveMainwindow();

	//���������ѯ��ť
	void ClickFindButton();

signals:
	//void sendDisconnect(QString);  //����ʲô

};

inline QDataStream &operator>>(QDataStream &in, FileInfo &fileInfo)
{
	int fileId;
	QString fileName;
	QString fileSize;
	QString fileType;
	QString fileTime;
	QString fileUser;
	int userId;

	in >> fileId >> fileName >> fileSize
		>> fileTime >> fileType >> fileUser >> userId;


	fileInfo.fileId = fileId;
	fileInfo.fileName = fileName;
	fileInfo.fileSize = fileSize;
	fileInfo.fileType = fileType;
	fileInfo.fileTime = fileTime;
	fileInfo.fileUser = fileUser;
	fileInfo.userId = userId;

	return in;
}
#endif // !DOWNLOADFILE_H_ 

