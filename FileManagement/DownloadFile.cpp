#include "DownloadFile.h"
DownloadFile::DownloadFile(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DownloadFile)
{
	ui->setupUi(this);
	//StotalSize = 0;
	byteReceived = 0;
	//ui->progressLabel->hide();
	//��ӱ�ͷ
	model = new QStandardItemModel();
	model->setColumnCount(4);
	model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("�ļ���"));
	model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("�ļ���С"));
	model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("�ļ�����"));
	model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("�ļ�����"));
	ui->downloadTable->setModel(model);
	ui->downloadTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	//�����п��ɱ� 
	ui->downloadTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->downloadTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	ui->downloadTable->setColumnWidth(0, 200);
	ui->downloadTable->setColumnWidth(1, 80);

	//connect(tcp, SIGNAL(connected()), this, SLOT(send()));  //�����ӳɹ�ʱ���Ϳ�ʼ�����ļ� 
}

DownloadFile::~DownloadFile()
{
	delete ui;
}

void DownloadFile::receiveFile()
{
	qDebug() << "get in receiveFile!!!";
	qDebug() << byteReceived;

	if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
	{
		qDebug() << "receive the file of head";
		//ui->receivedProgressBar->setValue(0);
		QDataStream in(tcp->tcpSocket);
		//QString globalUserName;
		in >> RtotalSize >> byteReceived >> fileName;
		//user.queryUserByName(globalUserName);
		//qDebug() << "the user :" << user.getUserName();

		fileName = "files/" + fileName;
		qDebug() << "the file of head: " << fileName;
		qDebug() << "totalSize: " << RtotalSize;
		qDebug() << " first byteReceived: " << byteReceived;
		newFile = new QFile(fileName);
		newFile->open(QFile::WriteOnly);

		//��ʼ��ʱ��
		downloadTime.start();

	}
	else  //��ʽ��ȡ�ļ�����  
	{
		receiveTime++;
		qDebug() << "the " << receiveTime << " receive the file ";
		inBlock = tcp->tcpSocket->readAll();
		qDebug() << "current receive the byte is :" << byteReceived;
		//qDebug() << "the block: " << inBlock;
		byteReceived += inBlock.size();

		float useTime = downloadTime.elapsed();
		double speed = (byteReceived) / useTime;
		ui->downloadSpeedLabel->show();
		ui->downloadSpeedLabel->setText(QString::fromLocal8Bit("������ %1MB (%2MB/s) ��%3MB ����ʱ:%4��\n����ʣ��ʱ�䣺%5��")
			.arg((byteReceived) / (1024 * 1024))//�ѽ���
			.arg(speed * 1000 / (1024 * 1024), 0, 'f', 2)//�ٶ�
			.arg(RtotalSize / (1024 * 1024))//�ܴ�С
			.arg(useTime / 1000, 0, 'f', 0)//��ʱ
			.arg(RtotalSize / speed / 1000 - useTime / 1000, 0, 'f', 0));//ʣ��ʱ��
		
		ui->progressBar->setMaximum(RtotalSize);
		ui->progressBar->setValue(byteReceived);
		qDebug() << "begin to write";
		newFile->write(inBlock);
		newFile->flush();
	}

	if (byteReceived == RtotalSize)
	{
		ui->downloadSpeedLabel->setText(QString::fromLocal8Bit("������ɣ�"));
		qDebug() << "receive is done";
		/*qDebug() << "the file name:" << file.getFileName();
		qDebug() << "the userid :" << user.getUserId();
		qDebug() << "the userid from file:" << file.getUserId();*/
		inBlock.clear();
		byteReceived = 0;
		RtotalSize = 0;
		receiveTime = 0;
		
		disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
		connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	}
}
void DownloadFile::ClickDownloadButton()
{
	receiveTime = 0;
	QString openFileName;
	//��ȡui�������Ϣ
	openFileName = ui->downloadFileName->text();

	QString bs = "downloadFile";
	QString data = bs + "#" + openFileName;

	//����������Ҫ
	QByteArray datasend = data.toLocal8Bit();
	if (tcp->tcpSocket->write(datasend))
	{
		
		qDebug() << data;
		connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
		disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	}
	else
	{
		qDebug() << "send filed :" << data;
	}
}

void DownloadFile::sendFileInfo()
{
	//ÿ�δ򿪸�ҳ���򷢲�ѯ��Ϣ����ȥ
	QString data = "findFileByName#" + globalUserName;
	QByteArray datasend = data.toLocal8Bit();
	qDebug() << datasend;
	tcp->tcpSocket->write(datasend);

	//��ʱȡ��TCPԭ������֤��Ϣ
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showFileInfo()));
}

void DownloadFile::showFileInfo()
{

	QByteArray dataread = tcp->tcpSocket->readAll();
	QString data = QString::fromLocal8Bit(dataread);
	qDebug() << "the data from client: " << dataread;


	QStringList listNumber = data.split("$");

	QList<FileInfo> fileInfo;
	//�������Ҫ���뵽����ȥ
	//�����ݶ��ŵ�QList��ȥ��
	//listNumber[0] ���ļ�����
	for (int i = 1;i <= listNumber[0].toInt();i++)
	{
		FileInfo littleFile;
		QStringList fileList = listNumber[i].split("#");
		littleFile.fileId = fileList[0].toInt();
		littleFile.fileName = fileList[1];
		littleFile.fileSize = fileList[2];
		littleFile.fileType = fileList[3];
		littleFile.userId = fileList[4].toInt();
		qDebug() << littleFile.fileId<<" " << littleFile.fileName << " " <<
			littleFile.fileSize << " " << littleFile.fileType << " " << littleFile.userId;
		fileInfo.append(littleFile);
	}

	ui->downloadTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	for (int i = 0; i < listNumber[0].toInt(); i++)
	{
		model->setItem(i, 0, new QStandardItem(fileInfo.at(i).fileName));
		model->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("%1MB").arg(
			QString::number(fileInfo.at(i).fileSize.toFloat() / (1024 * 1024), 'f', 2))));
		model->setItem(i, 2, new QStandardItem(fileInfo.at(i).fileType));

		//mDownloadButton = new QPushButton("����");
		ui->downloadTable->setIndexWidget(model->index(model->rowCount() - 1, 6), new QPushButton("Download"));
		//model->setItem(i, 3, mDownloadButton);
	}
	//�����Ͽ�
	connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showFileInfo()));
}