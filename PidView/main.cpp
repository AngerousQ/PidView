//#include "PIDViews.h"
#include "stdafx.h"
//#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	//AllocConsole();		//�½�����̨��ӡ��Ϣ
	//FILE* stream;
	//freopen_s(&stream, "CONOUT$", "w", stdout);//�ض��������
    QApplication a(argc, argv);
    PIDViews w;
    w.show();
    return a.exec();
}
