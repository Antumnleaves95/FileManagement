#include"stdafx.h"
#include "Login.h"
#include "FileManagement.h"

#include "Register.h"
#include "UploadFile.h"
#include "MyMessageBox.h"

#include <QtWidgets/QApplication>
#include <qApplication.h>
#include <QResource>


//����ȫ�ֱ���


int main(int argc, char *argv[])
{
	//ͼƬ�ļ����
	QResource::registerResource("FileManagement.rcc");
	QApplication a(argc, argv);
	QApplication::addLibraryPath("./plugins");
	Login L;
	Register R;
	FileManagement F;
	//�������BUG�������ȳ�ʼ��R��Ҫ�ȳ�ʼ��L

	L.show();
	//�ۺ�����ʵ����ת
	QObject::connect(&L, SIGNAL(showRegister()), &R, SLOT(receiveLogin()));
	QObject::connect(&L, SIGNAL(showMain()), &F, SLOT(receiveLogin()));
	QObject::connect(&R, SIGNAL(showLogin()), &L, SLOT(receiveRegister()));
	QObject::connect(&F, SIGNAL(showLogin()), &L, SLOT(receiveMain()));
	return a.exec();
}
