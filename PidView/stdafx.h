#pragma once
#include <QtWidgets>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <QSettings>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QtDebug>
#include <QThread>
#include <QSemaphore>
#include <QWaitCondition>
#include <QMutex>
#include <QDir>
#include <QSharedMemory>
#include <QObject>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QtCore/qglobal.h>

#ifdef Q_OS_WIN
//#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

#include "PidView.h"
#include "ui_PidView.h"

#include "Titlebar/titlebar.h"
#include "FlatUI/flatui.h"
//constexpr auto PID_POINTS_AMOUNT = 1000;				//ģ���㷨����ĵ���
#define CURVEWIDE   1//�����Դ���1.  ��ͼ���߿�
//#define POINTS_COUNT	500		//ģ���ͼʱ�ĵ���
////��һ��������PID�����ṹ�壬��������
//typedef struct _gPid {
//	float SetTargetHeight;					//�����趨ֵ(�ɻ���Ŀ��߶�)
//	float ActualHeight;						//ʵ��ֵ���ɻ���ʵ�ʸ߶ȣ�
//	float err;								//����ƫ��ֵ
//	float err_last;							//������һ��ƫ��ֵ
//	float Kp, Ki, Kd;						//������������֡�΢��ϵ��
//	float integral;							//�������ֵ
//	float OutSpeed;							//����ɻ��ٶȣ�����ִ�����ı�����ʵ����ͨ���������Ʒɻ����ٶȷ������С��
//	float LastOutSpeed;						//ͨ���ٶȱ仯���λ�Ʊ仯���ɻ���ʵ�ʸ߶�
//
//}MyPid;
extern QVector<double> HorizontalAxisValue;	//������ֵ
extern QVector<double> ActualValue;			//ִ��������������ֵ
extern QVector<double> DifferenceValue;		//Ŀ��ֵ�����ֵ�Ĳ�ֵ