#include "Echarts.h"
#include "ui_Echarts.h"
#include "websocketclientwrapper.h"
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QDebug>
#include <QWebSocketServer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QFileInfo>
#include <ActiveQt\QAxObject>
#include <ActiveQt\QAxBase>
#include <QFileDialog>
#include <ActiveQt/QAxBase>
#include "previewpage.h"
#include "AccountInfo.h"
#include "MyMessageBox.h"


Echarts::Echarts(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Echarts)
{
	ui->setupUi(this);
	
	mChartManager = new BDChart(this);
	PreviewPage *page = new PreviewPage(this);
	mWebView = new QWebEngineView(this);
	mWebView->setPage(page);
	QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
	QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
	channel = new QWebChannel(mWebView->page());

	channel->registerObject(QStringLiteral("chartManager"), mChartManager); //��ҳ������
	mWebView->page()->setWebChannel(channel);
	mWebView->setUrl(QUrl("qrc:/chart/web/html/index.html"));

	ui->verticalLayout->addWidget(mWebView);
	//this->setCentralWidget(mWebView);
	//this->resize(1000,600);
	// connects();
	
	connect(ui->send, SIGNAL(clicked()), mChartManager, SLOT(slot1())); //�ۺ�������ҳ���� 
}

Echarts::~Echarts()
{
	delete ui;
}

//void Echarts::connects()
//{
// connect(ui->actionSend, SIGNAL(triggered(bool)), mChartManager, SLOT(slotTest()));
//}

void Echarts::receiveDataFromMoney(QString data)
{
	dataToExcel = data;
}
void  Echarts::import_excel()
{

	QAxObject *excel = NULL;
	QAxObject *workbooks = NULL;
	QAxObject *workbook = NULL;
	excel = new QAxObject("Excel.Application");
	if (!excel)
	{
		QMessageBox::critical(this, "������Ϣ", "EXCEL����ʧ");
		return;
	}
	excel->dynamicCall("SetVisible(bool)", false);
	workbooks = excel->querySubObject("WorkBooks");
	QString  str = ui->lineEdit->text();
	const char*  ch;
	QByteArray ba = str.toLatin1();
	ch = ba.data();
	
	workbook = workbooks->querySubObject("Open(QString, QVariant)", QString(tr(ch)));
	QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);//�򿪵�һ��sheet
																		   //QAxObject * worksheet = workbook->querySubObject("WorkSheets");//��ȡsheets�ļ���ָ��
																		   //int intCount = worksheet->property("Count").toInt();//��ȡsheets������

	QAxObject * usedrange = worksheet->querySubObject("UsedRange");//��ȡ��sheet��ʹ�÷�Χ����
	QAxObject * rows = usedrange->querySubObject("Rows");
	QAxObject * columns = usedrange->querySubObject("Columns");
	/*��ȡ����������*/
	int intRowStart = usedrange->property("Row").toInt();
	int intColStart = usedrange->property("Column").toInt();
	int intCols = columns->property("Count").toInt();
	int intRows = rows->property("Count").toInt();
	/*��ȡexcel����*/


	QString data ;
	QString edata;
	QString qdata;
	QString outdata;
	QString indata;
	QString ddata = "OutcomeAccount#";
	QString adata = "IncomeAccount#";
	for (int i = intRowStart; i < intRowStart + intRows; i++)  //��
	{
		for (int j = intColStart; j < intColStart + intCols; j++)  //��
		{
			QAxObject * cell = worksheet->querySubObject("Cells(int,int)", i, j);  //��ȡ��Ԫ��
																				   // qDebug() << i << j << cell->property("Value");         //*****************************������!!!!!!
			QAxObject *celt = worksheet->querySubObject("Cells(int,int,)", i, 1); 
			data = cell->dynamicCall("Value2()").toString();
			if (celt->dynamicCall("Value2()").toInt() < 0)
			{	
				ddata +=  data + "#";
			}
			if(celt ->dynamicCall("Value2()").toInt() > 0)
			{
				adata +=  data + "#";
			}
			qDebug() << i << j << cell->dynamicCall("Value2()").toString(); //��ȷ
		
		}
		outdata = ddata + "$";
		indata = adata + "$";
		
		qDebug() << adata;
	}
	outdata=outdata+ globalUserName;
	indata=indata+ globalUserName;
	//tcp->tcpSocket->write(outdata.toUtf8());
	tcp->tcpSocket->write(indata.toUtf8());

	MyMessageBox::showMyMessageBox(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����ɹ�!"),
		MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);

	workbook->dynamicCall("Close (Boolean)", false);
	//ͬ��������ֵ��Ҳ��dynamimcCall("SetValue(const QVariant&)", QVariant(QString("Help!")))�����ųɹ��ġ���
	//excel->dynamicCall("Quit (void)");
	delete excel;//һ��Ҫ�ǵ�ɾ����Ҫ���߳��л�һֱ��excel.exe
}


