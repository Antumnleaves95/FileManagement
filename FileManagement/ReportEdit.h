#pragma once
#include "stdafx.h"
#include"ui_ReportEdit.h"

#ifndef REPORTEDIT_H_
#define REPORTEDIT_H_

extern TcpClient * tcp;    //ȫ�ֱ�����TCP ������
extern QString globalUserName;

struct ReportInfo
{
	QString reportId;
	QString reportName;
	QString reportNumber;
	QString reportLevel;
	QString reportTeacher;
	QString reportTerm;
	QString reportWeek;
	QString reportTime;
	QString reportMain;
	QString reportQuestionForSolve;
	QString reportQuestionHasSolved;
	QString reportQuestionFromLastWeek;
	QString reportYourGet;
	QString reportUserId;
};

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

	void sendReport();  //���ͱ���
	void ClickSubmit(); 
	//void ClickReset();

};
#endif // !REPORTEDIT_H_
