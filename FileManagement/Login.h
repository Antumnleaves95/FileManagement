#pragma once

#include <QtWidgets/QDialog>
#include "ui_Login.h"

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);

private:
	Ui::Dialog ui;

//�ۺ���
public slots:
	void Click_Login();

	void Click_Register();

signals:
	void showMain();
	void showRegister();
};
