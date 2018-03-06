#include "DownloadFile.h"
#include "ButtonDelegate.h"
#include "TableModel.h"
#include <QFileDialog>
#include<QToolTip>
#include <QtWin>
#include <QToolButton>
#include <QFileIconProvider>
#include "MyMessageBox.h"
DownloadFile::DownloadFile(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DownloadFile)
{
	ui->setupUi(this);
	//StotalSize = 0;
	byteReceived = 0;

	initModel();
	init();

	ui->downloadTable->setMouseTracking(true);   //�������׷��

	connect(ui->downloadTable, SIGNAL(entered(QModelIndex)),
		this, SLOT(showToolTip(QModelIndex)));

	//�ӿͻ��˴���õ�������
	connect(tcp, SIGNAL(sendDataToDownload(QString)), this, SLOT(receiveDataFromClient(QString)));
}

DownloadFile::~DownloadFile()
{
	delete ui;
}
void  DownloadFile::initModel()
{
	model = new QStandardItemModel();
	model->setColumnCount(7);
	ui->downloadTable->setShowGrid(false);
	ui->downloadTable->verticalHeader()->setVisible(false);// ��ֱ���ɼ�
	//ui->downloadTable->horizontalHeader()->setVisible(false);// ˮƽ���ɼ�
	model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit(""));
	model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("�ļ���"));
	model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("�ļ���С"));
	model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("�ϴ�ʱ��"));
	model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("�ϴ���"));
	model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("����"));
	model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("ɾ��"));
	
	ui->downloadTable->setModel(model);
	ui->downloadTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui->downloadTable->setColumnWidth(0, 30);
	ui->downloadTable->setColumnWidth(1, 240);
	ui->downloadTable->setColumnWidth(2, 70);
	ui->downloadTable->setColumnWidth(3, 110);
	ui->downloadTable->setColumnWidth(4, 70);
	ui->downloadTable->setColumnWidth(5, 40);
	ui->downloadTable->setColumnWidth(6, 40);
	
	//�����п��ɱ� 
	ui->downloadTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->downloadTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	ui->downloadTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	ui->downloadTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
	ui->downloadTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
	ui->downloadTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
}

//��ʼ������
void DownloadFile::init()
{
	//�������Ӹ���ť
	ui->condition->setMinimumHeight(30);
	ui->condition->setTextMargins(0, 0, 30, 0);
	//ui->condition->setPlaceholderText(QString::fromLocal8Bit("�������û���"));

	QToolButton *find = new QToolButton();
	find->setMaximumSize(40, 40);
	find->setCursor(Qt::PointingHandCursor);
	find->setIcon(QIcon(":/Resource/icon/lookReport.png"));
	find->setIconSize(QSize(20, 20));//����ʵ�ʵ���ͼƬ��С
	find->setStyleSheet("border:none");

	QSpacerItem *spaceItem1 = new QSpacerItem(150, 40, QSizePolicy::Expanding);
	QHBoxLayout *editLayout1 = new QHBoxLayout();
	editLayout1->setContentsMargins(1, 0, 1, 0);
	editLayout1->addSpacerItem(spaceItem1);
	editLayout1->addWidget(find);
	ui->condition->setLayout(editLayout1);
	connect(find, SIGNAL(clicked()), this, SLOT(ClickFindButton()));
}
//��ʾ�ı�׷��
void DownloadFile::showToolTip(const QModelIndex &index) {
	if (!index.isValid()) {
		qDebug() << "Invalid index";
		return;
	}
	QToolTip::showText(QCursor::pos(), index.data().toString());
}


