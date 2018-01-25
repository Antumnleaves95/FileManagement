#include "buttondelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QDialog>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <qdebug.h>

ButtonDelegate::ButtonDelegate(QObject *parent) :
	QItemDelegate(parent)
{
}


//�水ť��
void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//����indexһ�У�ʲô��Ӧ���ǻ�������ʵ�ֵ�
	QStyleOptionButton* button = m_btns.value(index);
	if (!button) {
		button = new QStyleOptionButton();
		button->rect = option.rect.adjusted(4, 4, -4, -4);
		button->text = "����";
		button->state |= QStyle::State_Enabled;

		(const_cast<ButtonDelegate *>(this))->m_btns.insert(index, button);
	}
	painter->save();

	if (option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());

	}
	painter->restore();
	QApplication::style()->drawControl(QStyle::CE_PushButton, button, painter);


}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	//����
	if (event->type() == QEvent::MouseButtonPress) {

		QMouseEvent* e = (QMouseEvent*)event;

		if (option.rect.adjusted(4, 4, -4, -4).contains(e->x(), e->y()) && m_btns.contains(index)) {
			m_btns.value(index)->state |= QStyle::State_Sunken;
		}
		return true;
	}

	//����¼����ɿ���
	if (event->type() == QEvent::MouseButtonRelease) {
		QMouseEvent* e = (QMouseEvent*)event;

		if (option.rect.adjusted(4, 4, -4, -4).contains(e->x(), e->y()) && m_btns.contains(index)) {
			m_btns.value(index)->state &= (~QStyle::State_Sunken);
			QString fileName = index.model()->data(index).toString();
			qDebug() << index<<fileName;
			QDialog *d = new QDialog();

			d->setGeometry(0, 0, 200, 200);
			d->move(QApplication::desktop()->screenGeometry().center() - d->rect().center());
			d->show();
			return true;
		}
	}
	return false;
}