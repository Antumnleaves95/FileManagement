#pragma once
#include "stdafx.h"
#include <QWidget>  
#include <QFile>  
#include <QTime>
#include <QTGui>
#include <string>  
#include <QStackedLayout>
#include <QFileDialog>
#include <QProgressBar>
#include "ui_UploadWindow.h"
#ifndef UPLOADWINDOW_H_
#define UPLOADWINDOW_H_

//�����

extern QString globalUserName;
extern QString ip;
extern int port;
class UploadWindow : public QWidget
{
	Q_OBJECT

public:
	explicit UploadWindow(QWidget *parent = 0);
	~UploadWindow();
	

private:
	Ui::UploadWindow *ui;

	//������Ҫ��Ҫѭ����ʾ
	QMap<QString,QProgressBar*> *fileProgressBarMap;  //QString�ǹؼ��֣�

	//QProgressBar fileProgressBar[6];  //����ϴ�5�����������ǵ�����
	QStackedLayout *stackLayout;	//  �����fileWidget��fileLayout
	QHBoxLayout *fileInfoLayout;	//��������ļ���Ϣ
	QWidget *fileWidget;  //��������ļ�����ͼ��
	QString fileName;  //�ļ���  
	QLabel *mFileName;
	QLabel *mFileIcon;  //�ļ�����ͼ���label
	QStringList fileNameList;
	QFileDialog *fileDialog;	//���ļ��Ի���
	QFile *localFile;


	//QTcpSocket *tcpSocket;
	//QThread *uploadThread;


	


	//QStringList fileNameList;  //�ļ����б�
	//QTime sendTime;  //����ʱ��
	//QByteArray outBlock;  //�ִδ�  
	//qint64 loadSize;  //ÿ�η������ݵĴ�С  
	//qint64 byteToWrite;  //ʣ�����ݴ�С  
	//qint64 totalSize;  //�ļ��ܴ�С  

					   //��ȡ�ļ���ͼ��
	QIcon fileIcon(const QString &extension) const;
	//��ȡ�ļ������ͣ�
	QString fileType(const QString &extension) const;

	void init();
	//int sendTimes;  //��������Ƿ�Ϊ��һ�η��ͣ���һ���Ժ������źŴ�������������ֶ�����  

	void initFile();
	// ������ʽ�ļ�;
	void loadStyleSheet(const QString &sheetName);
	// �ļ��ϷŹ���ʵ��
	void dragEnterEvent(QDragEnterEvent* event = NULL);
	void dropEvent(QDropEvent* event = NULL);

	void setFileIcon(QString);  //����׺�Ž�ȥ

	void insertFile(QString);  //�����ļ�����


private slots:
	//void send();  //�����ļ�ͷ��Ϣ  
	//void goOnSend(qint64);  //�����ļ�����  
	void ClickOpenButton();
	void ClickSendButton();
	void receiveMainwindow();

signals:
	void sendFileSignal();

};
#endif // !UPLOADWINDOW_H_



