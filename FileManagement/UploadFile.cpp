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
	qDebug() << "the bytetowrite: " << byteToWrite;
	totalSize = localFile->size();

	loadSize = 4 * 1024;  //ÿ�η������ݵĴ�С  

	QDataStream out(&outBlock, QIODevice::WriteOnly);
	QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);

	out << qint64(0) << qint64(0) << currentFileName;

	totalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
	byteToWrite += outBlock.size();
	qDebug() << "the bytetowrite: " << byteToWrite;
	out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
	out << totalSize << qint64(outBlock.size());
	qDebug() << "the file head:" << outBlock;
	tcp->tcpSocket->write(outBlock);  //���������ļ����͵��׽���  

	ui->progressLabel->show();
	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);
}

void UploadFile::goOnSend(qint64 numBytes)  //��ʼ�����ļ�����  
{
	qDebug() << "get in goOnSend";
	byteToWrite -= numBytes;  //ʣ�����ݴ�С  
	qDebug() << "the left byteTowrite: " << byteToWrite;
	outBlock = localFile->read(qMin(byteToWrite, loadSize));

	tcp->tcpSocket->write(outBlock);
	qDebug() << "information:" <<outBlock;
	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);

	if (byteToWrite == 0)  //�������  
	{
		ui->sendStatusLabel->setText(QString::fromLocal8Bit("�ļ��������!"));
		qDebug() << "the file had send";
	}
}

UploadFile::~UploadFile()
{
	delete ui;
}

void UploadFile::ClickOpenButton()  //���ļ�����ȡ�ļ���������·����  
{
	ui->sendStatusLabel->setText(QString::fromLocal8Bit("���ڴ��ļ�..."));
	ui->sendProgressBar->setValue(0);  //�ǵ�һ�η���  
	//tcp->tcpSocket->write("uploadFile");
	qDebug() << "send uploadFile";
	loadSize = 0;
	byteToWrite = 0;
	totalSize = 0;
	outBlock.clear();

	fileName = QFileDialog::getOpenFileName(this);
	localFile = new QFile(fileName);
	localFile->open(QFile::ReadOnly);

	ui->sendStatusLabel->setText(QString::fromLocal8Bit("�Ѵ��ļ� %1").arg(fileName));
}

void UploadFile::ClickSendButton()
{

	send();  //��һ�η��͵�ʱ������connectToHost����connect�źŲ��ܵ���send���ڶ���֮�����Ҫ����send��  
	connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
	ui->sendStatusLabel->setText(QString::fromLocal8Bit("���ڷ����ļ� %1").arg(fileName));
}

void UploadFile::receiveMainwindow()
{
	this->show();
}