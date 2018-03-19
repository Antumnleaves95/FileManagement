#include "DownloadManage.h"
#include "MyMessageBox.h"
#include "Database.h"
#include <QFileDialog>
#include <QToolTip>
#include <QtWin>
#include <QToolButton>
#include<QActionGroup>
#include <QFileIconProvider>
#include <QDesktopServices>
DownloadManage::DownloadManage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DownloadManage)
{
	ui->setupUi(this);
	initModel();
	initMap();
}

DownloadManage::~DownloadManage()
{
		
}

void DownloadManage::initModel()
{
	connect(ui->tableView, SIGNAL(entered(QModelIndex)),
		this, SLOT(showToolTip(QModelIndex)));

	connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));            //����Ҽ��˵�
	ui->tableView->setMouseTracking(true);   //�������׷��
	model = new QStandardItemModel();  //��ʼ��model
	model->setColumnCount(8);
	ui->tableView->setShowGrid(false);
	ui->tableView->verticalHeader()->setVisible(false);// ��ֱ���ɼ�
	//ui->tableView->horizontalHeader()->setVisible(false);// ˮƽ���ɼ�
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);



	model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit(""));
	model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("�ļ���"));
	model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("�ļ���С"));
	model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("����"));
	model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("�ٶ�"));
	model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("ʣ��ʱ��"));
	model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("��"));
	model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("ɾ��"));

	ui->tableView->setModel(model);
	ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui->tableView->setColumnWidth(0, 30);
	ui->tableView->setColumnWidth(1, 240);
	ui->tableView->setColumnWidth(2, 70);
	ui->tableView->setColumnWidth(3, 110);
	ui->tableView->setColumnWidth(4, 70);
	ui->tableView->setColumnWidth(5, 60);
	ui->tableView->setColumnWidth(6, 40);
	ui->tableView->setColumnWidth(7, 40);

	//�����п��ɱ� 
	ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
	ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
	//�����Ҽ��˵�
	setMenuEvent();
}
void DownloadManage::setMenuEvent()
{
	//�Ҽ��˵� 
	actionWait = new QAction(this);
	actionWait->setText(QStringLiteral("��ͣ"));
	actionBegin = new QAction(this);
	actionBegin->setText(QStringLiteral("��ʼ"));
	actionDelete = new QAction(this);
	actionDelete->setText(QStringLiteral("ɾ��"));
	actionOpenFile = new QAction(this);
	actionOpenFile->setText(QStringLiteral("���ļ�"));
	actionOpenFolder = new QAction(this);
	actionOpenFolder->setText(QStringLiteral("���ļ���"));

	connect(actionWait, SIGNAL(triggered()), this, SLOT(CallWaitDownload()));
	connect(actionBegin, SIGNAL(triggered()), this, SLOT(CallKeepOnDownload()));
	connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(CallOpenFile()));
	connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(CallOpenFolder()));
	//QActionGroup *actionGroup = new QActionGroup(this);
	//actionGroup->addAction(actionBegin);
	//actionGroup->addAction(actionWait);
	//actionBegin->setChecked(true);
	//actionGroup->setExclusive(true);

	popMenu = new QMenu(ui->tableView);
	popMenu->addAction(actionBegin);
	popMenu->addAction(actionWait);
	popMenu->addSeparator();   //��һ������
	popMenu->addAction(actionDelete);
	popMenu->addSeparator();   //��һ������
	popMenu->addAction(actionOpenFile);
	popMenu->addAction(actionOpenFolder);

	//actionBegin->setEnabled(false);
}

void DownloadManage::addBreakFile(int num, qint64 recordId, int fileId,
	QString fileName, QString filePath,
	qint64 breakPoint)
{
	BreakFile breakFile;
	breakFile.recordId = recordId;
	breakFile.fileId = fileId;
	breakFile.fileName = fileName;
	breakFile.breakPoint = breakPoint;
	breakFile.filePath = filePath;
	breakFileMap->insert(num,breakFile);
}
void DownloadManage::CallWaitDownload()
{
	//����ͣ��ť������ʱ��
	//actionBegin->setEnabled(true);
	QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
	QModelIndexList indexsSelected = selectionModel->selectedIndexes();
	QModelIndex indexSelected = indexsSelected.at(0);
	qDebug() << "waitRow" << indexSelected.row();

	QMap<int, int>::iterator it = fileStatusMap->find(indexSelected.row());
	it.value() = 2;
	downloadThreadMap->remove(indexSelected.row());
	

	//ѡ�е�����������ͣ�Ĳ���
	
	emit stopDownload(indexSelected.row());
}

