#include "MoneyManage.h"
#include "Echarts.h"
#include "Accounting.h"
#include <QToolTip>

MoneyManage::MoneyManage(QWidget *parent) :
	QWidget(parent), ui(new Ui::MoneyManage)
{
	ui->setupUi(this);
	initModel();
	//ui->ShowLastLabel->setPixmap(QPixmap(":/Resource/moneyBackground.jpg"));
	Echarts *echarts = new Echarts();
	//�������ݵ�ͼ��
	connect(this, SIGNAL(sendToCharts(QString)), echarts->mChartManager, SLOT(slot1(QString)));
	ui->verticalLayout->addWidget(echarts);
	connect(ui->tableView, SIGNAL(entered(QModelIndex)),
		this, SLOT(showToolTip(QModelIndex)));
}


void MoneyManage::initModel()
{
	ui->tableView->setMouseTracking(true);   //�������׷��
	model = new QStandardItemModel();
	model->setColumnCount(5);
	ui->tableView->setShowGrid(false);
	ui->tableView->verticalHeader()->setVisible(false);// ��ֱ���ɼ�
	//ui->tableView->horizontalHeader()->setVisible(false);// ˮƽ���ɼ�

	model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("����"));
	model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("���"));
	model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("ʱ��"));
	model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("��ע"));
	model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("������"));
	ui->tableView->setModel(model);
	ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui->tableView->setColumnWidth(0, 80);
	ui->tableView->setColumnWidth(1, 80);
	ui->tableView->setColumnWidth(2, 100);
	ui->tableView->setColumnWidth(3, 150);
	ui->tableView->setColumnWidth(4, 50);
}

MoneyManage::~MoneyManage()
{
	delete ui;
}

void MoneyManage::showToolTip(const QModelIndex &index) {
	if (!index.isValid()) {
		qDebug() << "Invalid index";
		return;
	}
	QToolTip::showText(QCursor::pos(), index.data().toString());
}

void MoneyManage::sendAccountLook(QString condition )
{
	QString data = "QueryAccount#" + condition;
	tcp->tcpSocket->write(data.toUtf8());
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showAccount()));
}

void MoneyManage::showAccount()
{
	QByteArray dataread = tcp->tcpSocket->readAll();
	QString data = QString::fromUtf8(dataread);
	qDebug() << "show account the data from client: " << data;
	emit sendToCharts(data);
	QStringList listNumber = data.split("$");
	//qDebug() << "adsadas" << listNumber[0].toInt();
	
	QList<AccountInfo> accountInfo;
	for (int i = 1; i <= listNumber[0].toInt(); i++)
	{
		AccountInfo account;
		QStringList accountList = listNumber[i].split("#");
		qDebug() << accountList;
		account.accountId = accountList[0].toInt();
		account.money = accountList[1].toInt();
		account.type = accountList[2];
		account.time = accountList[3];
		account.remark = accountList[4];
		account.name = accountList[5];
		accountInfo.append(account);
	}
	qDebug() << "test";

	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < listNumber[0].toInt() ; i++)
	{
		//����ǰ���е�����
		model->setItem(i, 0, new QStandardItem(accountInfo.at(i).type));
		model->setItem(i, 1, new QStandardItem(QString::number(accountInfo.at(i).money)));  //countFileSize���ݴ�С��ʾKB����MB
		model->setItem(i, 2, new QStandardItem(accountInfo.at(i).time));
		model->setItem(i, 3, new QStandardItem(accountInfo.at(i).remark));
		model->setItem(i, 4, new QStandardItem(accountInfo.at(i).name));
	}
	connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showAccount()));
}

void MoneyManage::clickAccountingButton()
{
	Accounting *accounting = new Accounting(this);
	accounting->move(100,100);
	accounting->show();
}

//�鿴�����˱�
void MoneyManage::clickLookAccount()
{

}