void DownloadFile::receiveFile()
{
	qDebug() << "get in receiveFile!!!";
	qDebug() << "the byte has received :" << byteReceived;

	if (byteReceived == 0)  //�Ÿտ�ʼ�������ݣ�������Ϊ�ļ���Ϣ  
	{
 
		qDebug() << "receive the file of head";
		//ui->receivedProgressBar->setValue(0);
		QDataStream in(tcp->tcpSocket);
		//QString globalUserName;
		in >> RtotalSize >> byteReceived >> fileName;
		//user.queryUserByName(globalUserName);
		//qDebug() << "the user :" << user.getUserName();
		
		qDebug() << "the file of head: " << fileName;
		qDebug() << "totalSize: " << RtotalSize;
		qDebug() << " first byteReceived: " << byteReceived;
		qDebug() << "start saveFileName:" << saveFileName;
		newFile = new QFile(saveFileName);
		newFile->open(QFile::WriteOnly);
		//��ʼ��ʱ��
		downloadTime.start();
	}
	else  //��ʽ��ȡ�ļ�����  
	{
		while (tcp->tcpSocket->bytesAvailable() > 0)
		{
			receiveTime++;
			qDebug() << "the " << receiveTime << " receive the file ";
			inBlock = tcp->tcpSocket->read(4 * 1024);
			qDebug() << "the inBlock size is " << inBlock.size();   //��tmճ�����ֵ�
			qDebug() << "current received the byte is :" << byteReceived;
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
			//qDebug() << "begin to write";

			newFile->write(inBlock);
			newFile->flush();
		}
	}

	if (byteReceived == RtotalSize)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�������!"), MESSAGE_INFORMATION, BUTTON_OK);
		ui->downloadSpeedLabel->setText(QString::fromLocal8Bit("������ɣ�"));
		qDebug() << "receive is done, and the byteReceived is :" << byteReceived <<" the RtotalSize is :"<< RtotalSize;
		/*qDebug() << "the file name:" << file.getFileName();
		qDebug() << "the userid :" << user.getUserId();
		qDebug() << "the userid from file:" << file.getUserId();*/
		inBlock.clear();
		byteReceived = 0;
		RtotalSize = 0;
		receiveTime = 0;
		newFile->close();
		disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
		connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	}
}
void DownloadFile::ClickDownloadButton()
{
	QPushButton *btn = (QPushButton *)sender();   //�����¼��Ķ��󣬰���˭��ȡ˭
	QString row = btn->property("row").toString();  //ȡ�ð�ť��row����
	qDebug() << "the row is: "<<row;

	receiveTime = 0;
	//��ȡui�������Ϣ
	//openFileName = ui->downloadFileName->text();   
	openFileName = btn->property("fileName").toString();
	QString bs = "downloadFile";
	QString data = bs + "#" + openFileName;
	if (saveFilePath(openFileName))  //����һ�»�ȡ·����������ʵ����ֱ�ӻ�ȡ�ļ���,��������������һ��
	//����������Ҫ
	{
		QByteArray datasend = data.toUtf8();  //����UTF8��ȥ
		if (tcp->tcpSocket->write(datasend))
		{
			qDebug() << "send data to server: " << data;
			disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));  //ȡ��TCP����������Ϣ
			connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
		}
		else
		{
			qDebug() << "send filed :" << data;
		}
	}
}
void DownloadFile::ClickDeleteButton()
{
	QPushButton *btn = (QPushButton *)sender();   //�����¼��Ķ��󣬰���˭��ȡ˭
	QString row = btn->property("row").toString();  //ȡ�ð�ť��row����

	
	QString deleteFileName = btn->property("deleteFileName").toString();
	QString deleteFileId = btn->property("deleteFileId").toString();
	if (!MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ȷ��Ҫɾ��%1��").arg(deleteFileName),
		MESSAGE_QUESTION, BUTTON_OK_AND_CANCEL, true))
	{
		QString bs = "deleteFile";
		QString data = bs + "#" + deleteFileName + "#" + deleteFileId;

		QByteArray datasend = data.toUtf8();
		if (tcp->tcpSocket->write(datasend))
		{
			qDebug() << "send data to server: " << data;

		}
		else
			qDebug() << "send failed!";
	}

}

void DownloadFile::sendFileInfo(QString condition)
{
	//ÿ�δ򿪸�ҳ���򷢲�ѯ��Ϣ����ȥ
	//QString data = "findFileByName#" + globalUserName;   //��ѯ���û����ļ���
	QString data = "findFile#";   //��ѯ�ļ���Ĭ��Ϊall
	data += condition;

	QByteArray datasend = data.toUtf8();
	qDebug() << datasend;
	tcp->tcpSocket->write(datasend);

	//��ʱȡ��TCPԭ������֤��Ϣ
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showFileInfo()));
}

void DownloadFile::receiveDataFromClient(QString data)
{
	if (QString::compare(data, "delete_T") == 0)
	{
		//ui->downloadTable->clearn

		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ɾ���ļ��ɹ���"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
		//ʵ��ʵʱ����
		initModel();  //ɾ�����������ʼ��
		sendFileInfo();  //Ȼ����Ϣ����������ѯ��ǰ�ļ����
		//model.
	}
	else if (QString::compare(data, "delete_F") == 0)
	{
		MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ɾ���ļ�ʧ�ܣ�"), MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
	}
}

