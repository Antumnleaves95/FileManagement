#include "ReportDetail.h"
#include "MyMessageBox.h"


ReportDetail::ReportDetail(QString reportId,QWidget *parent) :
	BaseWindow(parent),
	ui(new Ui::ReportDetail)
{
	ui->setupUi(this);

	
	connect(tcp, SIGNAL(sendDataToReportDetail(QString)), this, SLOT(receiveDataFromClient(QString)));
	


	//�մ򿪵�ʱ��Ҫѡ�����ťʱ�������е�ID�ı���
	sendReportDetail(reportId);
	init();
	//setFixedSize(700, 500); // ��ֹ�ı䴰�ڴ�С
	//initTitleBar();
}

ReportDetail::~ReportDetail()
{
	delete ui;
}

void ReportDetail::init()
{
	loadStyleSheet("ReportDetail");
	//����Ϊֻ��ģʽ
	ui->reportMainPlainTextEdit->setReadOnly(true);
	ui->reportQuestionForSolve->setReadOnly(true);
	ui->reportQuestionFromLastWeek->setReadOnly(true);
	ui->reportQuestionHasSolved->setReadOnly(true);
	ui->reportYourGet->setReadOnly(true);

	QPalette Pal(palette());
	// set black background
	Pal.setColor(QPalette::Background, Qt::green);
	this->setAutoFillBackground(true);
	this->setPalette(Pal);
	this->raise();

}
//
//void ReportDetail::initTitleBar()
//{
//	m_titleBar->move(1, 2);
//	m_titleBar->raise();
//	m_titleBar->setWindowBorderWidth(2);
//	m_titleBar->setBackgroundColor(138, 109, 53);
//	m_titleBar->setButtonType(MIN_BUTTON);
//	m_titleBar->setTitleWidth(this->width());
//}


void ReportDetail::sendReportDetail(QString reportId)
{
	//ÿ�δ򿪸�ҳ���򷢲�ѯ��Ϣ����ȥ
	QString data = "lookReport#reportId#" + reportId;   //��ѯ������
	QByteArray datasend = data.toUtf8();
	qDebug() << datasend;
	tcp->tcpSocket->write(datasend);

	//��ʱȡ��TCPԭ������֤��Ϣ
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showReport()));
}

void ReportDetail::showReport()
{
	QByteArray dataread = tcp->tcpSocket->readAll();
	QString data = QString::fromUtf8(dataread);
	qDebug() << "the data from client: " << data;

	//��Щ�����ǿյ�
	QStringList reportInfoList = data.split("#");
	//������ǳ�ʼ���ˣ��ӷ�����յ���¼�û�����Ϣ
	reportInfo.reportId = reportInfoList[0];  //ID����Ҫ��ʾ
	reportInfo.reportUserName = reportInfoList[1];
	reportInfo.reportTerm = reportInfoList[2];
	reportInfo.reportWeek = reportInfoList[3];
	reportInfo.reportMain = reportInfoList[4];
	reportInfo.reportTime = reportInfoList[5];
	reportInfo.reportName = reportInfoList[6];
	reportInfo.reportQuestionForSolve = reportInfoList[7];
	reportInfo.reportQuestionHasSolved = reportInfoList[8];
	reportInfo.reportQuestionFromLastWeek = reportInfoList[9];
	reportInfo.reportYourGet = reportInfoList[10];
	reportInfo.reportUserId = reportInfoList[11].toInt();  //���Ҳ����Ҫ��ʾ


	//���ý�����⣬XX,XXѧ�ڵڼ��ܵ��ܱ�
	ui->reporterDetail->setText(QString::fromLocal8Bit("%1,%2ѧ�ڵ�%3���ܱ�").arg(reportInfo.reportName)
		.arg(reportInfo.reportTerm).arg(reportInfo.reportWeek));
	ui->reportTime->setText(reportInfo.reportTime);

	//�������ģ�ҪŪ�ɲ����޸ĵ�����

	ui->reportMainPlainTextEdit->setPlainText(reportInfo.reportMain);
	ui->reportQuestionForSolve->setPlainText(reportInfo.reportQuestionForSolve);
	ui->reportQuestionFromLastWeek->setPlainText(reportInfo.reportQuestionFromLastWeek);
	ui->reportQuestionHasSolved->setPlainText(reportInfo.reportQuestionHasSolved);
	ui->reportYourGet->setPlainText(reportInfo.reportYourGet);
	
	//�ú�Ͽ�
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showReport()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
}


void ReportDetail::receiveDataFromClient(QString data)
{

}
//���ɾ����ť����ʱ��Ҫ
void ReportDetail::ClickDeleteButton()
{

}
//�޸İ�ť��Ҳ��ʱ��Ҫ
void ReportDetail::ClickUpdateButton()
{

}