void Echarts::choosefilepath() //ѡ���ļ�·��
{
	QString file_full, file_name, file_path;
	QFileInfo fi;
	file_full = QFileDialog::getOpenFileName(this);
	fi = QFileInfo(file_full);
	file_name = fi.fileName();
	file_path = fi.absolutePath();
	ui->lineEdit->setText(file_path + "\\" + file_name);

}
//void Echarts::exportexcel()
//{
//	qDebug() << 1223;
//}
void Echarts::export_excel()
{
	/*receiveDataFromMoney(QString data);*/
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	
	QStringList listNumber = dataToExcel.split("$");
	QList<AccountInfo> accountInfo;		//�����в���ͬ�����͵Ľṹ��
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
	qDebug() << accountInfo.size()+121312;
	QString filepath = QFileDialog::getSaveFileName(this, tr("Save orbit"), ".", tr("Microsoft Office 2007 (*.xlsx)"));//��ȡ����·��
	if (!filepath.isEmpty()) {
		QAxObject *excel = new QAxObject(this);
		excel->setControl("Excel.Application");//����Excel�ؼ�
		excel->dynamicCall("SetVisible (bool Visible)", "false");//����ʾ����
		excel->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ�����Ϊtrue��ô�ڹر��ǻ�������ơ��ļ����޸ģ��Ƿ񱣴桱����ʾ

		QAxObject *workbooks = excel->querySubObject("WorkBooks");//��ȡ����������
		workbooks->dynamicCall("Add");//�½�һ��������
		QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//��ȡ��ǰ������
		QAxObject *worksheets = workbook->querySubObject("Sheets");//��ȡ��������
		QAxObject *worksheet = worksheets->querySubObject("Item(int)", 1);//��ȡ�������ϵĹ�����1����sheet1

		QAxObject *cellA, *cellB, *cellC, *cellD;

		//���ñ���
		
		int cellrow =1;
		qDebug() << accountInfo.size();


		QString A = "A" + QString::number(cellrow);//����Ҫ�����ĵ�Ԫ����A1
		QString B = "B" + QString::number(cellrow);
		QString C = "C" + QString::number(cellrow);
		QString D = "D" + QString::number(cellrow);
		cellA = worksheet->querySubObject("Range(QVariant, QVariant)", A);//��ȡ��Ԫ��
		cellB = worksheet->querySubObject("Range(QVariant, QVariant)", B);
		cellC = worksheet->querySubObject("Range(QVariant, QVariant)", C);
		cellD = worksheet->querySubObject("Range(QVariant, QVariant)", D);
		cellA->dynamicCall("SetValue(const QVariant&)", QVariant("type"));//���õ�Ԫ���ֵ
		cellB->dynamicCall("SetValue(const QVariant&)", QVariant("money"));
		cellC->dynamicCall("SetValue(const QVariant&)", QVariant("time"));
		cellD->dynamicCall("SetValue(const QVariant&)", QVariant("remark"));
		cellrow++;

		int rows = accountInfo.size();
		for (int i = 0; i<rows; i++) {
			QString A = "A" + QString::number(cellrow);//����Ҫ�����ĵ�Ԫ����A1
			QString B = "B" + QString::number(cellrow);
			QString C = "C" + QString::number(cellrow);
			QString D = "D" + QString::number(cellrow);
			cellA = worksheet->querySubObject("Range(QVariant, QVariant)", A);//��ȡ��Ԫ��
			cellB = worksheet->querySubObject("Range(QVariant, QVariant)", B);
			cellC = worksheet->querySubObject("Range(QVariant, QVariant)", C);
			cellD = worksheet->querySubObject("Range(QVariant, QVariant)", D);
			cellA->dynamicCall("SetValue(const QVariant&)", QVariant(accountInfo.at(i).type));//���õ�Ԫ���ֵ
			cellB->dynamicCall("SetValue(const QVariant&)", QVariant(accountInfo.at(i).money));
			cellC->dynamicCall("SetValue(const QVariant&)", QVariant(accountInfo.at(i).time));
			cellD->dynamicCall("SetValue(const QVariant&)", QVariant(accountInfo.at(i).remark));
			qDebug() << accountInfo.at(i).type;
			cellrow++;
		}
		
		workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filepath));//������filepath��ע��һ��Ҫ��QDir::toNativeSeparators��·���е�"/"ת��Ϊ"\"����Ȼһ�����治�ˡ�
		workbook->dynamicCall("Close()");//�رչ�����
		excel->dynamicCall("Quit()");//�ر�excel
		delete excel;
		MyMessageBox::showMyMessageBox(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�����ɹ�!"),
			MESSAGE_INFORMATION, BUTTON_OK_AND_CANCEL);
		excel = NULL;
	}
	
	
}