void DownloadManage::CallKeepOnDownload()
{
	QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
	QModelIndexList indexsSelected = selectionModel->selectedIndexes();
	QModelIndex indexSelected = indexsSelected.at(0);

	QMap<int, BreakFile>::iterator it = breakFileMap->find(indexSelected.row());

	downloadThread = new DownloadThread(it.value().fileId,it.value().fileName,it.value().filePath, it.value().breakPoint,it.value().recordId, indexSelected.row(),true); //����new��һ���߳�
	qDebug() << "new download , main ThreadId :" << QThread::currentThreadId();
	downloadThreadMap->insert(indexSelected.row(), downloadThread);
	//fileStatusMap->insert(index, 0); //������������״̬
	//insertDownloadRecord(m_FileName, m_FilePath, "-1", -1, -1);
	connect(downloadThread, SIGNAL(finished()), downloadThread, SLOT(deleteLater()));
	connect(downloadThread, SIGNAL(downloadAvailable(int)), this, SLOT(beginToDownload(int)));
	//����Ҫ����һ���������󵽷������� 
	downloadThread->start();

	//�����ļ�״̬
	QMap<int, int>::iterator it1 = fileStatusMap->find(indexSelected.row());
	it1.value() = 1;

	
}

void DownloadManage::CallOpenFile()
{
	QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
	QModelIndexList indexsSelected = selectionModel->selectedIndexes();
	QModelIndex indexSelected = indexsSelected.at(0);
	QMap<int, BreakFile>::iterator it = breakFileMap->find(indexSelected.row());

	//QFileInfo fi = QFileInfo(fileFull);
	//QString filePath;
	//filePath = fi.absolutePath();

	QDesktopServices::openUrl(QUrl(it.value().filePath + it.value().fileName, QUrl::TolerantMode));
}

void DownloadManage::CallOpenFolder()
{
	QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
	QModelIndexList indexsSelected = selectionModel->selectedIndexes();
	QModelIndex indexSelected = indexsSelected.at(0);
	QMap<int, BreakFile>::iterator it = breakFileMap->find(indexSelected.row());

	//QFileInfo fi = QFileInfo(fileFull);
	//QString filePath;
	//filePath = fi.absolutePath();

	QDesktopServices::openUrl(QUrl(it.value().filePath, QUrl::TolerantMode));
}
void DownloadManage::showToolTip(const QModelIndex &index) {
	if (!index.isValid()) {
		qDebug() << "Invalid index";
		return;
	}
	QToolTip::showText(QCursor::pos(), index.data().toString());
}

void DownloadManage::initMap()
{
	downloadThreadMap = new QMap<int, DownloadThread*>;
	downloadProgressBarMap = new QMap<int, QProgressBar*>;
	downloadSpeedMap = new QMap<int, QLabel*>;
	downloadLeftTimeMap = new QMap<int, QLabel*>;
	fileSizeMap = new QMap<int, QLabel*>;
	fileStatusMap = new QMap<int, int>; 
	//fileIdMap = new QMap<int, int>;
	//fileNameMap = new QMap<int, QString>;
	breakFileMap = new QMap<int, BreakFile>;
	index = 0;
	queryLocalCache();
	
}



