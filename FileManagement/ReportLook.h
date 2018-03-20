#pragma once

#ifndef REPORTLOOK_H_
#define REPORTLOOK_H_
#include "stdafx.h"
#include "Report.h"
#include "ui_ReportLook.h"
#include <QStandardItemModel>
#include <qtoolbutton.h>
extern TcpClient * tcp;    //ȫ�ֱ�����TCP ������
extern QString globalUserName;

class ReportLook :public QWidget
{
	Q_OBJECT

		//public д������
public:
	ReportLook(QWidget *parent = Q_NULLPTR);
	~ReportLook();

	//���Ͳ鿴��������������������Ĭ��ΪALL
	void sendReportLook(QString condition = "all");

	// ������ʽ�ļ�;
	void loadStyleSheet(const QString &sheetName);

	//��ʾ�ı�
	

private:
	Ui::ReportLook *ui;

	QStandardItemModel *model;
	QToolButton *m_look;
	QToolButton *m_delete;

	QComboBox *TermComboBox;
	QComboBox *WeekComboBox;



	//�ж�������combox�Ƿ��Ѿ�������
	bool isWeekCombox = false,
		isTermCombox = false;

private slots:
	//��ʾ�ӷ��������յ�list
	void showReportList();

	void receiveDataFromClient(QString);
	// ��ʼ��qtableview�����ǳ�ʼ��model
	void initModel(); 

	void init();
	void showToolTip(const QModelIndex &index);

	//���������ѯ��ť
	void ClickFindButton();

	//���ѧ�ں����������combobox
	void insertWeekComboBox(int);
	void insertTermComboBox(int);
	//�鿴�ҵı���İ�ť
	void ClickLookMyReportButton();

	//�鿴���水ť
	void ClickLookReportButton();

	//ɾ�����水ť
	void ClickDeleteButton();


};
#endif // !REPORTLOOK_H_
