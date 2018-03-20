#include "UploadWindow.h"  
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <Qtime>
#include <QtWin>
#include <QFileIconProvider>
#include "MyMessageBox.h"

//����1�� �û���ҷ���ļ�����û�е�����͵Ļ����������Ѿ������ˡ������ļ������źš����ʱ��
//�ͻ�����⣬�����������������ճ�������
int sendtimes = 0;
UploadWindow::UploadWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UploadWindow)
{
	ui->setupUi(this);
	
	initWindow();
	init();  //��ʼ��
}

UploadWindow::~UploadWindow()
{
	delete ui;
}

void UploadWindow::initWindow()
{
	setAcceptDrops(true);
	QPixmap DragDrop(":/Resource/FunIcon/Drag-Drop.png");  //�������ļ���ͼ��
														   //Drag = fil.scaled(QSize(50, 60), Qt::KeepAspectRatio);
	ui->Drag->setPixmap(DragDrop);
	//ui->Drag->setAlignment(Qt::AlignCenter);
	loadStyleSheet("UploadFile");  //
}
//��Ҫ�ǳ�ʼ������
void UploadWindow::init()
{
	index = 0;
	fileProgressBarMap = new QMap<int, QProgressBar*>;   //��Ҫ��ʼ��
	uploadFileMap = new QMap<int, UploadFile*>;
	uploadQThreadMap = new QMap<int, UploadThread*>;
	fileSpeedMap = new QMap<int, QLabel*>;
	fileStatusLayoutMap = new QMap<int, QHBoxLayout*>;
	//fileSizeMap = new QMap<int, QLabel*>;
	fileNameMap.clear();  //��ʼ�����������*����NEW��
	row = 0;
	column = 0;
	cnt = 0;
}



//��Ҫ�����������ļ����½��߳�
void UploadWindow::insertFile(QString m_fileName)
{
	//�ȶ����һ�㣬�ܿ��
	fileSumLayout = new QVBoxLayout(this);
	fileWindow = new QWidget(this);
	fileWindow->setObjectName("widgetMain");
	fileWindow->setStyleSheet("QWidget#widgetMain{border: 1px solid black; border-radius: 5px;};");
	
	fileWindow->setGeometry(0, 0, 250, 100);
	//�ڶ���
	//�ļ�������С��ͼ��
	fileInfoLayout = new QHBoxLayout();
	fileSumLayout->insertLayout(0, fileInfoLayout);
	{
		mFileIcon = new QLabel();
		//mFileIcon->setGeometry(0, 0, 30, 40);
		mFileIcon->setMaximumSize(30, 40);
		fileNameSizeLayout = new QVBoxLayout();
		
		fileInfoLayout->insertWidget(0, mFileIcon);
		fileInfoLayout->insertLayout(1, fileNameSizeLayout);

		mFileName = new QLabel();
		mFileSize = new QLabel();
		localFile = new QFile(m_fileName);
		localFile->open(QFile::ReadOnly);
		mFileSize->setText(countFileSize(QString::number(localFile->size(), 10)));

		localFile->close();
		 //fileSizeMap->insert(index, mFileSize);
		setFileIcon(m_fileName);	//���ø��ļ������ֺ�ͼ��
		 //fileInfoLayout->insertSpacerItem(2,Spacer);
		fileNameSizeLayout->addWidget(mFileName, Qt::AlignLeft);
		fileNameSizeLayout->addWidget(mFileSize, Qt::AlignLeft);
	}

	//������
	fileProgressBar = new QProgressBar(this);
	fileSumLayout->insertWidget(1, fileProgressBar);
	fileProgressBar->setMaximumHeight(10);
	fileProgressBar->setTextVisible(false);
	fileProgressBarMap->insert(index, fileProgressBar);  //���������������map
	
	//�ļ�״̬����
	fileStatusLayout = new QHBoxLayout();
	fileStatusLayoutMap->insert(index, fileStatusLayout);
	fileSumLayout->insertLayout(2, fileStatusLayout);
	{
		mFileSpeed = new QLabel();
		mButtonWait = new QPushButton();
		mButtonCancel = new QPushButton();

		fileSpeedMap->insert(index, mFileSpeed);
		mButtonWait->setText("wait");
		mButtonCancel->setText("cancel");

		fileStatusLayout->insertWidget(0, mFileSpeed);
		fileStatusLayout->insertWidget(1, mButtonWait);
		fileStatusLayout->insertWidget(2, mButtonCancel);

		/*mFileSpeed->setText("sudu");*/
	}

	fileWindow->setLayout(fileSumLayout);
	ui->UploadGridLayout->addWidget(fileWindow,row/2,column%2);
	//ui->FileListLayout->addWidget(fileWindow);

	//QProgressBar *fileProgressBar = new QProgressBar(this);
	//fileProgressBar->setTextVisible(false);   //����ʾ�ٷֱ�
	//fileProgressBarMap->insert(index, fileProgressBar);  //���������������map
	//stackLayout = new QStackedLayout();
	//fileWidget = new QWidget(this);
	// //fileWidget->setLayout(fileInfoLayout);  //����ˮƽlayout
	// //�����ļ�������
	// fileInfoLayout = new QHBoxLayout(fileWidget);
	// mFileIcon = new QLabel(fileWidget);
	// mFileName = new QLabel(fileWidget);
	// fileInfoLayout->addWidget(mFileIcon);
	// fileInfoLayout->addWidget(mFileName);
	//
	// setFileIcon(m_fileName);	//���ø��ļ������ֺ�ͼ��
	//
	// stackLayout->addWidget(fileWidget);
	// stackLayout->addWidget(fileProgressBar);
	// fileProgressBar->setTextVisible(false);
	// stackLayout->setStackingMode(QStackedLayout::StackAll);
	// ui->FileListLayout->addLayout(stackLayout);  //��ֱ�ķ��ö���ļ�

	 //���ʱ���Ѿ��������׽����ˣ��ǾͲ��У���Ϊ�������߳̽����ġ�����Ҫ���ƶ����̣߳�
	 //UploadFile *uploadFile = new UploadFile(m_fileName,index);
	 qDebug() << "the send id and name:" << index << " " << m_fileName;
	 fileThread = new UploadThread(m_fileName, index,this);
	 connect(fileThread, SIGNAL(finished()), fileThread, SLOT(deleteLater()));	
	 uploadQThreadMap->insert(index, fileThread);

	 qDebug() << " main Thread : " << QThread::currentThreadId();
	 connect(fileThread, SIGNAL(sendAvailable(int)), this, SLOT(beginToSend(int)));
	 fileThread->start();
	 
	 index++;
	 row++;
	 column++;
}


