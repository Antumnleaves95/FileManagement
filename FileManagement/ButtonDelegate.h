#pragma once
#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QItemDelegate>

class ButtonDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	explicit ButtonDelegate(QObject *parent = 0);

	//��ͼ��
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	//��ť�¼��༭
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:

	public slots :

private:
	QMap<QModelIndex, QStyleOptionButton*> m_btns;

};

#endif // BUTTONDELEGATE_H