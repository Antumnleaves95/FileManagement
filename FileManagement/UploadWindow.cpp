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
	fileNameMap.clear();  //��ʼ�����������*����NEW��
	row = 0;
	column = 0;
}



//��Ҫ�����������ļ����½��߳�
void UploadWindow::insertFile(QString m_fileName)
{
	//�ȶ����һ�㣬�ܿ��
	QVBoxLayout *fileSumLayout = new QVBoxLayout(this);
	//�ڶ���
	QHBoxLayout *fileInfoLayout = new QHBoxLayout();
	{
		mFileIcon = new QLabel();
		
		QVBoxLayout *fileNameSizeLayout = new QVBoxLayout();
		
		fileInfoLayout->insertWidget(0, mFileIcon);
		fileInfoLayout->insertLayout(1, fileNameSizeLayout);

		 mFileName = new QLabel();
		 mFileSize = new QLabel();
		 setFileIcon(m_fileName);	//���ø��ļ������ֺ�ͼ��
		fileNameSizeLayout->insertWidget(0, mFileName);
		fileNameSizeLayout->insertWidget(1, mFileSize);
	}
	QProgressBar *fileProgressBar = new QProgressBar(this);
	fileProgressBar->setTextVisible(false);
	fileProgressBarMap->insert(index, fileProgressBar);  //���������������map

	QHBoxLayout *fileStatusLayout = new QHBoxLayout();
	{
		mFileSpeed = new QLabel();
		mButtonWait = new QPushButton();
		mButtonCancel = new QPushButton();

		mButtonWait->setText("wait");
		mButtonCancel->setText("cancel");

		fileStatusLayout->insertWidget(0, mFileSpeed);
		fileStatusLayout->insertWidget(1, mButtonWait);
		fileStatusLayout->insertWidget(2, mButtonCancel);
	}
	
	fileSumLayout->insertLayout(0, fileInfoLayout);
	fileSumLayout->insertWidget(1, fileProgressBar);
	fileSumLayout->insertLayout(2, fileStatusLayout);
	QWidget *fileWindow = new QWidget(this);
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
	connect(it.value()->uploadFile, SIGNAL(sendOver()), this, SLOT(checkSendOver()));
	connect(it.value()->uploadFile, SIGNAL(updateProgress(int, qint64, qint64)),
		this, SLOT(updataProgressBar(int, qint64, qint64)),Qt::BlockingQueuedConnection);
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
			qDebug() << "the file list:" << List.at(i);
			insertFile(List.at(i));
		}
		fileNameList.append(List);
	}
}



void UploadWindow::ClickSendButton()
{
	emit sendFileSignal();
	ui->sendBtn->setEnabled(false); //��ťʧЧ
}
void UploadWindow::checkSendOver()
{
	QMap<int, UploadThread*>::iterator it;
	for (it = uploadQThreadMap->begin(); it != uploadQThreadMap->end(); it++)
	{
		//�����һ��û�����򷵻�
		if (it.value()->uploadFile->isOver == false)
		{
			/*qDebug() << it.value()->;*/
			return;
		}
	}
	//���еĶ��������
	if (!MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�ϴ����!"), MESSAGE_INFORMATION, BUTTON_OK, true))
		{
			ui->sendBtn->setEnabled(true);
		}
	//ֱ�ӳ�ʼ����Щmap
	init();
	ui->retranslateUi(this);
}

//���½�����
void UploadWindow::updataProgressBar(int num,qint64 byteToWrite,qint64 totalSize)
{
	QMap<int, QProgressBar*>::iterator it = fileProgressBarMap->find(num);
	//qDebug() << byteToWrite << " " << totalSize;
	it.value()->setMaximum(totalSize);// ȡ����ֵ���ô�С
	it.value()->setValue(totalSize - byteToWrite);  //���õ�ǰֵ
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
		tmpFile.close();
		icon = provider.icon(QFileInfo(fileName));
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

