#include "ReportEdit.h"
#include "MyMessageBox.h"

ReportEdit::ReportEdit(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ReportEdit)
{
	ui->setupUi(this);
	connect(tcp, SIGNAL(sendDataToReportEdit(QString)), this, SLOT(receiveDataFromClient(QString)));

	//setFixedSize(500, 500);
	init();
}

ReportEdit::~ReportEdit()
{
	delete ui;
}

void ReportEdit::init()
{
	//��ʼ�������ڻ�û��÷ŵ�ʲô���ͷŵ����ݰɡ�

}

void ReportEdit::sendReportEdit()
{
	//ÿ�δ򿪸�ҳ���򷢲�ѯ��Ϣ����ȥ
	QString data = "QueryReporter#" + globalUserName;   //��ѯ���е��ļ�
	QByteArray datasend = data.toUtf8();
	qDebug() << datasend;
	tcp->tcpSocket->write(datasend);

	//��ʱȡ��TCPԭ������֤��Ϣ
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showReporter()));
}


//�ӷ��������ձ����˵���Ϣ�����ǵ�¼�ߵ���Ϣ
void ReportEdit::showReporter()
{
	QByteArray dataread = tcp->tcpSocket->readAll();
	QString data = QString::fromUtf8(dataread);
	qDebug() << "the data from client: " << dataread;

	//��Щ�����ǿյ�
	QStringList reportInfoList = data.split("#");
	//������ǳ�ʼ���ˣ��ӷ�����յ���¼�û�����Ϣ
	reportInfo.reportName = reportInfoList[0];
	reportInfo.reportNumber = reportInfoList[1];
	reportInfo.reportLevel = reportInfoList[2];
	reportInfo.reportTeacher = reportInfoList[3];
	reportInfo.reportUserId = reportInfoList[4];
	//reportInfo.reportTerm = reportInfoList[4];
	//reportInfo.reportWeek = reportInfoList[5];
	//reportInfo.reportMain = reportInfoList[6];
	//reportInfo.reportQuestionForSolve = reportInfoList[7];
	//reportInfo.reportQuestionFromLastWeek = reportInfoList[8];
	//reportInfo.reportQuestionHasSolved = reportInfoList[9];
	//reportInfo.reportYourGet = reportInfoList[10];
	//reportInfo.reportUserId = reportInfoList[11];

	ui->reportName->setText(reportInfo.reportName);
	ui->reportName->setEnabled(false);
	ui->reportName->setFocusPolicy(Qt::NoFocus);

	ui->reportNumber->setText(reportInfo.reportNumber);
	ui->reportNumber->setEnabled(false);
	ui->reportNumber->setFocusPolicy(Qt::NoFocus);

	ui->reportLevel->setText(reportInfo.reportLevel);
	ui->reportLevel->setEnabled(false);
	ui->reportLevel->setFocusPolicy(Qt::NoFocus);

	ui->reportTeacher->setText(reportInfo.reportTeacher);
	ui->reportTeacher->setEnabled(false);
	ui->reportTeacher->setFocusPolicy(Qt::NoFocus);

	//�����Լ����ð�
	//ui->reportTerm->setEditText(reportInfo.reportTerm);
	//ui->reportWeek->setText(reportInfo.reportWeek);


	//���һ���ǿգ���ע�͵���
	/*ui->reportMainPlainTextEdit->setPlainText(reportInfo.reportMain);
	ui->reportQuestionForSolve->setPlainText(reportInfo.reportQuestionForSolve);
	ui->reportQuestionFromLastWeek->setPlainText(reportInfo.reportQuestionForSolve);
	ui->reportQuestionHasSolved->setPlainText(reportInfo.reportQuestionHasSolved);
	ui->reportYourGet->setPlainText(reportInfo.reportYourGet);
*/

	//�ú�Ͽ�
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showReporter()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
}

