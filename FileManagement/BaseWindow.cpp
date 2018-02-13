#include "BaseWindow.h"
#include "stdafx.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <qdebug.h>

BaseWindow::BaseWindow(QWidget *parent)
	: QWidget(parent)
{
	// FramelessWindowHint属性设置窗口去除边框;
	// WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	// 设置窗口背景透明;
	setAttribute(Qt::WA_TranslucentBackground);
	// 关闭窗口时释放资源;
	/*setAttribute(Qt::WA_DeleteOnClose);*/
	// 初始化标题栏;
	initTitleBar();
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
	//设置背景色;
	//QPainter painter(this);
	//QPainterPath pathBack;
	//pathBack.setFillRule(Qt::WindingFill);
	//pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
	//painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	//painter.fillPath(pathBack, QBrush(QColor(247, 238, 214)));

	//设置窗口阴影
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(10, 10, this->width() - 20, this->height() - 20);

	QPainter painter1(this);
	painter1.setRenderHint(QPainter::Antialiasing, true);
	painter1.fillPath(path, QBrush(QColor(247, 238, 214)));

	QColor color(0, 0, 0, 50);
	for (int i = 0; i<10; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
		color.setAlpha(150 - qSqrt(i) * 50);
		painter1.setPen(color);
		painter1.drawPath(path);
	}

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