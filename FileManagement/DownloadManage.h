#pragma once
#ifndef DOWNLOADMANAGE_H_
#define DOWNLOADMANAGE_H_
#include "stdafx.h"
#include "ui_DownlaodManage.h"
#include <QStandardItemModel>
#include <QMap>
#include <QFile>
#include "FileInfo.h"
#include "DownloadThread.h"
extern TcpClient * tcp;
extern QString globalUserName;


class DownloadManage : public QWidget
{
	Q_OBJECT

public:
	explicit DownloadManage(QWidget *parent = 0);
	~DownloadManage();
	void initModel();   //��ʼ��TableView����ֱ�Ӳ���ȽϺã�
	void initMap();  //��ʼ��Map
	void loadStyleSheet(const QString &sheetName);
	QIcon fileIcon(const QString &extension) const;
	//��ȡ�ļ������ͣ�
	QString fileType(const QString &extension) const;

	void setFileIcon(QString);  //����׺�Ž�ȥ

	QString countFileSize(QString);

private:
	Ui::DownloadManage *ui;
	QStandardItemModel *model;


	QMap<int, DownloadThread*> *downloadThreadMap;
	QMap<int, QProgressBar*> *downloadProgressBarMap;
	QMap<int, QLabel*> *downloadSpeedMap;
	QMap<int, QWidget*> *downloadWidgetMap;  //��һ��ֻ��QWidgetʵ�֣����԰���Ҫ����QTableView����ȥ�����ǲ���
	QMap<int, QLabel*> *downloadLeftTimeMap;
	QMap<int, QLabel*> *fileSizeMap;
	int index;

	DownloadThread *downloadThread;  //�����߳�
	QWidget *IconName;  //ͬʱ��Icon���ļ���

	QLabel *mFileName;   //��label
	QLabel *mFileIcon;  //�ļ�����ͼ���label
	QLabel *mFileSpeed;
	QLabel *mFileSize;
	QLabel *mLeftTime;
	QFile *localFile;  //

private slots:
	void showToolTip(const QModelIndex &index);
	void insertDownloadFile(QString name,QString path); 
	void beginToDownload(int); 
	void checkDownloadOver(int);
	void updateProgressBar(int, qint64, qint64);
	void updateSpeedLabel(int, double, QString);

	//void addNewDownloadFile();
};
#endif // !DOWNLOADMANAGE_N_
	 