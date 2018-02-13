#include "ReportLook.h"
#include "MyMessageBox.h"
#include "ReportDetail.h"
#include <QToolTip>


ReportLook::ReportLook(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ReportLook)
{
	ui->setupUi(this);
	initModel();

	ui->tableView->setMouseTracking(true);   //�������׷��

	connect(ui->tableView, SIGNAL(entered(QModelIndex)),
		this, SLOT(showToolTip(QModelIndex)));
}
ReportLook::~ReportLook()
{
	delete ui;
}

//���ñ�ͷ�Լ�һЩ����
void ReportLook::initModel()
{
	model = new QStandardItemModel();
	model->setColumnCount(9);
	ui->tableView->setShowGrid(false);
	ui->tableView->verticalHeader()->setVisible(false);// ��ֱ���ɼ�
	//ui->tableView->horizontalHeader()->setVisible(false);// ˮƽ���ɼ�
	model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit(""));
	model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("�û���"));
	model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("ѧ��"));
	model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("����"));
	model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("���ݸ�Ҫ"));
	model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("ʱ��"));
	model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("����"));
	model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("�鿴"));
	model->setHeaderData(8, Qt::Horizontal, QString::fromLocal8Bit("ɾ��"));

	ui->tableView->setModel(model);
	ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui->tableView->setColumnWidth(0, 30);
	ui->tableView->setColumnWidth(1, 80);
	ui->tableView->setColumnWidth(2, 50);
	ui->tableView->setColumnWidth(3, 30);
	ui->tableView->setColumnWidth(4, 120);
	ui->tableView->setColumnWidth(5, 120);
	ui->tableView->setColumnWidth(6, 50);
	ui->tableView->setColumnWidth(7, 40);
	ui->tableView->setColumnWidth(8, 40);

	//�����п��ɱ� 
	ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);
}

void ReportLook::showToolTip(const QModelIndex &index) {
	if (!index.isValid()) {
		qDebug() << "Invalid index";
		return;
	}
	QToolTip::showText(QCursor::pos(), index.data().toString());
}

void ReportLook::sendReportLook(QString condition)
{
	//ÿ�δ򿪸�ҳ���򷢲�ѯ��Ϣ����ȥ
	QString data = "lookReport#" + condition;   //��ѯ���е��ļ�
	QByteArray datasend = data.toUtf8();
	qDebug() << datasend;
	tcp->tcpSocket->write(datasend);

	//��ʱȡ��TCPԭ������֤��Ϣ
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showReportList()));
}

