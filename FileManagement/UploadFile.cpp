#include "UploadFile.h"  
#include <QHostAddress>  
#include <QTextCodec>  
#include <QFileDialog>  

UploadFile::UploadFile(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UploadFile)
{
	ui->setupUi(this);

	ui->progressLabel->hide();

	//QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));

	//tcpClient = new QTcpSocket(this);
	sendTimes = 0;

	//connect(tcp, SIGNAL(connected()), this, SLOT(send()));  //�����ӳɹ�ʱ���Ϳ�ʼ�����ļ�  
	//connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));


}

void UploadFile::send()  //�����ļ�ͷ��Ϣ  
{
	byteToWrite = localFile->size();  //ʣ�����ݵĴ�С  
	totalSize = localFile->size();

	loadSize = 4 * 1024;  //ÿ�η������ݵĴ�С  

	QDataStream out(&outBlock, QIODevice::WriteOnly);
	QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);

	out << qint64(0) << qint64(0) << currentFileName;

	totalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
	byteToWrite += outBlock.size();

	out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
	out << totalSize << qint64(outBlock.size());

	tcp->tcpSocket->write(outBlock);  //���������ļ����͵��׽���  

	ui->progressLabel->show();
	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);
}

void UploadFile::goOnSend(qint64 numBytes)  //��ʼ�����ļ�����  
{
	byteToWrite -= numBytes;  //ʣ�����ݴ�С  
	outBlock = localFile->read(qMin(byteToWrite, loadSize));

	tcp->tcpSocket->write(outBlock);

	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);

	if (byteToWrite == 0)  //�������  
		ui->sendStatusLabel->setText(tr("�ļ��������!"));
}

UploadFile::~UploadFile()
{
	delete ui;
}

void UploadFile::ClickOpenButton()  //���ļ�����ȡ�ļ���������·����  
{
	ui->sendStatusLabel->setText(tr("���ڴ��ļ�..."));
	ui->sendProgressBar->setValue(0);  //�ǵ�һ�η���  

	loadSize = 0;
	byteToWrite = 0;
	totalSize = 0;
	outBlock.clear();

	fileName = QFileDialog::getOpenFileName(this);
	localFile = new QFile(fileName);
	localFile->open(QFile::ReadOnly);

	ui->sendStatusLabel->setText(tr("�Ѵ��ļ� %1").arg(fileName));
}

void UploadFile::ClickSendButton()
{

		send();  //��һ�η��͵�ʱ������connectToHost����connect�źŲ��ܵ���send���ڶ���֮�����Ҫ����send��  
		connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		ui->sendStatusLabel->setText(tr("���ڷ����ļ� %1").arg(fileName));
}

void UploadFile::receiveMainwindow()
{
	this->show();
}