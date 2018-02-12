#pragma once
#include "stdafx.h"
#include "Report.h"
#include "ui_ReportLook.h"
#include <QStandardItemModel>
#ifndef REPORTLOOK_H_
#define REPORTLOOK_H_

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
	void showToolTip(const QModelIndex &index);

private:
	Ui::ReportLook *ui;

	QStandardItemModel *model;
	QPushButton *m_look;
	QPushButton *m_delete;

private slots:
	//��ʾ�ӷ��������յ�list
	void showReportList();

	//void receiveDataFromClient(QString);
	// ��ʼ��qtableview�����ǳ�ʼ��model
	void initModel(); 

	//���������ѯ��ť
	void ClickFindButton();

	//�鿴�ҵı���İ�ť
	void ClickLookMyReportButton();

	//�鿴���水ť
	void ClickLookReportButton();



};
#endif // !REPORTLOOK_H_