//-1����δ����
void DownloadManage::insertDownloadRecord(QString m_FileName, QString m_FilePath,QString m_FileSize,qint64 max,qint64 now)
{
	qDebug() << "insert download file" << m_FileName << m_FilePath;
	//������浽tableView

	
	//����÷���ǰ�棬��Ȼ��һ������ֱ�Ӳ��룬û��rowcount
	model->setItem(index, 1, new QStandardItem(m_FileName));

	mFileIcon = new QLabel();
	mFileName = new QLabel();
	mFileIcon->setMinimumSize(30, 35);
	setFileIcon(m_FileName);

	ui->tableView->setIndexWidget(model->index(model->rowCount() - 1, 0), mFileIcon);


	//ʵ��ͼ����ļ�����ͬһ����ڵڶ���
	//IconName = new QWidget();
	//QHBoxLayout *fileInfoLayout = new QHBoxLayout();
	//IconName->setLayout(fileInfoLayout);
	//mFileIcon = new QLabel(IconName);
	//mFileName = new QLabel(IconName);
	//setFileIcon(m_FileName);
	//QHBoxLayout *fileInfoLayout = new QHBoxLayout();
	//fileInfoLayout->setMargin(0);
	//mFileIcon = new QLabel();
	//mFileIcon->setMinimumSize(30, 35);
	////mFileIcon->setMaximumSize(10, 10);
	//mFileName = new QLabel();
	////mFileName->setMinimumSize(20, 25);
	//setFileIcon(m_FileName);
	//fileInfoLayout->insertWidget(0, mFileIcon);
	//fileInfoLayout->insertWidget(1, mFileName);
	//IconName = new QWidget();
	//IconName->setLayout(fileInfoLayout);
	//fileInfoLayout->setAlignment(mFileIcon, Qt::AlignLeft);
	//fileInfoLayout->setAlignment(mFileName, Qt::AlignLeft);
	//ui->tableView->setIndexWidget(model->index(model->rowCount() - 1, 1), IconName);
	//ui->horizontalLayout->insertWidget(0, IconName);


	mFileSize = new QLabel();
	QWidget *sizeWidget = new QWidget();
	QHBoxLayout *SizeLayout = new QHBoxLayout();
	SizeLayout->addWidget(mFileSize);
	SizeLayout->setAlignment(mFileSize, Qt::AlignLeft);
	sizeWidget->setLayout(SizeLayout);
	fileSizeMap->insert(index, mFileSize);
	if (m_FileSize != "-1")
		mFileSize->setText(m_FileSize);
	ui->tableView->setIndexWidget(model->index(model->rowCount() - 1, 2), sizeWidget);


	//�Ž�����
	QProgressBar *downloadProgressBar = new QProgressBar(this);
	//downloadProgressBar->hide();
	//downloadProgressBar->setMaximumHeight(20);
	downloadProgressBar->setTextVisible(false);
	
	if (max == -1)
	{
		downloadProgressBarMap->insert(index, downloadProgressBar);  //���������������map
	}
	else
	{
		qDebug() << "max :" << max << "now: " << now;
		downloadProgressBar->setMaximum(max);
		downloadProgressBar->setValue(now);
		downloadProgressBarMap->insert(index, downloadProgressBar);  //���������������map
		downloadProgressBar->show();
	}
	ui->tableView->setIndexWidget(model->index(model->rowCount() - 1, 3), downloadProgressBar);

	//���ٶ���
	mFileSpeed = new QLabel();
	QWidget *widgetSpeed = new QWidget();
	QHBoxLayout *SpeedLayout = new QHBoxLayout();
	SpeedLayout->addWidget(mFileSpeed);
	SpeedLayout->setAlignment(mFileSpeed, Qt::AlignLeft);
	widgetSpeed->setLayout(SpeedLayout);

	downloadSpeedMap->insert(index, mFileSpeed);
	ui->tableView->setIndexWidget(model->index(model->rowCount() - 1, 4), widgetSpeed);
	//model->item(model->rowCount() - 1, 4)->setTextAlignment(Qt::AlignCenter);  //����

	//��ʣ��ʱ��
	mLeftTime = new QLabel();
	downloadLeftTimeMap->insert(index, mLeftTime);
	ui->tableView->setIndexWidget(model->index(model->rowCount() - 1, 5), mLeftTime);
	ui->tableView->setModel(model);
	index++;
	
}
//��downloadWindow�����������ʱ�򣬲���һ�����ؼ�¼��downloadManage�������̣߳�
void DownloadManage::insertDownloadFile(QString m_FileName, QString m_FilePath)
{
	
	//����һ���µ��߳�
	QString downloadName = checkRename(m_FileName, m_FilePath);
	downloadThread = new DownloadThread(m_FileName,m_FilePath,index,false); //����new��һ���߳�
	qDebug() << "new download , main ThreadId :" <<QThread::currentThreadId();
	downloadThreadMap->insert(index, downloadThread);
	fileStatusMap->insert(index, 0); //������������״̬
	qDebug() << "downloadName:" << downloadName;
	insertDownloadRecord(downloadName, m_FilePath,"-1", -1, -1);
	connect(downloadThread, SIGNAL(finished()), downloadThread, SLOT(deleteLater()));
	connect(downloadThread, SIGNAL(downloadAvailable(int)), this, SLOT(beginToDownload(int)));
	//����Ҫ����һ���������󵽷������� 
	downloadThread->start();
	
}

