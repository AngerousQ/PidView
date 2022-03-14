#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PidView.h"
#include "NoteDialog.h"
#include "Titlebar/titlebar.h"
#include "PidAlgorithmThread/PidAlgorithmThread.h"
//��һ��������PID�����ṹ�壬��������
typedef struct _gPid {
	float SetTargetHeight;					//�����趨ֵ(�ɻ���Ŀ��߶�)
	float ActualHeight;						//ʵ��ֵ���ɻ���ʵ�ʸ߶ȣ�
	float err;								//����ƫ��ֵ
	float err_last;							//������һ��ƫ��ֵ
	float Kp, Ki, Kd;						//������������֡�΢��ϵ��
	float integral;							//�������ֵ
	float OutSpeed;							//����ɻ��ٶȣ�����ִ�����ı�����ʵ����ͨ���������Ʒɻ����ٶȷ������С��
	float LoadValue;						//���帺��ֵ������ģ��Ϊ����ķ���
	float LastOutSpeed;						//ͨ���ٶȱ仯���λ�Ʊ仯���ɻ���ʵ�ʸ߶�

}MyPid;
class PidView : public QMainWindow
{
    Q_OBJECT

public:
    PidView(QWidget *parent = Q_NULLPTR);
	void setViewInterface(QCustomPlot *customPlot);
	void PidViewGraph();
private:
	Ui::PidViewClass ui;
	TitleBar* pTitleBar;
	NoteDialog *pNoteDialog;
	RecordCalculateThread * dataCollection = NULL;               //���ݲɼ� �� �㷨���� �߳�

	QVector<QCheckBox*>CheckBoxSet;
	QVector<int>    GraphSelectedByCheckBox;         //��¼������Щ���α�ѡ��  0�����һ����

	QTimer ReplotGraphTimer;
	QTimer refreshAxisTimer;                //��Ϊ����һ�������ԭ�� ����Ҫʵʱ������������Ϣ
	QCPAxisRect *curveRect[4];              //��������ϵ
	QCPAxis *onlyCurveXAxis;               //���õ�ʱ����
	bool timeAxisSelectFlage = false;       //ʱ�����Ƿ񱻵�ѡ��־λ
	int selectRect;                         //�����Ӧ����¼�����ק�����죩��λ��

	//pid����
	void PID_GetDrawPoints();
	void PID_init(MyPid& VirtualPid);
	float PID_realize(MyPid& VirtualPid);
private slots:
	void onCheckBoxSet(int i);	//ѡ����ʱ�����ò�

	void ReplotGraphSlot();                          //���ݽ���ˢ��
	void RefreshAxisSlot();                     //��ʱˢ����������źŲ�
	void onPushButtonSlot();

	void on_horizontalSliderP_valueChanged(int value);
	void on_horizontalSliderI_valueChanged(int value);
	void on_horizontalSliderD_valueChanged(int value);

	void mousePress();
	void mouseWheel();
	void selectionChanged();
	void contextMenuRequest(QPoint pos);        //����Ҽ�  ͼ��λ
signals:
	void dataOperationSignal();                 //һ����������� �͸��߳� ʹ֮���������µ�����
};
