#pragma once

#include "stdafx.h"
#include <QtWidgets/QMainWindow>
#include "UploadWindow.h"
#include "DownloadWindow.h"
#include "DownloadManage.h"
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


	UploadWindow *uploadWindow;  // �����ϴ��ļ���ָ��
	DownloadWindow *downloadWindow;
	DownloadManage *downloadManage; 
	UserInformation *userInformation; 
	ReportEdit *reportEdit;
	ReportLook *reportLook;
	
	QStackedLayout *m_pStackedLayout; 
	//�����л�
	void StackWindow();

	//��ʼ������
	void init();

	//�󶨴���֮����źŲ�
	void WindowSlotBind();  
	//��ʼ��������
	//void initTitleBar();
	
private slots:
	void Btn_Click();
	void ClickUserInformation();
	void ClickUploadWindow();
	void ClickDownloadWindow();
	void ClickDownloadManage();
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