//���źŲ�
void DownloadManage::beginToDownload(int num)
{
	//���ΰ��źŲ�
	qDebug() << num << "bind signal ";
	QMap<int, DownloadThread*>::iterator it = downloadThreadMap->find(num);
	//��Щ���Ǹ���UI���Լ��Ƿ������
	connect(it.value()->downloadFile, SIGNAL(downloadOver(int)), this, SLOT(checkDownloadOver(int)), Qt::BlockingQueuedConnection);
	connect(it.value()->downloadFile, SIGNAL(updateProgress(int, qint64, qint64)),
		this, SLOT(updateProgressBar(int, qint64, qint64)), Qt::BlockingQueuedConnection);
	connect(it.value()->downloadFile, SIGNAL(updateSpeedLabel(int, double,QString)),
		this, SLOT(updateSpeedLabel(int, double,QString)), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(stopDownload(int)), it.value()->downloadFile, SLOT(stopReceive(int)));
	connect(it.value()->downloadFile, SIGNAL(addToBreakFile(int, qint64,int,QString,QString,qint64)), this, SLOT(addBreakFile(int, qint64, int, QString, QString, qint64)));
	//connect(this, SIGNAL(sendFileSignal()), it.value()->downloadFile, SLOT(receiveSendSignal()));

}
void DownloadManage::checkDownloadOver(int num)
{
	//�������
	QMap<int, int>::iterator it = fileStatusMap->find(num);
	it.value() = 1;

	//QMap<int, DownloadThread*>::iterator it = downloadQThreadMap->find(num);
	//����һ��ͼƬ
	//qDebug() << num << "is send over";
	//if (it.value()->downloadFile->isOver == true)
	//{
	//	qDebug() << num << "is download over";
	//	QMap<int, QHBoxLayout*>::iterator it1 = fileStatusLayoutMap->find(num);
	//	QLabel *successLogo = new QLabel();
	//	successLogo->setPixmap(QPixmap(":/Resource/icon/correct.png"));
	//	QLabel *successMessage = new QLabel(QString::fromLocal8Bit("�ϴ��ɹ�"));

	//	it1.value()->insertWidget(0, successLogo);
	//	it1.value()->insertWidget(1, successMessage);
	//	QMap<int, QLabel*>::iterator it2 = fileSpeedMap->find(num);
	//	it2.value()->hide();
	//}
}

void DownloadManage::updateProgressBar(int num, qint64 byteToWrite, qint64 totalSize)
{
	QMap<int, QProgressBar*>::iterator itP = downloadProgressBarMap->find(num);
	itP.value()->setMaximum(totalSize);// ȡ����ֵ���ô�С
	itP.value()->setValue(byteToWrite);  //���õ�ǰֵ

	QMap<int, QLabel*>::iterator it1 = fileSizeMap->find(num);
	it1.value()->setText(countFileSize(QString::number(totalSize, 10)));
}

//�����ٶȣ����QString��downloadFile�������� 00::00::00��ʽ��
void DownloadManage::updateSpeedLabel(int num, double speed, QString leftTime)
{
	QMap<int, QLabel*>::iterator it1 = downloadSpeedMap->find(num);
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

	QMap<int, QLabel*>::iterator it2 = downloadLeftTimeMap->find(num);
	it2.value()->setText(leftTime);
}
void DownloadManage::setFileIcon(QString fileName)
{

	QString suffix = (fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1)).left(3); //��ȡ�ļ���׺ǰ�����ַ�
	QString allName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1); //��ȡ�ļ�����
	qDebug() << "the suffix :" << suffix << " and the allName:" << allName;
	// ��ȡͼ�ꡢ����
	QIcon icon = fileIcon(suffix);
	QString strType = fileType(suffix);
	qDebug() << strType;
	QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(30, 35)));
	//QPixmap fileIcon(icon.);  //�������ļ���ͼ��
	//fileIcon = fileIcon.scaled(QSize(30, 40), Qt::KeepAspectRatio);
	mFileIcon->setPixmap(pixmap);
	mFileIcon->setAlignment(Qt::AlignCenter);
	mFileName->setText(allName);

	//mFileIcon->show();
	//mFileName->show();
}
QIcon DownloadManage::fileIcon(const QString &extension) const
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

