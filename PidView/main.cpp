#include "stdafx.h"

int main(int argc, char *argv[])
{
	//AllocConsole();		//�½�����̨��ӡ��Ϣ
	//FILE* stream;
	//freopen_s(&stream, "CONOUT$", "w", stdout);//�ض��������
    QApplication a(argc, argv);
    PidView w;
    w.show();
    return a.exec();
}
