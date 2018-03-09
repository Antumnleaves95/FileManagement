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
}

//��Ҫ�����������ļ����½��߳�
void UploadWindow::insertFile(QString m_fileName)
{
	QProgressBar *fileProgressBar = new QProgressBar(this);
	fileProgressBar->setTextVisible(false);   //����ʾ�ٷֱ�
	fileProgressBarMap->insert(index, fileProgressBar);  //���������������map
	stackLayout = new QStackedLayout();
	fileWidget = new QWidget(this);
	 //fileWidget->setLayout(fileInfoLayout);  //����ˮƽlayout
	 //�����ļ�������
	 fileInfoLayout = new QHBoxLayout(fileWidget);
	 mFileIcon = new QLabel(fileWidget);
	 mFileName = new QLabel(fileWidget);
	 fileInfoLayout->addWidget(mFileIcon);
	 fileInfoLayout->addWidget(mFileName);
	
	 setFileIcon(m_fileName);	//���ø��ļ������ֺ�ͼ��
	
	 stackLayout->addWidget(fileWidget);
	 stackLayout->addWidget(fileProgressBar);
	 fileProgressBar->setTextVisible(false);
	 stackLayout->setStackingMode(QStackedLayout::StackAll);
	 ui->FileListLayout->addLayout(stackLayout);  //��ֱ�ķ��ö���ļ�

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
}

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
	QMap<int, UploadFile*>::iterator it;
	for (it = uploadFileMap->begin(); it != uploadFileMap->end(); it++)
	{
		//�����һ��û�����򷵻�
		if (it.value()->isOver == false)
		{
			/*qDebug() << it.value()->;*/
			return;
		}
	}
	//QMap<int, UploadThread*>::iterator it1;
	//for (it1 = uploadQThreadMap->begin(); it1 != uploadQThreadMap->end(); it1++)
	//{
	//	it1.value()->~UploadThread();
	//}
	//���еĶ��������
	if (!MyMessageBox::showMyMessageBox(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�ϴ����!"), MESSAGE_INFORMATION, BUTTON_OK, true))
		{
			ui->sendBtn->setEnabled(true);
		}
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

