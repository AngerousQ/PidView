#include"stdafx.h"
#include "NoteDialog.h"
NoteDialog::NoteDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose); //���������� attribute n.���ԣ����ԡ�
	setWindowFlags(Qt::WindowStaysOnTopHint);//�ý��̴���ʼ�������ϲ�
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());  //���������ui.setupUi(this)����������Ч��  ����ԭʼ�߿�

	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(onCloseButtonSlot()));//
}

NoteDialog::~NoteDialog()
{
}
void NoteDialog::onCloseButtonSlot()
{
	this->close();
}