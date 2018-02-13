
#pragma once
#include "stdafx.h"
#include "Report.h"
#include "BaseWindow.h"
#include"ui_ReportDetail.h"

#ifndef REPORTDETAIL_H_
#define REPORTDETAIL_H_

extern TcpClient * tcp;    //ȫ�ֱ�����TCP ������
extern QString globalUserName;



class ReportDetail : public BaseWindow
{
	Q_OBJECT
public:
	ReportDetail(QString,QWidget *parent = Q_NULLPTR);
	~ReportDetail();

	void sendReportDetail(QString);  //���ͽ��ս�������

private:
	Ui::ReportDetail *ui;
	ReportInfo reportInfo;
	void init();
	//void initTitleBar();



private slots:
	void receiveDataFromClient(QString);
	void showReport();  //��ʾ�����˵���Ϣ
	
	//�޸�
	//void sendReport();  //���ͱ���

	//���ɾ����ť
	void ClickDeleteButton();

	//����޸İ�ť
	void ClickUpdateButton();
	//void ClickReset();

};
#endif // !REPORTDetail_H_
