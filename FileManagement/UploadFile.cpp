#include "UploadFile.h"  
#include <QByteArray>
#include <QDataStream>
#include <QFileDialog>
#include <QHostAddress>
#include <QIODevice>
#include <QString>


int sendtimes = 0;
UploadFile::UploadFile(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UploadFile)
{
	ui->setupUi(this);

	ui->progressLabel->hide();

	//connect(tcp, SIGNAL(connected()), this, SLOT(send()));  //�����ӳɹ�ʱ���Ϳ�ʼ�����ļ�  
	


}

void UploadFile::send()  //�����ļ�ͷ��Ϣ  
{
	connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
	byteToWrite = localFile->size();  //ʣ�����ݵĴ�С  
	qDebug() << "the file bytetowrite: " << byteToWrite;
	totalSize = localFile->size();

	loadSize = 2 * 1024;  //ÿ�η������ݵĴ�С  

	QDataStream out(&outBlock, QIODevice::WriteOnly);

	//��ȡ�ļ�����
	QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
	
	//ǰ���������ļ���С�ͷ����ļ�ͷ�Ĵ�С���������ļ������û���
	out << qint64(0) << qint64(0) << currentFileName <<globalUserName;

	totalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
	byteToWrite += outBlock.size();
	qDebug() << "the total bytetowrite: " << byteToWrite;
	out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
	out << totalSize << qint64(outBlock.size());
	qDebug() << "the file head:" << outBlock;
	tcp->tcpSocket->write(outBlock);  //���������ļ���Ϣ���͵��׽���  


	//UI��Ϣ
	ui->progressLabel->show();
	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);
}

void UploadFile::goOnSend(qint64 numBytes)  //��ʼ�����ļ�����  
{
	sendtimes++;  
	qDebug() << sendtimes<<" get in goOnSend";
		qDebug() << "the numBytes: " << numBytes <<"the loadSize:" <<loadSize
			<< "the " << sendtimes << "  left byteTowrite: " << byteToWrite;

	byteToWrite -= numBytes;  //ʣ�����ݴ�С  
	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //���ʣ�����ݱ�ÿ�η��͵�С����ʣ���

	tcp->tcpSocket->write(outBlock);    //�������Ϣд��socket

	//qDebug() << "information:" <<outBlock;
	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);

	if (byteToWrite == 0)  //�������  
	{
		ui->sendStatusLabel->setText(QString::fromLocal8Bit("�ļ��������!"));
		disconnect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		sendtimes = 0;
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

	//���߷���������Ҫ�����ļ���
	tcp->tcpSocket->write("uploadFile");
	qDebug() << globalUserName<<" will send a file to server";
	loadSize = 0;
	byteToWrite = 0;
	totalSize = 0;
	sendtimes = 0;
	outBlock.clear();

	fileName = QFileDialog::getOpenFileName(this);
	localFile = new QFile(fileName);
	localFile->open(QFile::ReadOnly);

	ui->sendStatusLabel->setText(QString::fromLocal8Bit("�Ѵ��ļ� %1").arg(fileName));
}

void UploadFile::ClickSendButton()
{
	
	send();  //��һ�η��͵�ʱ������connectToHost����connect�źŲ��ܵ���send���ڶ���֮�����Ҫ����send��  
	
	//connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
	ui->sendStatusLabel->setText(QString::fromLocal8Bit("���ڷ����ļ� %1").arg(fileName));
}

void UploadFile::receiveMainwindow()
{
	this->show();
}