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
	connect(ui->send, SIGNAL(clicked()), mChartManager, SLOT(slot1())); //�ۺ�������ҳ���� ���ɷ����źŵ�Echarts ���յ���û�з�Ӧ
}

Echarts::~Echarts()
{
	delete ui;
}

//void Echarts::connects()
//{
// connect(ui->actionSend, SIGNAL(triggered(bool)), mChartManager, SLOT(slotTest()));
//}
//void  Echarts::importexcel()
//{
//	//QAxObject *excel  = new QAxObject("Excel.Application");
//	//QAxObject *workbooks = new QAxObject();
//	//QAxObject *workbook = new QAxObject();
//	//
//	//if (!excel) { QMessageBox::critical(this, "������Ϣ", "EXCEL����ʧ"); return; }
//	//excel->dynamicCall("SetVisible(bool)", false);
//	//workbooks = excel->querySubObject("WorkBooks");
//	//workbook = workbooks->querySubObject("Open(QString, QVariant)", QString(tr("g:\BFS.xls"))); QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);//�򿪵�һ��sheet
//	//QAxObject * usedrange = worksheet->querySubObject("UsedRange");//��ȡ��sheet��ʹ�÷�Χ����
//	//QAxObject * rows = usedrange->querySubObject("Rows");
//	//QAxObject * columns = usedrange->querySubObject("Columns");
//	//int intRowStart = usedrange->property("Row").toInt();
//	//int intColStart = usedrange->property("Column").toInt();
//	//int intCols = columns->property("Count").toInt();
//	//int intRows = rows->property("Count").toInt();
//
//	//for (int i = intRowStart; i < intRowStart + intRows; i++) //�� 
//	//{
//	//	for (int j = intColStart; j < intColStart + intCols; j++) //��
//	//	{
//	//		QAxObject * range = worksheet->querySubObject("Cells(int,int)", i, j); //��ȡ��Ԫ��  
//	//		qDebug() << i << j << range->property("Value"); //************������!!!!!!
//	//	}
//	//}
//	QAxObject *excel = NULL;
//	QAxObject *workbooks = NULL;
//	QAxObject *workbook = NULL;
//	excel = new QAxObject("Excel.Application");
//	if (!excel)
//	{
//		QMessageBox::critical(this, "������Ϣ", "EXCEL����ʧ");
//		return;
//	}
//	excel->dynamicCall("SetVisible(bool)", false);
//	workbooks = excel->querySubObject("WorkBooks");
//	workbook = workbooks->querySubObject("Open(QString, QVariant)", QString(tr("E:\\test.xlsx")));
//	QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);//�򿪵�һ��sheet
//																		   //QAxObject * worksheet = workbook->querySubObject("WorkSheets");//��ȡsheets�ļ���ָ��
//																		   //int intCount = worksheet->property("Count").toInt();//��ȡsheets������
//
//	QAxObject * usedrange = worksheet->querySubObject("UsedRange");//��ȡ��sheet��ʹ�÷�Χ����
//	QAxObject * rows = usedrange->querySubObject("Rows");
//	QAxObject * columns = usedrange->querySubObject("Columns");
//	/*��ȡ����������*/
//	int intRowStart = usedrange->property("Row").toInt();
//	int intColStart = usedrange->property("Column").toInt();
//	int intCols = columns->property("Count").toInt();
//	int intRows = rows->property("Count").toInt();
//	/*��ȡexcel����*/
//	for (int i = intRowStart; i < intRowStart + intRows; i++)  //��
//	{
//		for (int j = intColStart; j < intColStart + intCols; j++)  //��
//		{
//			QAxObject * cell = worksheet->querySubObject("Cells(int,int)", i, j);  //��ȡ��Ԫ��
//																				   // qDebug() << i << j << cell->property("Value");         //*****************************������!!!!!!
//			qDebug() << i << j << cell->dynamicCall("Value2()").toString(); //��ȷ
//		}
//	}
//	workbook->dynamicCall("Close (Boolean)", false);
//	//ͬ��������ֵ��Ҳ��dynamimcCall("SetValue(const QVariant&)", QVariant(QString("Help!")))�����ųɹ��ġ���
//	//excel->dynamicCall("Quit (void)");
//	delete excel;//һ��Ҫ�ǵ�ɾ����Ҫ���߳��л�һֱ��excel.exe
//}

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
void Echarts::exportexcel()
{
	qDebug() << 1223;
}
void Echarts::onimportexcel()
{
	//QAxObject excel("Excel.Application");
	//excel.setProperty("Visible", false);
	//QAxObject *work_books = excel.querySubObject("WorkBooks");
	//work_books->dynamicCall("Open (const QString&)", "E:/test.xlsx");
	//QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
	//QAxObject *work_sheets = work_book->querySubObject("Sheets");  //SheetsҲ�ɻ���WorkSheets
	//int sheet_count = work_sheets->property("Count").toInt();  //��ȡ��������Ŀ
	//if (sheet_count > 0)
	//{
	//	QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
	//	QAxObject *used_range = work_sheet->querySubObject("UsedRange");
	//	QAxObject *rows = used_range->querySubObject("Rows");
	//	int row_count = rows->property("Count").toInt();  //��ȡ����
	//	QString txt = work_sheet->querySubObject("Cells(int,int)", i, 1)->property("Value").toString(); //��ȡ��Ԫ������
	//	work_book->dynamicCall("Close(Boolean)", false);  //�ر��ļ�
	//	excel.dynamicCall("Quit(void)");  //�˳�
	//}
}