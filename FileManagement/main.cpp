#include"stdafx.h"
#include "Login.h"
#include "FileManagement.h"

#include "Register.h"
#include "UploadFile.h"
#include "MyMessageBox.h"

#include <QtWidgets/QApplication>
#include <qApplication.h>


//����ȫ�ֱ���


int main(int argc, char *argv[])
{

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
