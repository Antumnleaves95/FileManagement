#include"stdafx.h"
#include "Login.h"
#include "FileManagement.h"

#include "Register.h"
#include "Database.h"
#include "MyMessageBox.h"
#include <Qdir.h>
#include <QtWidgets/QApplication>
#include <qApplication.h>


//����ȫ�ֱ���


int main(int argc, char *argv[])
{

	QString strLibPath(QDir::toNativeSeparators(QApplication::applicationDirPath()) +
		QDir::separator() + "plugins");  
	
	QApplication a(argc, argv);
	FileManagement F;
	Login L;
	Register R;
	L.show();

	//�ۺ�����ʵ����ת
	QObject::connect(&L, SIGNAL(showRegister()), &R, SLOT(receiveLogin()));
	QObject::connect(&L, SIGNAL(showMain()), &F, SLOT(receiveLogin()));
	QObject::connect(&R, SIGNAL(showLogin()), &L, SLOT(receiveRegister()));

	return a.exec();
}
