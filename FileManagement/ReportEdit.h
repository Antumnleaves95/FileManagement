#pragma once
#include "stdafx.h"
#include "Report.h"
#include"ui_ReportEdit.h"

#ifndef REPORTEDIT_H_
#define REPORTEDIT_H_

extern TcpClient * tcp;    //ȫ�ֱ�����TCP ������
extern QString globalUserName;

class ReportEdit : public QWidget
{
	Q_OBJECT
public:
	ReportEdit(QWidget *parent = Q_NULLPTR);
	~ReportEdit();

	void sendReportEdit();  //���ͽ��ս�������

private:
	Ui::ReportEdit *ui;
	ReportInfo reportInfo;
	void init();
	void loadStyleSheet(const QString &sheetName);  //������ʽ

	

private slots:
	void receiveDataFromClient(QString);
	void showReporter();  //��ʾ�����˵���Ϣ
	void showLastWeek();
	void sendLastWeek();  //����
	void sendReport();  //���ͱ���
	void ClickSubmit(); 
	//void ClickReset();

};
#endif // !REPORTEDIT_H_