//����index��ֵ�����źŲ�
void UploadWindow::beginToSend(int num)
{
	//���ΰ��źŲ�
	QMap<int, UploadThread*>::iterator it = uploadQThreadMap->find(num);;
	//��Щ���Ǹ���UI���Լ��Ƿ������
	connect(it.value()->uploadFile, SIGNAL(sendOver(int)), this, SLOT(checkSendOver(int)), Qt::BlockingQueuedConnection);
	connect(it.value()->uploadFile, SIGNAL(updateProgress(int, qint64, qint64)),
		this, SLOT(updataProgressBar(int, qint64, qint64)),Qt::BlockingQueuedConnection);
	connect(it.value()->uploadFile, SIGNAL(updateSpeedLabel(int, double)), 
		this, SLOT(updateSpeedLabel(int, double)), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(sendFileSignal()), it.value()->uploadFile, SLOT(receiveSendSignal()));
	
}
void UploadWindow::loadStyleSheet(const QString &sheetName)
{
	QFile file("Resource/qss/" + sheetName + ".qss");
	qDebug() << sheetName + ".qss";
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		qDebug() << "is  css";
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	else
		qDebug() << "is not css";
}
void UploadWindow::dragEnterEvent(QDragEnterEvent *event)
{
	//���Ϊ�ļ�����֧���Ϸ�
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void UploadWindow::dropEvent(QDropEvent *event)
{
	//ע�⣺��������ж��ļ����ڣ���˼���û�һ�����϶��˶���ļ����������϶�һ��Ŀ¼
	//������ȡ����Ŀ¼�����ڲ�ͬ�Ĳ���ƽ̨�£��Լ���д����ʵ�ֶ�ȡ����Ŀ¼�ļ���
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;
	//���ı�����׷���ļ���
	foreach(QUrl url, urls) {
		fileName = url.toLocalFile();
		qDebug() << fileName;
		//ui->sendStatusLabel->setText(fileName);
		fileNameList.append(fileName);
		insertFile(fileName);
	}
}

void UploadWindow::initFile()
{

}

//�򿪰�ť
void UploadWindow::ClickOpenButton()  //���ļ�����ȡ�ļ���������·����  
{
	//ֱ�Ӽ���
	QStringList List = QFileDialog::getOpenFileNames();
	
	//fileName = QFileDialog::getOpenFileName(this);
	if (!List.isEmpty())
	{
		for (int i = 0; i < List.size(); i++)
		{
			fileName = List.at(i);
			qDebug() << "the file list:" << List.at(i);
			insertFile(List.at(i));
		}
		fileNameList.append(List);
	}
}



void UploadWindow::ClickSendButton()
{
	emit sendFileSignal();
	//QTcpSocket *tcp = new QTcpSocket();
	//tcp->connectToHost(ip, port);  //�����socket���ӵ�������
	ui->sendBtn->setEnabled(false); //��ťʧЧ
}
void UploadWindow::checkSendOver(int num)
{

	QMap<int, UploadThread*>::iterator it = uploadQThreadMap->find(num);
	//����һ��ͼƬ
	//qDebug() << num << "is send over";
	if (it.value()->uploadFile->isOver == true)
	{
		qDebug() << num << "is send over";
		QMap<int, QHBoxLayout*>::iterator it1 = fileStatusLayoutMap->find(num);
		QLabel *successLogo = new QLabel();
		successLogo->setPixmap(QPixmap(":/Resource/icon/correct.png"));
		QLabel *successMessage = new QLabel(QString::fromLocal8Bit("�ϴ��ɹ�"));

		it1.value()->insertWidget(0,successLogo);
		it1.value()->insertWidget(1,successMessage);
		QMap<int, QLabel*>::iterator it2 = fileSpeedMap->find(num);
		it2.value()->hide();
		//it.value()->~UploadThread();  //ֱ��������
		cnt++;
	}
	if (cnt == index)
	{
		ui->sendBtn->setEnabled(true);
	}
	//ֱ�ӳ�ʼ����Щmap
	//init();
	//ui->retranslateUi(this);
}

//���½�����
void UploadWindow::updataProgressBar(int num,qint64 byteToWrite,qint64 totalSize)
{
	QMap<int, QProgressBar*>::iterator itP = fileProgressBarMap->find(num);
	
	//mFileSize->setText(countFileSize(QString::number(totalSize, 10)));  //�����ļ���С����ʾ�ڽ����� 
	//qDebug() <<"index "<<num <<" value"<< byteToWrite << " " << totalSize;
	
	//Ϊʲô�����������
	itP.value()->setMaximum(totalSize);// ȡ����ֵ���ô�С
	itP.value()->setValue(totalSize - byteToWrite);  //���õ�ǰֵ

	
}
void UploadWindow::updateSpeedLabel(int num, double speed)
{
	QMap<int, QLabel*>::iterator it1 = fileSpeedMap->find(num);
	speed = (speed) / 1024 * 1024;
	//����ٶȴ���MB����ʾMB������ʾKB
	if (speed > 1024)
	{
		speed /= 1024;
		it1.value()->setText(QString::fromLocal8Bit("%1M/S").arg(speed, 0, 'f', 2));
	}
	else
	{
		it1.value()->setText(QString::fromLocal8Bit("%1KB/S").arg(speed, 0, 'f', 2));
	}
}
QString UploadWindow::countFileSize(QString fileSize)
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

void UploadWindow::setFileIcon(QString fileName)
{

	QString suffix = (fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)).left(3); //��ȡ�ļ���׺ǰ�����ַ�
	QString allName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1); //��ȡ�ļ�����
	qDebug() << "the suffix :" << suffix << " and the allName:" << allName;
	// ��ȡͼ�ꡢ����
	QIcon icon = fileIcon(suffix);
	QString strType = fileType(suffix);
	qDebug() << strType;
	QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(30, 40)));
	//QPixmap fileIcon(icon.);  //�������ļ���ͼ��
	//fileIcon = fileIcon.scaled(QSize(30, 40), Qt::KeepAspectRatio);
	mFileIcon->setPixmap(pixmap);
	mFileIcon->setAlignment(Qt::AlignCenter);
	mFileName->setText(allName);

	mFileIcon->show();
	mFileName->show();
}

void UploadWindow::receiveMainwindow()
{
	this->show();
}

QIcon UploadWindow::fileIcon(const QString &extension) const
{
	QFileIconProvider provider;
	QIcon icon;
	QString strTemplateName = QDir::tempPath() + QDir::separator() +
		QCoreApplication::applicationName() + "_XXXXXX." + extension;
	QTemporaryFile tmpFile(strTemplateName);
	tmpFile.setAutoRemove(false);

	if (tmpFile.open())
	{
		
		icon = provider.icon(QFileInfo(strTemplateName));
		tmpFile.close();
		// tmpFile.remove();
	}
	else
	{
		qDebug() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
	}

	return icon;
}

QString UploadWindow::fileType(const QString &extension) const
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

