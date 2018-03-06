#pragma once

#include "stdafx.h"
#include <QtWidgets/QMainWindow>
#include "UploadFile.h"
#include "DownloadFile.h"
#include "UserInformation.h"
#include "ReportEdit.h"
#include "ReportLook.h"
#include "BaseWindow.h"
#include "ui_FileManagement.h"
#include <QStackedLayout>

extern QString globalUserName;
class FileManagement : public BaseWindow
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
	ReportEdit *reportEdit;
	ReportLook *reportLook;

	bool uploadFileWindowIsOpen = false,
		downloadFileWindowIsOpen = false,
		userInformationWindowIsOpen = false,
		reportEditWindowIsOpen = false,
		reportLookWindowIsOpen = false;

	QStackedLayout *m_pStackedLayout; 
	//�����л�
	void StackWindow();

	//��ʼ������
	void init();

	//��ʼ��������
	//void initTitleBar();
	
private slots:
	void Btn_Click();
	void ClickUserInformation();
	void ClickUploadFile();
	void ClickDownloadFile();
	void ClickReportEdit();
	void ClickReportLook();
	void ClickReturn();


	void receiveLogin();
signals:
	void showUploadFile();
	void showLogin();
	////��ʾ��ť
	//void showInfo();


};
