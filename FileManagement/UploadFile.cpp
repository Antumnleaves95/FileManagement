#include "UploadFile.h"  
#include <QByteArray>
#include <QDataStream>
#include <QFileDialog>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <Qtime>
#include "MyMessageBox.h"

int sendtimes = 0;
UploadFile::UploadFile(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UploadFile)
{
	ui->setupUi(this);
	//ui->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
	ui->progressLabel->hide();
	ui->uploadSpeedLabel->hide();
	//connect(tcp, SIGNAL(connected()), this, SLOT(send()));  //�����ӳɹ�ʱ���Ϳ�ʼ�����ļ� 
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
	qDebug() << globalUserName << " will send a file to server";
	//����򿪰�ť��ʱ���ʼ������
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

void UploadFile::send()  //�����ļ�ͷ��Ϣ  
{
	//��һ�η��͵�ʱ��������
	connect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
	byteToWrite = localFile->size();  //ʣ�����ݵĴ�С  
	qDebug() << "the file bytetowrite: " << byteToWrite;
	totalSize = localFile->size();

	loadSize = 4 * 1024;  //ÿ�η������ݵĴ�С  

	QDataStream out(&outBlock, QIODevice::WriteOnly);

	//��ȡ�ļ�����
	QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
	//ռλ��,�������Ҫ������ռλ��Ȼ�������������峤�Ⱥ��ڲ���
	//ǰ���������ļ���С�ͷ����ļ�ͷ�Ĵ�С��Ϊʲô��qint64�أ������������ļ������û���
	out << qint64(0) << qint64(0) << currentFileName <<globalUserName;

	totalSize += outBlock.size();  //�ܴ�СΪ�ļ���С�����ļ�������Ϣ��С  
	byteToWrite += outBlock.size();
	qDebug() << "the total bytetowrite: " << byteToWrite;
	out.device()->seek(0);  //�ص��ֽ��������д��ǰ������qint64���ֱ�Ϊ�ܴ�С���ļ�������Ϣ��С  
	out << totalSize << qint64(outBlock.size());   //������Ƕ�Ӧǰ���qint64��
	qDebug() << "the file head:" << outBlock;
	tcp->tcpSocket->write(outBlock);  //���������ļ���Ϣ���͵��׽���  

	sendTime.start();  //����ʱ�俪ʼ��ʱ
	//UI��Ϣ
	ui->progressLabel->show();
	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);
}


//��������
void UploadFile::goOnSend(qint64 numBytes)  
{
	sendtimes++;  
	qDebug() << sendtimes<<" get in goOnSend";
		qDebug() << "the numBytes: " << numBytes <<"the loadSize:" <<loadSize
			<< "the " << sendtimes << "  left byteTowrite: " << byteToWrite;

	byteToWrite -= numBytes;  //ʣ�����ݴ�С  
	outBlock = localFile->read(qMin(byteToWrite, loadSize));   //���ʣ�����ݱ�ÿ�η��͵�С����ʣ���

	tcp->tcpSocket->write(outBlock);    //�������Ϣд��socket
	
	//qDebug() << "information:" <<outBlock;

	//�������ٶȴ�����ʾ�ٶȺ�������
	float useTime = sendTime.elapsed();
	double speed = (totalSize - byteToWrite) / useTime;
	ui->uploadSpeedLabel->show();
	ui->uploadSpeedLabel->setText(QString::fromLocal8Bit("�ѷ��� %1MB (%2MB/s) ��%3MB ����ʱ:%4��\n����ʣ��ʱ�䣺%5��")
		.arg((totalSize - byteToWrite) / (1024 * 1024))//�ѽ���
		.arg(speed * 1000 / (1024 * 1024), 0, 'f', 2)//�ٶ�
		.arg(totalSize / (1024 * 1024))//�ܴ�С
		.arg(useTime / 1000, 0, 'f', 0)//��ʱ
		.arg(totalSize / speed / 1000 - useTime / 1000, 0, 'f', 0));//ʣ��ʱ��


	//����UI�Ľ�������������Կ���Ӱ������
	ui->sendProgressBar->setMaximum(totalSize);
	ui->sendProgressBar->setValue(totalSize - byteToWrite);

	if (byteToWrite == 0)  //�������  
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�ϴ����!"), MESSAGE_INFORMATION, BUTTON_OK);
		ui->sendStatusLabel->setText(QString::fromLocal8Bit("�ļ��������!"));
		//������ϾͶϿ����д�ֽڵĲۺ�����
		disconnect(tcp->tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
		sendtimes = 0;
		qDebug() << "the file had send";
	}
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