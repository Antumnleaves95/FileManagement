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
	QAxObject excel("Excel.Application");
	excel.setProperty("Visible", true);
	QAxObject *work_books = excel.querySubObject("WorkBooks");
	work_books->dynamicCall("Open(const QString&)", "E:\\test121.xlsx");
	excel.setProperty("Caption", "Qt Excel");
	QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
	QAxObject *work_sheets = work_book->querySubObject("Sheets");  //SheetsҲ�ɻ���WorkSheets  

																   //ɾ��������ɾ����һ����  
	QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
	first_sheet->dynamicCall("delete");

	//���빤�������������һ�У�  
	int sheet_count = work_sheets->property("Count").toInt();  //��ȡ��������Ŀ  
	QAxObject *last_sheet = work_sheets->querySubObject("Item(int)", sheet_count);
	QAxObject *work_sheet = work_sheets->querySubObject("Add(QVariant)", last_sheet->asVariant());
	last_sheet->dynamicCall("Move(QVariant)", work_sheet->asVariant());

	work_sheet->setProperty("Name", "Qt Sheet");  //���ù���������  

												  //������Ԫ�񣨵�2�е�2�У�  
	QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", 2, 2);
	cell->setProperty("Value", "Java C++ C# PHP Perl Python Delphi Ruby");  //���õ�Ԫ��ֵ  
	cell->setProperty("RowHeight", 50);  //���õ�Ԫ���и�  
	cell->setProperty("ColumnWidth", 30);  //���õ�Ԫ���п�  
	cell->setProperty("HorizontalAlignment", -4108); //����루xlLeft����-4131  ���У�xlCenter����-4108  �Ҷ��루xlRight����-4152  
	cell->setProperty("VerticalAlignment", -4108);  //�϶��루xlTop��-4160 ���У�xlCenter����-4108  �¶��루xlBottom����-4107  
	cell->setProperty("WrapText", true);  //���ݹ��࣬�Զ�����  
										  //cell->dynamicCall("ClearContents()");  //��յ�Ԫ������  

	QAxObject* interior = cell->querySubObject("Interior");
	interior->setProperty("Color", QColor(0, 255, 0));   //���õ�Ԫ�񱳾�ɫ����ɫ��  

	QAxObject* border = cell->querySubObject("Borders");
	border->setProperty("Color", QColor(0, 0, 255));   //���õ�Ԫ��߿�ɫ����ɫ��  

	QAxObject *font = cell->querySubObject("Font");  //��ȡ��Ԫ������  
	font->setProperty("Name", QStringLiteral("���Ĳ���"));  //���õ�Ԫ������  
	font->setProperty("Bold", true);  //���õ�Ԫ������Ӵ�  
	font->setProperty("Size", 20);  //���õ�Ԫ�������С  
	font->setProperty("Italic", true);  //���õ�Ԫ������б��  
	font->setProperty("Underline", 2);  //���õ�Ԫ���»���  
	font->setProperty("Color", QColor(255, 0, 0));  //���õ�Ԫ��������ɫ����ɫ��  

													//���õ�Ԫ�����ݣ����ϲ���Ԫ�񣨵�5�е�3��-��8�е�5�У�  
	QAxObject *cell_5_6 = work_sheet->querySubObject("Cells(int,int)", 5, 3);
	cell_5_6->setProperty("Value", "Java");  //���õ�Ԫ��ֵ  
	QAxObject *cell_8_5 = work_sheet->querySubObject("Cells(int,int)", 8, 5);
	cell_8_5->setProperty("Value", "C++");

	QString merge_cell;
	merge_cell.append(QChar(3 - 1 + 'A'));  //��ʼ��  
	merge_cell.append(QString::number(5));  //��ʼ��  
	merge_cell.append(":");
	merge_cell.append(QChar(5 - 1 + 'A'));  //��ֹ��  
	merge_cell.append(QString::number(8));  //��ֹ��  
	QAxObject *merge_range = work_sheet->querySubObject("Range(const QString&)", merge_cell);
	merge_range->setProperty("HorizontalAlignment", -4108);
	merge_range->setProperty("VerticalAlignment", -4108);
	merge_range->setProperty("WrapText", true);
	merge_range->setProperty("MergeCells", true);  //�ϲ���Ԫ��  
												   //merge_range->setProperty("MergeCells", false);  //��ֵ�Ԫ��  

												   //work_book->dynamicCall("Save()");  //�����ļ���Ϊ�˶Ա�test�������test2�ļ������ﲻ����������� work_book->dynamicCall("SaveAs(const QString&)", "E:\\test2.xlsx");  //���Ϊ��һ���ļ�   
	work_book->dynamicCall("Close(Boolean)", false);  //�ر��ļ�  
	excel.dynamicCall("Quit(void)");  //�˳�  
}