void ReportLook::showReportList()
{
	initModel();
	QByteArray dataread = tcp->tcpSocket->readAll();
	QString data = QString::fromUtf8(dataread);
	qDebug() << "the data from client: " << dataread;
	QStringList listNumber = data.split("$");

	QList<ReportInfo> reportInfo;
	//�������Ҫ���뵽����ȥ
	//�����ݶ��ŵ�QList��ȥ��
	//listNumber[0] ���ļ�����
	for (int i = 1;i <= listNumber[0].toInt();i++)
	{
		ReportInfo littleReport;
		QStringList reportList = listNumber[i].split("#");
		littleReport.reportId = reportList[0];  //ID����Ҫ��ʾ
		littleReport.reportUserName = reportList[1];
		littleReport.reportTerm = reportList[2];
		littleReport.reportWeek = reportList[3];
		littleReport.reportMain = reportList[4];
		littleReport.reportTime = reportList[5];
		littleReport.reportName = reportList[6];
		littleReport.reportQuestionForSolve = reportList[7];
		littleReport.reportQuestionHasSolved = reportList[8];
		littleReport.reportQuestionFromLastWeek = reportList[9];
		littleReport.reportYourGet = reportList[10];
		littleReport.reportUserId = reportList[11].toInt();  //���Ҳ����Ҫ��ʾ

		qDebug() << littleReport.reportId << " " << littleReport.reportUserName << " " <<
			littleReport.reportTerm << " " << littleReport.reportWeek << " "
			<< littleReport.reportMain << " " << littleReport.reportTime << " " << littleReport.reportName;
		//�����������뱨��list
		reportInfo.append(littleReport);
	}
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < listNumber[0].toInt(); i++)
	{
		//����ǰ���е�����
		model->setItem(i, 1, new QStandardItem(reportInfo.at(i).reportUserName));
		model->setItem(i, 2, new QStandardItem(reportInfo.at(i).reportTerm));  //countFileSize���ݴ�С��ʾKB����MB
		model->setItem(i, 3, new QStandardItem(reportInfo.at(i).reportWeek));
		model->setItem(i, 4, new QStandardItem(reportInfo.at(i).reportMain));
		model->setItem(i, 5, new QStandardItem(reportInfo.at(i).reportTime));
		model->setItem(i, 6, new QStandardItem(reportInfo.at(i).reportName));

		//Ϊ�����������Ӱ�ť
		m_look = new QPushButton();
		QIcon lookReport("Resource/ion/lookReport.png"); //����QIcon����
		m_look->setIcon(lookReport); //��ͼƬ���õ���ť��
		m_look->setIconSize(QSize(20, 20));//����ʵ�ʵ���ͼƬ��С
		m_look->setStyleSheet("border:none");

		m_delete = new QPushButton();
		QIcon deleteReport("Resource/ion/deleteFile.png"); //����QIcon����
		m_delete->setIcon(deleteReport); //��ͼƬ���õ���ť��
		m_delete->setIconSize(QSize(20, 20));//����ʵ�ʵ���ͼƬ��С
		m_delete->setStyleSheet("border:none");

		//�������ذ�ť�Ĳۺ���
		connect(m_look, SIGNAL(clicked(bool)), this, SLOT(ClickLookReportButton()));
		connect(m_delete, SIGNAL(clicked(bool)), this, SLOT(ClickDeleteButton()));
		//ֱ�Ӱ��ļ�������row���ˣ�

		m_look->setProperty("row", i);  //Ϊ��ť����row����

		m_look->setProperty("reportId", reportInfo.at(i).reportId);  //Ϊ��ť����filename����


		m_delete->setProperty("row", i);
		m_delete->setProperty("deleteReportId", reportInfo.at(i).reportId);


		ui->tableView->setIndexWidget(model->index(model->rowCount() - 1, 7), m_look);
		ui->tableView->setIndexWidget(model->index(model->rowCount() - 1, 8), m_delete);

		//��������һ��ͷ����ʱû��

		//QIcon icon = fileIcon(fileInfo.at(i).fileType);
		//QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(20, 30)));
		//QLabel *fIcon = new QLabel();
		//fIcon->setPixmap(pixmap);
		//ui->downloadTable->setIndexWidget(model->index(model->rowCount() - 1, 0), fIcon);

		//�������ݾ���
		for (int j = 2;j < 7;j++)
			model->item(i, j)->setTextAlignment(Qt::AlignCenter);
	}
	//m_model->setData(data);
	//�����Ͽ�
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showReportList()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));

}

void ReportLook::ClickLookReportButton()
{
	QPushButton *btn = (QPushButton *)sender();   //�����¼��Ķ��󣬰���˭��ȡ˭
	QString row = btn->property("row").toString();  //ȡ�ð�ť��row����
	qDebug() << "the row is: " << row;
	
	QString ReportId = btn->property("reportId").toString();  //��ȡ��ť��id����
	qDebug() << "the ReportId is: " << ReportId;
	//ֱ�ӵ���һ��ReportDetail�������
	ReportDetail *reportDetail = new ReportDetail(ReportId);
	////���ô��ڱ���ɫ
	//QPalette Pal(palette());
	//// set black background
	//Pal.setColor(QPalette::Background, Qt::green);
	//reportDetail->setAutoFillBackground(true);
	//reportDetail->setPalette(Pal);
	//reportDetail->raise();
	reportDetail->show();

}

//�鿴�ҵı���
void ReportLook::ClickLookMyReportButton()
{
	//�����û�����ȥ
	sendReportLook("UserName#" + globalUserName);
}

void ReportLook::ClickFindButton()
{
	//����Ҫ�ж�һ��combox�����ʲôֵ��
}
