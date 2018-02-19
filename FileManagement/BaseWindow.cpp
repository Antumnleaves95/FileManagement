#include "BaseWindow.h"
#include "stdafx.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include<QBitmap>
#include <QFile>
#include <qdebug.h>

BaseWindow::BaseWindow(QWidget *parent)
	: QWidget(parent)
{
	// FramelessWindowHint�������ô���ȥ���߿�;
	// WindowMinimizeButtonHint ���������ڴ�����С��ʱ��������������ڿ�����ʾ��ԭ����;
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	// ���ô��ڱ���͸��;
	setAttribute(Qt::WA_TranslucentBackground);
	// �رմ���ʱ�ͷ���Դ;
	/*setAttribute(Qt::WA_DeleteOnClose);*/
	// ��ʼ��������;
	initTitleBar();

	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	effect->setBlurRadius(15);
	effect->setColor(Qt::black);
	effect->setOffset(0, 0);
	setGraphicsEffect(effect);


	//QBitmap bmp(this->size());
	//bmp.fill();
	//QPainter p(&bmp);
	//p.setPen(Qt::NoPen);
	//p.setBrush(Qt::black);
	//p.drawRoundedRect(bmp.rect(), 20, 20);
	//setMask(bmp);
	//setStyleSheet("border-radius:10px;");
}

BaseWindow::~BaseWindow() 
{

}

void BaseWindow::initTitleBar()
{
	m_titleBar = new TitleBar(this);
	m_titleBar->move(-5, 10);
	m_titleBar->raise();
	m_titleBar->setWindowBorderWidth(2);
	//m_titleBar->setBackgroundColor(255, 255, 255);
	m_titleBar->setButtonType(MIN_BUTTON);
	//m_titleBar->setTitleWidth(this->width());
	 
	connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

}

void BaseWindow::paintEvent(QPaintEvent* event)
{



	//���ô��ڱ���ɫ
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(10, 10, this->width() - 20, this->height() - 20);

	QPainter painter1(this);
	painter1.setRenderHint(QPainter::Antialiasing, true);
	painter1.fillPath(path, QBrush(QColor(247, 238, 214)));




	//QRect rect = this->rect();
	//rect.setWidth(rect.width() - 1);
	//rect.setHeight(rect.height() - 1);

	//QColor color(0, 0, 0, 50);
	//for (int i = 0; i<10; i++)
	//{
	//	QPainterPath path;
	//	path.setFillRule(Qt::WindingFill);
	//	path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
	//	color.setAlpha(150 - qSqrt(i) * 50);
	//	painter1.setPen(color);
	//	painter1.drawPath(path);
	//}

	//Բ�Ǵ���
	//QPainter painter2(this);
	//painter2.setRenderHint(QPainter::Antialiasing);  // �����;
	//painter2.setBrush(QBrush(QColor(247, 238, 214)));
	//painter2.setPen(Qt::transparent);
	//QRect rect = this->rect();
	//rect.setWidth(rect.width() - 15);
	//rect.setHeight(rect.height() - 15);
	////rect.setLeft(rect.height() - 10);
	////rect.setRight(rect.height() - 10);
	////painter2.drawRect(10, 10, this->width() - 20, this->height() - 20);
	//painter2.drawRoundedRect(rect, 15, 15);
	////Ҳ����QPainterPath ���ƴ��� painter.drawRoundedRect(rect, 15, 15);
	//{
	//	/*QPainterPath painterPath;
	//	painterPath.addRect(10, 10, this->width() - 20, this->height() - 20);
	//	painterPath.addRoundedRect(rect, 15, 15);
	//	painter2.drawPath(painterPath);*/
	//}

	return QWidget::paintEvent(event);
}

void BaseWindow::loadStyleSheet(const QString &sheetName)
{
	QFile file("Resource/qss/"+sheetName + ".qss");
	qDebug() << sheetName + ".qss";
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{

		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	else
		qDebug() << "is not qss";
}

void BaseWindow::onButtonMinClicked()
{
	showMinimized();
}

void BaseWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	m_titleBar->getRestoreInfo(windowPos, windowSize);
	this->setGeometry(QRect(windowPos, windowSize));
}

void BaseWindow::onButtonMaxClicked()
{
	m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(FactRect);
}

void BaseWindow::onButtonCloseClicked()
{
	close();
}