void DownloadFile::showFileInfo()
{
	initModel();
	QByteArray dataread = tcp->tcpSocket->readAll();
	//QDataStream in(tcp->tcpSocket);

	qDebug() << dataread;

	QBuffer buf(&dataread);
	buf.open(QIODevice::ReadOnly);
	QDataStream in(&buf);
	int FileListSize;
	in >> FileListSize;  //�����ļ�����

	//����һ��fileInfo��Qlist
	QList<FileInfo> fileInfo;
	//��������������д��
	for (int i = 0;i < FileListSize;i++)
	{
		FileInfo f1;
		in >> f1;
		fileInfo.append(f1);
		qDebug() << f1.fileId << " " << f1.fileName << " " <<
			f1.fileSize << " " << f1.fileType << " " 
			<< f1.fileTime << " " << f1.fileUser << " " << f1.userId;
	}
	buf.close();
	//FileInfo f2;


	////QString data = QString::fromUtf8(dataread);
	////qDebug() << "the data from client: " << dataread;
	////QStringList listNumber = data.split("$");

	//QList<FileInfo> fileInfo;
	////�������Ҫ���뵽����ȥ
	////�����ݶ��ŵ�QList��ȥ��
	////listNumber[0] ���ļ�����
	//for (int i = 1;i <= listNumber[0].toInt();i++)
	//{
	//	FileInfo littleFile;
	//	QStringList fileList = listNumber[i].split("^");
	//	littleFile.fileId = fileList[0].toInt();
	//	littleFile.fileName = fileList[1];
	//	littleFile.fileSize = fileList[2];
	//	littleFile.fileType = fileList[3];
	//	littleFile.fileTime = fileList[4];
	//	littleFile.fileUser = fileList[5];
	//	littleFile.userId = fileList[6].toInt();
	//	qDebug() << littleFile.fileId<<" " << littleFile.fileName << " " <<
	//		littleFile.fileSize << " " << littleFile.fileType << " " 
	//		<< littleFile.fileTime << " " << littleFile.fileUser << " " << littleFile.userId;
	//	fileInfo.append(littleFile);
	//}
	ui->downloadTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	for (int i = 0; i < fileInfo.size(); i++)
	{
		//����ǰ���е�����
		model->setItem(i, 1, new QStandardItem(fileInfo.at(i).fileName));
		model->setItem(i, 2, new QStandardItem(countFileSize(fileInfo.at(i).fileSize)));  //countFileSize���ݴ�С��ʾKB����MB
		model->setItem(i, 3, new QStandardItem(fileInfo.at(i).fileTime));
		model->setItem(i, 4, new QStandardItem(fileInfo.at(i).fileUser));

		//Ϊ�����������Ӱ�ť
		m_download = new QToolButton();
		QIcon downloadFile(":/Resource/icon/downloadFile.png"); //����QIcon����
		m_download->setIcon(downloadFile); //��ͼƬ���õ���ť��
		m_download->setIconSize(QSize(20, 20));//����ʵ�ʵ���ͼƬ��С
		m_download->setStyleSheet("border:none");
		m_download->setCursor(Qt::PointingHandCursor);
		m_delete = new QToolButton(); 
		QIcon deleteFile(":/Resource/icon/deleteFile.png"); //����QIcon����
		m_delete->setIcon(deleteFile); //��ͼƬ���õ���ť��
		m_delete->setIconSize(QSize(20, 20));//����ʵ�ʵ���ͼƬ��С
		m_delete->setStyleSheet("border:none");
		m_delete->setCursor(Qt::PointingHandCursor);
		//�������ذ�ť�Ĳۺ���
		connect(m_download, SIGNAL(clicked(bool)), this, SLOT(ClickDownloadButton()));
		connect(m_delete, SIGNAL(clicked(bool)), this, SLOT(ClickDeleteButton()));
		//ֱ�Ӱ��ļ�������row���ˣ�

		m_download->setProperty("row", i);  //Ϊ��ť����row����
		m_download->setProperty("fileName", fileInfo.at(i).fileName);  //Ϊ��ť����filename����

		m_delete->setProperty("row", i);
		m_delete->setProperty("deleteFileId", fileInfo.at(i).fileId);
		m_delete->setProperty("deleteFileName", fileInfo.at(i).fileName);

		
		ui->downloadTable->setIndexWidget(model->index(model->rowCount() - 1, 5), m_download);
		ui->downloadTable->setIndexWidget(model->index(model->rowCount() - 1, 6), m_delete);

		//��������һ��ͼ�ꣿ
		QIcon icon = fileIcon(fileInfo.at(i).fileType);
		QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(20, 30)));
		QLabel *fIcon = new QLabel();
		fIcon->setPixmap(pixmap);
		ui->downloadTable->setIndexWidget(model->index(model->rowCount() - 1, 0), fIcon);
		
		//�������ݾ���
		for(int j = 2;j < 5;j++)
		model->item(i, j)->setTextAlignment(Qt::AlignCenter);
	}
	//m_model->setData(data);
	//�����Ͽ�
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showFileInfo()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	
}

