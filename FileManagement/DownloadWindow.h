#pragma once
#ifndef DOWNLOADWINDOW_H_
#define DOWNLOADWINDOW_H_
#include "stdafx.h"
#include "ui_DownloadWindow.h"
#include <QFile>  
#include <QString> 
#include <QStandardItemModel>
#include <QMap>
#include "DownloadFile.h"
#include "DownloadThread.h"
#include "FileInfo.h"
extern TcpClient * tcp;
extern QString globalUserName;


//�������Ҫ�������Ž���ģ��Լ�һЩ��Ϣ

//��Ԫ��
class TableModel;
class ButtonDelegate;


class DownloadWindow : public QWidget
{
	Q_OBJECT

public:
	explicit DownloadWindow(QWidget *parent = 0);
	~DownloadWindow();

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
	Ui::DownloadWindow *ui;

	//����һ�����ƣ���������̫�࣡
	QMap<int, DownloadThread*> *downloadThreadMap;  //�����߳�map

	//QTcpSocket *tcpClient;
	QTime downloadTime;
	QFile *newFile;
	QByteArray inBlock;
	QByteArray buffer;  //���������
	QString fileName;
	QString saveFileName;   //�����·��
	QString openFileName;  //�����ť���õ�������


	
	//qint64 RtotalSize;  //�ܹ���Ҫ���͵��ļ���С���ļ�����&�ļ�����Ϣ��  
	//qint64 byteReceived;  //�Ѿ����յĴ�С  
	//int receiveTime;	//���ܵĴ���

	QMap<int, DownloadFile*> *downloadFileMap;  //����һ�����ص�map�߳�
	int index; //�߳����
	QStandardItemModel *model;
	QToolButton *m_download;
	QToolButton *m_delete;

	//TableModel *m_model;
	ButtonDelegate *m_buttonDelegate;


	void initWindow();
	void init();

	void addDownloadFile(QString, QString, int); //�����ť������һ�������߳�
private slots:

	//void receiveFile();  //�ӷ����������ļ������ء�
	//					 //void send();  //�����ļ�ͷ��Ϣ  
	//					 //void goOnSend(qint64);  //�����ļ����� 
	void ClickDownloadButton();
	void ClickDeleteButton();  // ɾ����ť
	bool saveFilePath(QString);   //ѡ������·��
	void showFileInfo();
	void initModel(); // ��ʼ��qtableview�����ǳ�ʼ��model
	void receiveDataFromClient(QString);
	void showToolTip(const QModelIndex &index);
	//���������ѯ��ť
	void ClickFindButton();

	void receiveThreadClosed(int);

signals:
	//void sendDisconnect(QString);  //����ʲô
	void newDownloadFile(QString, QString, int);  //����һ���µ��źţ������ع�������һ���µ�

};


#endif // !DownloadWindow_H_ 

