#pragma once

#include <QDialog>
#include "ui_NoteDialog.h"

class NoteDialog : public QDialog
{
	Q_OBJECT

public:
	NoteDialog(QWidget *parent = Q_NULLPTR);
	~NoteDialog();
private slots:
	void onCloseButtonSlot();//���¹رռ�����
private:
	Ui::NoteDialog ui;
};
