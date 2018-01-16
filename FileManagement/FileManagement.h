#pragma once

#include "stdafx.h"
#include <QtWidgets/QMainWindow>
#include "UploadFile.h"
#include "DownloadFile.h"
#include "UserInformation.h"
#include "ui_FileManagement.h"

extern QString globalUserName;
class FileManagement : public QMainWindow
{
	Q_OBJECT

public:
	FileManagement(QWidget *parent = Q_NULLPTR);
	~FileManagement();

private:
	Ui::FileManagement *ui;


	UploadFile *uploadFile;  // �����ϴ��ļ���ָ��
	DownloadFile *downloadFile;
	UserInformation *userInformation; 
	bool uploadFileWindowIsOpen = false, downloadFileWindowIsOpen = false
		,userInformationWindowIsOpen = false;

private slots:
	void Btn_Click();
	void ClickUserInformation();
	void ClickUploadFile();
	void ClickDownloadFile();
	void receiveLogin();
signals:
	void showUploadFile();
	
	////��ʾ��ť
	//void showInfo();


};