QString DownloadManage::fileType(const QString &extension) const
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
QString DownloadManage::countFileSize(QString fileSize)
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
}

//���ļ�·�����ļ���������
QString DownloadManage::checkRename(QString mFileName,QString mFilePath)
{
	QString suffix = mFileName.right(mFileName.size() - mFileName.lastIndexOf('.') - 1); //��ȡ�ļ���׺;
																					  //����ͷ������Ĳ�һ��
	QString fullName = mFileName.left(mFileName.size() - (mFileName.size() - mFileName.lastIndexOf('.'))); //��ȡ�ļ���
	
	int i = 0;																											   //��ѯ�ļ����Ƿ��ظ�
	while (1)
	{
		QFile *existFile = new QFile(mFilePath + mFileName);  //�����ж��Ƿ��ظ�
		qDebug() << "exitsFile" << mFilePath + mFileName;
		if (existFile->exists())   //����ļ�����
		{
			i++;
			mFileName = fullName + "(" + QString::number(i, 10) + ")." + suffix;

			qDebug() << "the fileName :" << mFileName;
		}
		else
			break;
	}
	return mFileName;
}

//��ѯ���ص����ؼ�¼
void DownloadManage::queryLocalCache()
{
	QString sql = "select * from DownloadRecord;";
	QList<QStringList> record = Database::queryDB(sql, 7);
	//�õ��������ؼ�¼
	qDebug() << "cache size:" << record.size();

	for (int i = 0; i < record.size(); i++)
	{
		qDebug() << "cache :"<<record.at(i).at(1);
		qDebug() << "sum: " << record.at(i).at(4).toInt();
		qDebug() << "break: " << record.at(i).at(5).toInt();
		
		QString cFileName = record.at(i).at(1);
		QString cFilePath = record.at(i).at(2);
		QString cFileSize = countFileSize(record.at(i).at(3));

		qint64 cSumBlock = record.at(i).at(4).toInt() ;
		qint64 cbreakPoint = record.at(i).at(5).toInt() ;
		int cfileId = record.at(i).at(6).toInt();
		//�������
		//����Ѿ��������
		if (cSumBlock == cbreakPoint)
		{
			fileStatusMap->insert(index, 1);
		}
		else
		{

			fileStatusMap->insert(index, 2);
		}
		BreakFile breakfile;
		breakfile.recordId = record.at(i).at(0).toInt();
		breakfile.fileId = cfileId;
		breakfile.fileName = cFileName;
		breakfile.filePath = cFilePath;
		breakfile.breakPoint = cbreakPoint;
		breakFileMap->insert(index, breakfile);
		insertDownloadRecord(cFileName, cFilePath, cFileSize, cSumBlock, cbreakPoint);
	}
}
void DownloadManage::slotContextMenu(QPoint pos) {

	QModelIndex current_index = ui->tableView->indexAt(pos);

	qDebug() << current_index;
	if (current_index.isValid())
	{
		//�ҵ�ѡ�е��ļ�״̬
		QMap<int, int>::iterator it = fileStatusMap->find(current_index.row());
		//���������
		if (it.value() == 0)
		{
			actionBegin->setEnabled(false);
			actionWait->setEnabled(true);
			actionOpenFile->setEnabled(false);
		}
		//�������
		else if (it.value() == 1)
		{
			actionBegin->setEnabled(false);
			actionWait->setEnabled(false);
			actionOpenFile->setEnabled(true);
		}
		else if (it.value() == 2)
		{
			actionBegin->setEnabled(true);
			actionWait->setEnabled(false);
			actionOpenFile->setEnabled(false);
		}
		popMenu->exec(QCursor::pos()); // �˵����ֵ�λ��Ϊ��ǰ����λ��
	}
}
