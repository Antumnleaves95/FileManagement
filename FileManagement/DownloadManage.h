#pragma once
#ifndef DOWNLOADMANAGE_H_
#define DOWNLOADMANAGE_H_
#include "stdafx.h"
#include "ui_DownlaodManage.h"
#include <QStandardItemModel>
#include <QMap>
#include <QFile>
#include <QMenu>
#include <QProgressBar>
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
	//��ʼ��TableView����ֱ�Ӳ���ȽϺã�
	void initModel();  
	//��ʼ��Map
	void initMap();  
	void loadStyleSheet(const QString &sheetName);
	QIcon fileIcon(const QString &extension) const;
	//��ȡ�ļ������ͣ�
	QString fileType(const QString &extension) const;
	//����׺�Ž�ȥ
	void setFileIcon(QString); 
	QString countFileSize(QString);
	QString checkRename(QString mFileName, QString mFilePath);
	//��ѯ���ص����ؼ�¼
	void queryLocalCache();
	void setMenuEvent();

private:
	Ui::DownloadManage *ui;
	QStandardItemModel *model;


	QMap<int, DownloadThread*> *downloadThreadMap;
	QMap<int, QProgressBar*> *downloadProgressBarMap;
	QMap<int, QLabel*> *downloadSpeedMap;
	QMap<int, QWidget*> *downloadWidgetMap;  //��һ��ֻ��QWidgetʵ�֣����԰���Ҫ����QTableView����ȥ�����ǲ���
	QMap<int, QLabel*> *downloadLeftTimeMap;
	QMap<int, QLabel*> *fileSizeMap;
	//QMap<int, int> *fileIdMap;
	//QMap<int, QString> *fileNameMap;
	QMap<int, BreakFile> *breakFileMap;
	QMap<int, int> *fileStatusMap; //״̬0Ϊ������ϣ�1Ϊ�����У�2Ϊ��ͣ
	int index;

	DownloadThread *downloadThread;  //�����߳�
	QWidget *IconName;  //ͬʱ��Icon���ļ���

	QLabel *mFileName;   //��label
	QLabel *mFileIcon;  //�ļ�����ͼ���label
	QLabel *mFileSpeed;
	QLabel *mFileSize;
	QLabel *mLeftTime;
	QFile *localFile;  //


	//�˵�
	QMenu *popMenu;
	QAction *actionWait;
	QAction *actionBegin;
	QAction *actionDelete;
	QAction *actionOpenFile;
	QAction *actionOpenFolder;

private slots:
	void showToolTip(const QModelIndex &index);
	void insertDownloadFile(QString name,QString path); 

	void insertDownloadRecord(QString, QString,QString,qint64,qint64);
	void beginToDownload(int); 
	void checkDownloadOver(int);
	void updateProgressBar(int, qint64, qint64);
	void updateSpeedLabel(int, double, QString);
	void slotContextMenu(QPoint pos);

	//�Ҽ��˵�
	void CallWaitDownload();
	void CallKeepOnDownload();
	void CallOpenFile();
	void CallOpenFolder();
	//void CallDelete();
	void addBreakFile(int, qint64 recordId, int fileId,
		QString fileName, QString filePath,
		qint64 breakPoint);

signals:
	void stopDownload(int);
	//void addNewDownloadFile();
};
#endif // !DOWNLOADMANAGE_N_
	 