//�����жϷ������������ݣ�����һ���ۺ�����
void ReportEdit::receiveDataFromClient(QString data)
{
	if (QString::compare(data, "submit_T") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�ύ�ܱ��ɹ���"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);

	}
	else if (QString::compare(data, "submit_F") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�ύ�ܱ�ʧ�ܣ�"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
	}
}

//����ύ��ť
void ReportEdit::ClickSubmit()
{
	//ui->reportMainPlainTextEdit->toPlainText()�������QString���͵�
	if (ui->reportMainPlainTextEdit->toPlainText().isEmpty()
		|| ui->reportQuestionForSolve->toPlainText().isEmpty()
		|| ui->reportQuestionFromLastWeek->toPlainText().isEmpty()
		|| ui->reportQuestionHasSolved->toPlainText().isEmpty()
		|| ui->reportYourGet->toPlainText().isEmpty())
	{
		qDebug() << "there are some null";
		//�����Ϊ��������
		QString messageReminder;
		if (ui->reportMainPlainTextEdit->toPlainText().isEmpty())
		{
			messageReminder = QString::fromLocal8Bit("ѧϰ����");
			ui->reportMainPlainTextEdit->setFocus();

		}
		if (ui->reportQuestionForSolve->toPlainText().isEmpty())
		{
			messageReminder = QString::fromLocal8Bit("����������");
			ui->reportQuestionForSolve->setFocus();

		}
		if (ui->reportQuestionFromLastWeek->toPlainText().isEmpty())
		{
			messageReminder = QString::fromLocal8Bit("���ܵ�����");
			ui->reportQuestionFromLastWeek->setFocus();

		}
		if (ui->reportQuestionHasSolved->toPlainText().isEmpty())
		{
			messageReminder = QString::fromLocal8Bit("���������");
			ui->reportQuestionHasSolved->setFocus();

		}
		if (ui->reportYourGet->toPlainText().isEmpty())
		{
			messageReminder = QString::fromLocal8Bit("ѧϰ�ĵ�");
			ui->reportYourGet->setFocus();

		}
		//��ʾȷ���Լ�����
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("%1����Ϊ��!").arg(messageReminder),
			MESSAGE_QUESTION, BUTTON_OK_AND_CANCEL, true);
	}
	else
	{
		qDebug() << "send report!";
		sendReport();
	}
}

void ReportEdit::sendReport()
{

	QString bs = "sendReport";

	//��ȡ��ǰ�������õ�ѧ�ں�����
	reportInfo.reportTerm = ui->reportTerm->currentText();
	reportInfo.reportWeek = ui->reportWeek->currentText();
	reportInfo.reportMain = ui->reportMainPlainTextEdit->toPlainText();
	reportInfo.reportQuestionForSolve = ui->reportQuestionForSolve->toPlainText();
	reportInfo.reportQuestionFromLastWeek = ui->reportQuestionFromLastWeek->toPlainText();
	reportInfo.reportQuestionHasSolved = ui->reportQuestionHasSolved->toPlainText();
	reportInfo.reportYourGet = ui->reportYourGet->toPlainText();

	QString data = bs + "#" + reportInfo.reportUserId + "#" + 
		reportInfo.reportTerm + "#" + reportInfo.reportWeek + "#" + reportInfo.reportMain
		+ "#"+ reportInfo.reportQuestionForSolve
		+ "#"+ reportInfo.reportQuestionFromLastWeek + "#" + reportInfo.reportQuestionHasSolved
		+ "#"+ reportInfo.reportYourGet;

	QByteArray datasend = data.toUtf8();
	if (tcp->tcpSocket->write(datasend))
	{
		qDebug() << "send data to server: " << data;
		/*MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�ύ�ɹ�!"),
			MESSAGE_QUESTION, BUTTON_OK_AND_CANCEL, true);*/
	}
	else
	{
		qDebug() << "send failed!";
		/*MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�ύʧ��!"),
			MESSAGE_QUESTION, BUTTON_OK_AND_CANCEL, true);*/
	}

}
