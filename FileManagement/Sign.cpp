#include "Sign.h"



Sign::Sign(QWidget *parent) :
	BaseWindow(parent),
	ui(new Ui::Sign)
{
	ui->setupUi(this);
	initModel();
	sendSignLook();
	connect(ui->tableView, SIGNAL(entered(QModelIndex)),
		this, SLOT(showToolTip(QModelIndex)));
}


Sign::~Sign()
{
}

void Sign::initModel()
{
	ui->tableView->setMouseTracking(true);   //�������׷��
	model = new QStandardItemModel();
	model->setColumnCount(2);
	ui->tableView->setShowGrid(false);
	ui->tableView->verticalHeader()->setVisible(false);// ��ֱ���ɼ�
													   //ui->tableView->horizontalHeader()->setVisible(false);// ˮƽ���ɼ�

	model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("ǩ��ʱ��"));
	model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("����"));

	ui->tableView->setModel(model);
	ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui->tableView->setColumnWidth(0, 200);
	ui->tableView->setColumnWidth(1, 100);

}

void Sign::showToolTip(const QModelIndex &index) {
	if (!index.isValid()) {
		qDebug() << "Invalid index";
		return;
	}
	QToolTip::showText(QCursor::pos(), index.data().toString());
}

void Sign::sendSignLook(QString condition)
{
	QString data = "QuerySign#" + condition;
	tcp->tcpSocket->write(data.toUtf8());
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	connect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showSignList()));
}

void Sign::showSignList()
{
	QByteArray dataread = tcp->tcpSocket->readAll();
	QString data = QString::fromUtf8(dataread);
	qDebug() << "show account the data from client: " << data;
	QStringList listNumber = data.split("$");

	QList<SignInfo> signInfo;
	for (int i = 1; i <= listNumber[0].toInt(); i++)
	{
		SignInfo sign;
		QStringList SignList = listNumber[i].split("#");
		qDebug() << SignList;
		sign.SignId = SignList[0].toInt();
		sign.SignTime = SignList[1];
		sign.userId = SignList[2].toInt();
		sign.userName = SignList[3];

		signInfo.append(sign);
	}

	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < listNumber[0].toInt(); i++)
	{
		//����ǰ���е�����
		model->setItem(i, 0, new QStandardItem(signInfo.at(i).SignTime));
		model->setItem(i, 1, new QStandardItem(signInfo.at(i).userName));  //countFileSize���ݴ�С��ʾKB����MB
		 
	}
	connect(tcp->tcpSocket, SIGNAL(readyRead()), tcp, SLOT(readMessages()));
	disconnect(tcp->tcpSocket, SIGNAL(readyRead()), this, SLOT(showSignList()));
}