QString DownloadFile::countFileSize(QString fileSize)
{
	float floatSize;
	floatSize = fileSize.toFloat() / 1024;   //���Ǽ���KB��
	//���floatSize����1024KB�����MB,�ұ���2λС��
	if (floatSize > 1024)
	{
		return QString::fromLocal8Bit("%1MB").arg(QString::number(fileSize.toFloat() / (1024 * 1024), 'f', 2)); //����һ��QString
	}
	else
	{
		return QString::fromLocal8Bit("%1KB").arg(QString::number(fileSize.toFloat() / (1024), 'f', 2)); //����һ��QString
	}

	//QString::number(fileSize.toFloat() / (1024 * 1024), 'f', 2);  //��ת��Ϊ��������
}
bool DownloadFile::saveFilePath(QString openFileName)
{
	QFileDialog m_QFileDialog;   //�½�һ���Ի�

	//!-- �õ�ԭ�ļ��ĺ�׺��  
	QString suffix_save = QFileInfo(openFileName).suffix();
	//!--- ���ù���ƥ���ļ���ʽ  
	QString setFilter = "office(*.doc *.docx *.xls *.xlsx *.ppt *.pptx);;image(*.jpg *.bmp);;txt(*.xml *.txt *.pdf);;(*.*);;";
	//!--- selectFilterΪ�����Ի����е��ļ���ʽ��dirStringΪ�ļ�·����������ʾ�ļ���  
	QString selectFilter, dirString;

	if (openFileName.isEmpty()) return false;
	else dirString = QFileInfo(openFileName).fileName();

	//!--- �ж��ļ���׺���Լ��ļ���ʽ�����Ƿ�ƥ�䣬û��������Ӧ����  
	if (!suffix_save.isEmpty() && setFilter.contains(selectFilter)) selectFilter = suffix_save.insert(0, "*.");
	if (!suffix_save.isEmpty() && !setFilter.contains(selectFilter)) selectFilter = "*.*";

	 saveFileName = m_QFileDialog.getSaveFileName(this, "�����ļ�", dirString, setFilter, &selectFilter,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	
	 //qDebug() << " saveFileName :"<< saveFileName;  //û���Ⱑ����
	if (saveFileName.isEmpty()) return false;

	return true;
	//QFile file(saveFileName);

	//if (selectFilter.compare("*.*")) saveFileName = saveFileName + selectFilter.remove(0, 1);
	//qDebug() << " saveFileName1 :" << saveFileName;  //��������
}

void DownloadFile::loadStyleSheet(const QString &sheetName)
{
	QFile file(":/Resource/qss/" + sheetName + ".qss");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		//qDebug() << "is qss";
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
}

QIcon DownloadFile::fileIcon(const QString &extension) const
{
	QFileIconProvider provider;
	QIcon icon;
	QString strTemplateName = QDir::tempPath() + QDir::separator() +
		QCoreApplication::applicationName() + "_XXXXXX." + extension;
	QTemporaryFile tmpFile(strTemplateName);
	tmpFile.setAutoRemove(false);

	if (tmpFile.open())
	{
		QString file_name = tmpFile.fileName();
		tmpFile.close();
		icon = provider.icon(QFileInfo(file_name));
		tmpFile.remove();
	}
	else
	{
		qDebug() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
	}

	return icon;
}

QString DownloadFile::fileType(const QString &extension) const
{
	QFileIconProvider provider;
	QString strType;
	QString strFileName = QDir::tempPath() + QDir::separator() +
		QCoreApplication::applicationName() + "_XXXXXX." + extension;
	QTemporaryFile tmpFile(strFileName);
	tmpFile.setAutoRemove(false);

	if (tmpFile.open())
	{
		tmpFile.close();
		strType = provider.type(QFileInfo(tmpFile.fileName()));
		// tmpFile.remove();
	}
	else
	{
		qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
	}

	return strType;
}

//������ѯ��������Ϊ����Ĭ��Ϊall����Ҫ�ǲ�ѯ�ļ�����
void DownloadFile::ClickFindButton()
{
	//�����������ֵ
	QString condition = ui->condition->text();
	if (condition.isEmpty())
	{
		sendFileInfo();
	}
	else
	{
		sendFileInfo(condition);
	}
}

