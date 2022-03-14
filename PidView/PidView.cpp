//#include "PIDViews.h"
#include "stdafx.h"
PIDViews::PIDViews(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose); //���������� attribute n.���ԣ����ԡ�
	setWindowFlags(Qt::WindowStaysOnTopHint);//�ý��̴���ʼ�������ϲ�
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());  //�����������������������Ч��  ����ԭʼ�߿�
	ui.statusBar->setStyleSheet(QString("QStatusBar{color: rgb(255, 255, 255);background: rgb(0, 136, 236); }"));
	ui.statusBar->showMessage(QStringLiteral("����"));
		//�����������һ��  �Ϳ����� �������ϲ��ʹ
	ui.customPlot->setContextMenuPolicy(Qt::CustomContextMenu);//�滮�Ҽ����Ժ������źŲۣ��Ҽ��˵�ָʾ�������Ĳ˵�������
// ���Ӳۣ���һ���ᱻѡ��ʱ�����������������϶�������

	connect(ui.pushButton1, SIGNAL(clicked()), this, SLOT(onPushButtonSlot()));//
	connect(ui.pushButton2, SIGNAL(clicked()), this, SLOT(onPushButtonSlot()));//
	connect(ui.pushButton3, SIGNAL(clicked()), this, SLOT(onPushButtonSlot()));//

	connect(ui.horizontalSlider,   SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSliderP_valueChanged(int)));//����value  �źž��޷���Ӧ
	connect(ui.horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSliderI_valueChanged(int)));//
	connect(ui.horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSliderD_valueChanged(int)));//
	//ui.horizontalSlider->setRange(0, 1000);//���÷�Χ
	//ui.horizontalSlider->setValue(37); //���õ�ǰֵ
	//connect(&ui.doubleSpinBox, &QDoubleSpinBox::valueChanged,this,
	//	[=](int value)
	//	{
	//		qDebug() << "Value : " << value;
	//		qDebug() << "Text : " << ui.doubleSpinBox->text();
	//	});
	
	connect(ui.doubleSpinBox, static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged), [=](const QString &text)
		{
			if (ui.doubleSpinBox->underMouse())		//����ڸÿؼ��� ����Ӧ�ۺ���
			{
				ui.horizontalSlider->setValue(((int)(text.toFloat() * 1000)) % 1000);
				PID_GetDrawPoints();
			}
		});
	connect(ui.doubleSpinBox_2, static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged), [=](const QString &text)
		{
			if (ui.doubleSpinBox_2->underMouse())		//����ڸÿؼ��� ����Ӧ�ۺ���
			{
				ui.horizontalSlider_2->setValue(((int)(text.toFloat() * 1000)) % 1000);
				PID_GetDrawPoints();
			}
		});
	connect(ui.doubleSpinBox_3, static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged), [=](const QString &text)
		{
			if (ui.doubleSpinBox_3->underMouse())		//����ڸÿؼ��� ����Ӧ�ۺ���
			{
				ui.horizontalSlider_3->setValue(((int)(text.toFloat() * 1000)) % 1000);
				PID_GetDrawPoints();
			}
		});
	connect(ui.doubleSpinBox_4, static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged), [=](const QString &text)//�ı�Ŀ��ֵ
		{
			if (ui.doubleSpinBox_4->underMouse())		//����ڸÿؼ��� ����Ӧ�ۺ���
			{
				PID_GetDrawPoints();
			}
		});
	connect(ui.doubleSpinBox_5, static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged), [=](const QString &text)//�ı�Ŀ��ֵ
		{
			if (ui.doubleSpinBox_5->underMouse())		//����ڸÿؼ��� ����Ӧ�ۺ���
			{
				PID_GetDrawPoints();
			}
		});
	connect(ui.doubleSpinBox_6, static_cast<void(QDoubleSpinBox::*)(const QString &)>(&QDoubleSpinBox::valueChanged), [=](const QString &text)//�ı为��ֵ
		{
			if (ui.doubleSpinBox_6->underMouse())		//����ڸÿؼ��� ����Ӧ�ۺ���
			{
				PID_GetDrawPoints();
			}
		});
	
	connect(ui.customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
	connect(ui.customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
	connect(ui.customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));//ͼ��������ѡ���źŲ�
	connect(ui.customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

	ui.tableWidget->setColumnCount(500);//��������
	
	connect(&ReplotGraphTimer, SIGNAL(timeout()), this, SLOT(ReplotGraphSlot()));
	connect(&refreshAxisTimer, SIGNAL(timeout()), this, SLOT(RefreshAxisSlot()));
	refreshAxisTimer.start(0);
	pTitleBar = new TitleBar(this, 2);
	installEventFilter(pTitleBar);          //������¼��������  �����к����صĺ��


	//������������
	GraphSelectedByCheckBox.append(0);		//Ĭ����ʾ1�Ų���
	CheckBoxSet << ui.checkBox01 << ui.checkBox02 << ui.checkBox03 << ui.checkBox04
				<< ui.checkBox05 << ui.checkBox06 << ui.checkBox07 << ui.checkBox08;
	QSignalMapper* mapperData = new QSignalMapper(this);
	for (int i = 0; i < CheckBoxSet.size(); i++) {
		mapperData->setMapping(CheckBoxSet.at(i), i);
		connect(CheckBoxSet.at(i), SIGNAL(clicked()), mapperData, SLOT(map()));


		CheckBoxSet.at(i)->installEventFilter(this);
		CheckBoxSet.at(i)->setFont(QFont("Microsoft YaHei", 9, QFont::Normal));

		FlatUI::Instance()->setCheckBoxQss(CheckBoxSet.at(i), "white", 1);
	}
	CheckBoxSet.at(GraphSelectedByCheckBox.at(0))->setChecked(true);
	FlatUI::Instance()->setCheckBoxQss(CheckBoxSet.at(GraphSelectedByCheckBox.at(0)), "#00FFFF");
	CheckBoxSet.at(GraphSelectedByCheckBox.at(0))->setFont(QFont("Microsoft YaHei", 9, QFont::Light));
	connect(mapperData, SIGNAL(mapped(int)), this, SLOT(onCheckBoxSet(int)));
	setViewInterface(ui.customPlot);
	ui.customPlot->setBackground(QBrush(QColor(35, 35, 35)));
	ui.customPlot->replot();
	ReplotGraphTimer.start(0); // ���0��ζ�ž����ܿ��ˢ��
		//�����߳�
	//dataCollection = new RecordCalculateThread(1);
	//dataCollection->start();
	//Sleep(200);
	//connect(this, SIGNAL(dataOperationSignal()), dataCollection->recordCalculateWork, SLOT(recordCalculate()));//��main�����ڲ����Զ�moc������Ҫ���þ�̬����QObject��

	PID_GetDrawPoints();
}
void PIDViews::mousePress()
{
	int nFor;
	nFor = GraphSelectedByCheckBox.size() ? GraphSelectedByCheckBox.size() : 1;
	for (int i = 0; i < nFor; i++) {
		if (curveRect[i]->outerRect().contains(QPoint(cursor().pos().x() - this->geometry().x(), cursor().pos().y() - this->geometry().y()))) {//curveRect[i]->outerRect().contains()
			selectRect = i;
		}
		if (timeAxisSelectFlage) {          //���ʱ���ᱻ��ѡ  ���᲻���ᱻ��ק
			//curveRect[i]->setRangeDrag(curveRect[i]->axis(QCPAxis::atBottom)->orientation());

			if (selectRect > nFor || selectRect < 0) {      //���жϵ������� �Ƿ������� �򲻿��Խ������Ź���
				curveRect[i]->setRangeDrag(0);//
			}
			else {
				curveRect[i]->setRangeDrag(curveRect[i]->axis(QCPAxis::atBottom)->orientation());
			}
		}
		else {
			//curveRect[i]->setRangeDrag(curveRect[i]->axis(QCPAxis::atLeft)  ->orientation());
			curveRect[i]->setRangeDrag(Qt::Horizontal | Qt::Vertical);//����֧��������ק
		}
		//curveRect[i]->setRangeDrag(Qt::Horizontal|Qt::Vertical);//����֧��������ק
		//if(analysisTypeFlage)
		//analysisRect[i]->setRangeDrag(Qt::Horizontal|Qt::Vertical);
	}
}
void PIDViews::mouseWheel()
{

	QSharedPointer<QCPAxisTickerFixed> yTickerFixed[6];//��ô��ֵ Ҳ��û˭��
	int nFor;
	nFor = GraphSelectedByCheckBox.size() ? GraphSelectedByCheckBox.size() : 1;
	for (int i = 0; i < nFor; i++) {
		curveRect[i]->setRangeZoom(Qt::Vertical | Qt::Horizontal);//
		//timeTickerFixed->setTickStep((int)(curveRect[i]->axis(QCPAxis::atBottom)->range().size()/6));
		//curveRect[i]->axis(QCPAxis::atBottom)->setTicker(timeTickerFixed);
		if (timeAxisSelectFlage) {          //���ʱ���ᱻ��ѡ  ���᲻���ᱻ����
			if (curveRect[i]->outerRect().contains(QPoint(cursor().pos().x() - this->geometry().x(), cursor().pos().y() - this->geometry().y()))) {//curveRect[i]->outerRect().contains()
				selectRect = i;
			}
			if (selectRect > nFor || selectRect < 0) {      //���жϵ������� �Ƿ������� �򲻿��Խ������Ź���
				curveRect[i]->setRangeZoom(0);//
			}
			else {
				curveRect[i]->setRangeZoom(curveRect[i]->axis(QCPAxis::atBottom)->orientation());
			}
		}
		else {
			curveRect[i]->setRangeZoom(curveRect[i]->axis(QCPAxis::atLeft)->orientation());
		}

		yTickerFixed[i] = QSharedPointer<QCPAxisTickerFixed>(new QCPAxisTickerFixed);//��ô��ֵ Ҳ��û˭��
		yTickerFixed[i]->setTickStep(QString::number(curveRect[i]->axis(QCPAxis::atLeft)->range().size() / 10, 'f', 1).toFloat());
		curveRect[i]->axis(QCPAxis::atLeft)->setTicker(yTickerFixed[i]);
	}
}
void PIDViews::setViewInterface(QCustomPlot* customPlot)
{
	customPlot->plotLayout()->clear();// clear default axis rect so we can start from scratch
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);//| QCP::iSelectPlottables���Խ��ͼ��ѡ����|��ͼ���Ƿ��ѡ  QCP::iSelectAxes |���Ƿ����ѡ��  QCP::iRangeDrag|QCP::iRangeZoom |
	QList<QString> dataName;
	dataName << "G_IArms" << "G_IBrms" << "G_ICrms" << "G_GIS" << "G_I2" << "G_UABrms" << "G_UBCrms" << "G_UCArms";

	QSharedPointer<QCPAxisTickerFixed> xTickerFixed(new QCPAxisTickerFixed);
	QSharedPointer<QCPAxisTickerFixed> xTickerFixed2(new QCPAxisTickerFixed);//���߲��ֺ��� 0-10S����  Ƶ�ײ��ֺ���̶� 0-6Hz

	QSharedPointer<QCPAxisTickerFixed> y1TickerFixed(new QCPAxisTickerFixed);//���߲�������̶�
	xTickerFixed->setTickStep(10);
	xTickerFixed2->setTickStep(10);
	y1TickerFixed->setTickStep(20);
	QCPAxisRect *CurveAxisRect[4];
	QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
	int nFor;
	nFor = GraphSelectedByCheckBox.size() ? GraphSelectedByCheckBox.size() : 1;
	for (int i = 0; i < nFor; i++) {
		CurveAxisRect[i] = new QCPAxisRect(customPlot);
		CurveAxisRect[i]->setupFullAxesBox(true);
		customPlot->plotLayout()->addElement(i, 0, CurveAxisRect[i]);

		if (i < nFor - 1) {
			customPlot->plotLayout()->setRowStretchFactor(i, (100 / nFor) - 1.1);  //ColumnStretchFactor();
		}
		else {
			customPlot->plotLayout()->setRowStretchFactor(i, (100 / nFor) + 1.1*(nFor - 1));
		}

		// add two axis rects in the sub layout next to each other:
		//subLayout->setColumnStretchFactor(0, 3); // left axis rect shall have 60% of width
		//subLayout->setColumnStretchFactor(1, 2); // right one only 40% (3:2 = 60:40)
		CurveAxisRect[i]->axis(QCPAxis::atBottom)->setTicker(xTickerFixed);
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setTicker(y1TickerFixed);
		CurveAxisRect[i]->axis(QCPAxis::atBottom)->setRange(0, 100.00);
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setRange(0, 300);                //����Y�᷶Χ

		CurveAxisRect[i]->axis(QCPAxis::atBottom)->setBasePen(QPen(Qt::white, 1));    //�������߻���
		CurveAxisRect[i]->axis(QCPAxis::atBottom)->setSelectedBasePen(QPen(Qt::white, 1));
		CurveAxisRect[i]->axis(QCPAxis::atRight)->setBasePen(Qt::NoPen);//QPen(Qt::white,1)
		CurveAxisRect[i]->axis(QCPAxis::atTop)->setBasePen(Qt::NoPen);
		CurveAxisRect[i]->axis(QCPAxis::atBottom)->setTickLabels(false);
		CurveAxisRect[i]->axis(QCPAxis::atBottom)->setTicks(false);
		CurveAxisRect[i]->axis(QCPAxis::atRight)->setTickLabels(false);
		CurveAxisRect[i]->axis(QCPAxis::atRight)->setTicks(false);
		CurveAxisRect[i]->axis(QCPAxis::atTop)->setTickLabels(false);
		CurveAxisRect[i]->axis(QCPAxis::atTop)->setTicks(false);
		CurveAxisRect[i]->axis(QCPAxis::atBottom)->setPadding(14);                   //ʱ����������߿�ľ���
		//CurveAxisRect[i]->axis(QCPAxis::atBottom)->grid()->setPen(QPen(Qt::NoPen));//grid ���񡢸��� �������񻭱�
		//CurveAxisRect[i]->axis(QCPAxis::atLeft)->grid()->setPen(QPen(Qt::NoPen));//grid ���񡢸��� �������񻭱�
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setUpperEnding(QCPLineEnding::esSpikeArrow);//void setUpperEnding();�������̶ȶ���ʽ ��ͷ
		if (GraphSelectedByCheckBox.size() == 0) {
			CurveAxisRect[i]->axis(QCPAxis::atLeft)->setLabel("Nothing");               //����y������
		}
		else {
			CurveAxisRect[i]->axis(QCPAxis::atLeft)->setLabel(dataName.at(GraphSelectedByCheckBox.at(i)));
		}
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setLabelColor(Qt::yellow);
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setSelectedLabelColor(Qt::yellow);
		//customPlot->xAxis->setLabelPadding(0);                 //����Label��x�����
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setLabelPadding(10);
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setTickLabelColor(Qt::white);
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setSelectedTickLabelColor(Qt::yellow);
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setTickLabelPadding(10);
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setPadding(10);
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setBasePen(QPen(Qt::white, 2));
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setSelectedBasePen(QPen(Qt::yellow, 2));
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setTickPen(QPen(Qt::white, 1));
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setSelectedTickPen(QPen(Qt::yellow, 1));
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setSubTickPen(QPen(Qt::white, 1));
		CurveAxisRect[i]->axis(QCPAxis::atLeft)->setSelectedSubTickPen(QPen(Qt::yellow, 1));

		CurveAxisRect[i]->axis(QCPAxis::atLeft)->grid()->setZeroLinePen(Qt::NoPen);
		CurveAxisRect[i]->axis(QCPAxis::atBottom)->grid()->setZeroLinePen(Qt::NoPen);
		for (int n = 0; n < 1; n++) {
			customPlot->addGraph(CurveAxisRect[i]->axis(QCPAxis::atBottom), CurveAxisRect[i]->axis(QCPAxis::atLeft));
			//actionList.append(new QAction(tr("#%1��").arg(a+1),this));
			CurveAxisRect[i]->graphs().at(n)->setAntialiased(true);//�رշ�����
			CurveAxisRect[i]->graphs().at(n)->setPen(QPen(QColor(255, 255, 128), CURVEWIDE));
			//CurveAxisRect[i]->graphs().at(n)->setLineStyle(QCPGraph::lsNone);
			//CurveAxisRect[i]->graphs().at(n)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot, 2));
		}


		if (i == nFor - 1) {
			onlyCurveXAxis = CurveAxisRect[i]->addAxis(QCPAxis::atBottom);
			onlyCurveXAxis->setRange(0, 100.00);
			onlyCurveXAxis->setTicker(xTickerFixed2);
			onlyCurveXAxis->setLabel("Discrete Data" );
			onlyCurveXAxis->setLabelColor(Qt::white);
			onlyCurveXAxis->setSelectedLabelColor(Qt::white);
			onlyCurveXAxis->setTickLabelColor(Qt::white);
			onlyCurveXAxis->setSelectedTickLabelColor(Qt::yellow);
			onlyCurveXAxis->setSelectedBasePen(QPen(Qt::yellow, 2));
			onlyCurveXAxis->setSelectedTickPen(QPen(Qt::yellow, 2));
			onlyCurveXAxis->setSelectedSubTickPen(QPen(Qt::yellow, 2));
			onlyCurveXAxis->setBasePen(QPen(Qt::white, 2));
			onlyCurveXAxis->setTickPen(QPen(Qt::white, 2));
			onlyCurveXAxis->setSubTickPen(QPen(Qt::white, 2));
		}
		CurveAxisRect[i]->setBackground(QBrush(QColor(0, 0, 0)));
	}
	for (int i = 0; i < nFor; i++) {
		customPlot->axisRect(i)->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);//ͬ������������
		curveRect[i] = CurveAxisRect[i];
	}
}
void PIDViews::selectionChanged()                     //��ѡ���߻�ͼ��
{

	int nFor;
	nFor = GraphSelectedByCheckBox.size() ? GraphSelectedByCheckBox.size() : 1;
	if (onlyCurveXAxis->selectedParts().testFlag(QCPAxis::spAxis) || onlyCurveXAxis->selectedParts().testFlag(QCPAxis::spTickLabels)) {//ʹ�̶�������ͬʱ��ѡ
		onlyCurveXAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
		timeAxisSelectFlage = true;
	}
	else {
		timeAxisSelectFlage = false;
	}
	for (int i = 0; i < nFor; i++) {
		//if (curveRect[i]->axis(QCPAxis::atBottom)->selectedParts().testFlag(QCPAxis::spAxis) || curveRect[i]->axis(QCPAxis::atBottom)->selectedParts().testFlag(QCPAxis::spTickLabels))//ʹ�̶�������ͬʱ��ѡ
		//{
		//  curveRect[i]->axis(QCPAxis::atBottom)->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
		//}
		if (curveRect[i]->axis(QCPAxis::atLeft)->selectedParts().testFlag(QCPAxis::spAxis) || curveRect[i]->axis(QCPAxis::atLeft)->selectedParts().testFlag(QCPAxis::spTickLabels))
		{
			curveRect[i]->axis(QCPAxis::atLeft)->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
		}
	}
}
void PIDViews::contextMenuRequest(QPoint pos)         //����Ҽ�--ͼ��λ
{
	Q_UNUSED(pos);
	int nFor = GraphSelectedByCheckBox.size() ? GraphSelectedByCheckBox.size() : 1;
	QSharedPointer<QCPAxisTickerFixed> timeTickerFixed(new QCPAxisTickerFixed);
	timeTickerFixed->setTickStep(100 / 10);
	onlyCurveXAxis->setRange(0, 100);
	onlyCurveXAxis->setTicker(timeTickerFixed);
	for (int i = 0; i < nFor; i++) {
		curveRect[i]->axis(QCPAxis::atBottom)->setRange(0, 100);
		curveRect[i]->axis(QCPAxis::atBottom)->setTicker(timeTickerFixed);
	}

}
void PIDViews::PIDViewsGraph()
{
	for (int i = 0;i < GraphSelectedByCheckBox.size();i++)
	{
		if (i == 0)
		{
			curveRect[i]->graphs().at(0)->setData(HorizontalAxisValue, ActualValue);
		}

		if (i == 1)
		{
			curveRect[i]->graphs().at(0)->setData(HorizontalAxisValue, DifferenceValue);
		}
		
	}
	//
	//emit dataOperationSignal();             //���ݻ�����ɷ����źŸ� �����߳� ��������
}
void PIDViews::ReplotGraphSlot()                           //��ʱ��ͼ��ˢ��
{
	PIDViewsGraph();
	ui.customPlot->replot();
}
void PIDViews::RefreshAxisSlot()                      //��Ϊ����һ�������ԭ�� ����Ҫʵʱ������������Ϣ
{

	int nFor = GraphSelectedByCheckBox.size() ? GraphSelectedByCheckBox.size() : 1;
	if (selectRect > nFor - 1)selectRect = nFor - 1;   //�����ѡ�еĻ��鱻��ѡ���� ��ʱselectRect��Ҫ���¸���
	QSharedPointer<QCPAxisTickerFixed> timeTickerFixed(new QCPAxisTickerFixed);
	if (selectRect > nFor || selectRect < 0) {
		timeTickerFixed->setTickStep((int)100 / 10);
		onlyCurveXAxis->setTicker(timeTickerFixed);
	}
	else {
		timeTickerFixed->setTickStep(QString::number(curveRect[selectRect]->axis(QCPAxis::atBottom)->range().size() / 10, 'f', 2).toFloat());
		onlyCurveXAxis->setTicker(timeTickerFixed);
	}
	for (int i = 0; i < nFor; i++) {
		//if(timeAxisSelectFlage){
		curveRect[i]->axis(QCPAxis::atBottom)->setTicker(timeTickerFixed);
		if (selectRect > nFor || selectRect < 0) {}
		else {
			curveRect[i]->axis(QCPAxis::atBottom)->setRange(curveRect[selectRect]->axis(QCPAxis::atBottom)->range());                                                             //curveRect[selectRect]->axis(QCPAxis::atBottom)->range().upper
			onlyCurveXAxis->setRange(curveRect[selectRect]->axis(QCPAxis::atBottom)->range());
		}
		//}
	}
}
void PIDViews::onPushButtonSlot()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button == ui.pushButton1)
	{
		ui.doubleSpinBox  ->setValue(0.2);	 ui.horizontalSlider  ->setValue(200);		//KpĬ��Ϊ0.2
		ui.doubleSpinBox_2->setValue(0.015); ui.horizontalSlider_2->setValue(15);		//KiĬ��Ϊ0.015
		ui.doubleSpinBox_3->setValue(0.2);	 ui.horizontalSlider_3->setValue(200);		//KdĬ��Ϊ0.2
		
		ui.doubleSpinBox_4->setValue(200);		//Ŀ��ֵĬ��Ϊ200
		ui.doubleSpinBox_5->setValue(0);		//��ʼֵĬ��Ϊ0
		ui.doubleSpinBox_6->setValue(0);		//����ֵĬ��Ϊ0

		ui.lineEdit_4->setText("1");			//KpĬ��ϵ��1
		ui.lineEdit_5->setText("1");			//KiĬ��ϵ��1
		ui.lineEdit_6->setText("1");			//KdĬ��ϵ��1

		ui.lineEdit_2->setText("200");			//Ĭ�ϸ���200������
		ui.lineEdit_3->setText("500");			//Ĭ��ģ��500������
		PID_GetDrawPoints();
	}else if (button == ui.pushButton2) {    
		PID_GetDrawPoints();
	}else if (button == ui.pushButton3)
	{
		//pNoteDialog = new NoteDialog;
		//pNoteDialog->exec();
		pNoteDialog = new NoteDialog(this);
		pNoteDialog->move(QPoint(this->width() / 2 - pNoteDialog->width() / 2, this->height() / 2 - pNoteDialog->height() / 2));
		pNoteDialog->exec();
	}
}

void PIDViews::on_horizontalSliderP_valueChanged(int value)
{
	if (!ui.horizontalSlider->underMouse())return;
	int IntegerPart = (int)ui.doubleSpinBox->text().toFloat();
	if ((value / 1000.0)<=1.0)
	{
		ui.doubleSpinBox->setValue((double)(IntegerPart +(float)value / 1000.0));
		PID_GetDrawPoints();
	}

}
void PIDViews::on_horizontalSliderI_valueChanged(int value)
{
	if (!ui.horizontalSlider_2->underMouse())return;
	int IntegerPart = (int)ui.doubleSpinBox_2->text().toFloat();
	if ((value / 1000.0) <= 1.0)
	{
		ui.doubleSpinBox_2->setValue((double)(IntegerPart + (float)value / 1000.0));
		PID_GetDrawPoints();
	}
}
void PIDViews::on_horizontalSliderD_valueChanged(int value)
{
	if (!ui.horizontalSlider_3->underMouse())return;
	int IntegerPart = (int)ui.doubleSpinBox_3->text().toFloat();
	if ((value / 1000.0) <= 1.0)
	{
		ui.doubleSpinBox_3->setValue((double)(IntegerPart + (float)value / 1000.0));
		PID_GetDrawPoints();
	}
}
void PIDViews::onCheckBoxSet(int i)                     //���CheckBox ���������б� �����źŶ�Ӧ�Ĳ�
{
	if (CheckBoxSet.at(i)->isChecked()) {
		if (GraphSelectedByCheckBox.size() >= 4) {		//������ͬʱ�۲�4�ֲ���
			GraphSelectedByCheckBox.removeFirst();
		}
		GraphSelectedByCheckBox.append(i);
	}
	else 
	{
		for (int n = 0; n < GraphSelectedByCheckBox.size(); n++) {
			if (GraphSelectedByCheckBox.at(n) == i)
				GraphSelectedByCheckBox.removeAt(n);
		}
	}
	for (int n = 0; n < CheckBoxSet.size(); n++) {
		CheckBoxSet.at(n)->setChecked(false);
		FlatUI::Instance()->setCheckBoxQss(CheckBoxSet.at(n), "white");
		CheckBoxSet.at(n)->setFont(QFont("Microsoft YaHei", 9, QFont::Normal));
	}
	for (int n = 0; n < GraphSelectedByCheckBox.size(); n++) {
		CheckBoxSet.at(GraphSelectedByCheckBox.at(n))->setChecked(true);
		FlatUI::Instance()->setCheckBoxQss(CheckBoxSet.at(GraphSelectedByCheckBox.at(n)), "#00FFFF");
		CheckBoxSet.at(GraphSelectedByCheckBox.at(n))->setFont(QFont("Microsoft YaHei", 9, QFont::Light));
	}

	ui.customPlot->clearGraphs();
	ui.customPlot->clearFocus();//����
	ui.customPlot->clearItems();
	ui.customPlot->clearMask();
	ui.customPlot->clearPlottables();
	ui.customPlot->plotLayout()->clear();
	setViewInterface(ui.customPlot);
	ui.customPlot->replot();

}


//pid����
void PIDViews::PID_GetDrawPoints()
{
	int PointsCount = ui.lineEdit_3->text().toInt();	//ģ����ܵ���
	HorizontalAxisValue.resize(PointsCount);
	ActualValue.resize(PointsCount);
	DifferenceValue.resize(PointsCount);
	MyPid pid;
	PID_init(pid);
	
	ui.tableWidget->clearContents();//��ձ�����е�Ԫ�����ݣ���������������Ϣ��
	ui.tableWidget->setColumnCount(PointsCount);//��������
	for (int i = 0; i < PointsCount; i++)
	{
		if (i < ui.lineEdit_2->text().toInt())
		{
			pid.LoadValue = ui.doubleSpinBox_6->text().toFloat();
		}
		else 
		{
			pid.LoadValue = 0.0;
		}
		HorizontalAxisValue.replace(i, (double)(i * (100.0 / PointsCount)));

		//int count = 0;
		//while (count < POINTS_COUNT)
		//{
		ActualValue.replace(i, PID_realize(pid));
		DifferenceValue.replace(i, pid.OutSpeed);//ui.doubleSpinBox_4->value() - ActualValue.at(i)��ʾĿ��ֵ��ʵ��ֵ�Ĳ�ֵ


		ui.tableWidget->setItem(0, i, new QTableWidgetItem(QString::number(pid.err, 'f', 3)));
		ui.tableWidget->setItem(1, i, new QTableWidgetItem(QString::number(pid.OutSpeed, 'f', 3)));
		ui.tableWidget->setItem(2, i, new QTableWidgetItem(QString::number(pid.ActualHeight, 'f', 3)));
		ui.tableWidget->setItem(3, i, new QTableWidgetItem(QString::number(pid.Kp*pid.err, 'f', 3)));
		ui.tableWidget->setItem(4, i, new QTableWidgetItem(QString::number(pid.Ki*pid.integral, 'f', 3)));
		ui.tableWidget->setItem(5, i, new QTableWidgetItem(QString::number(pid.OutSpeed + pid.LoadValue - pid.Kp*pid.err - pid.Ki*pid.integral, 'f', 3)));
	}
}
//�ڶ�������ʼ���������������£�
void PIDViews::PID_init(MyPid& VirtualPid)
{
	printf("PID_init begin \n");
	//VirtualPid.SetTargetHeight = 0.0;
	VirtualPid.SetTargetHeight = ui.doubleSpinBox_4->value();
	VirtualPid.ActualHeight = 0.0 + ui.doubleSpinBox_5->value();

	VirtualPid.err = 0.0;
	VirtualPid.err_last = 0.0;

	VirtualPid.integral = 0.0;
	VirtualPid.Kp = ui.doubleSpinBox  ->text().toFloat()*ui.lineEdit_4->text().toFloat();
	VirtualPid.Ki = ui.doubleSpinBox_2->text().toFloat()*ui.lineEdit_5->text().toFloat();
	VirtualPid.Kd = ui.doubleSpinBox_3->text().toFloat()*ui.lineEdit_6->text().toFloat();

	VirtualPid.OutSpeed = 0.0;
	VirtualPid.LastOutSpeed = 0.0;

	VirtualPid.LoadValue = ui.doubleSpinBox_6->text().toFloat();

	
	printf("PID_init end \n");
}
//ͳһ��ʼ��������������Kp, Ki, Kd�������������Թ��̵��У�����Ҫ��Ŀ���Ч��������ͨ��������������ֱ�ӽ��е��ڡ�
//����������д�����㷨��PIDʵ�֡��������£�
float PIDViews::PID_realize(MyPid& VirtualPid)
{
	//VirtualPid.SetTargetHeight = ui.doubleSpinBox_4->value();
	VirtualPid.err = VirtualPid.SetTargetHeight - VirtualPid.ActualHeight;
	VirtualPid.integral += VirtualPid.err;
	VirtualPid.LastOutSpeed = VirtualPid.OutSpeed;
	VirtualPid.OutSpeed = VirtualPid.Kp*VirtualPid.err + VirtualPid.Ki*VirtualPid.integral + VirtualPid.Kd*(VirtualPid.err - VirtualPid.err_last) - VirtualPid.LoadValue;

	VirtualPid.err_last = VirtualPid.err;

	VirtualPid.ActualHeight += ((VirtualPid.LastOutSpeed + VirtualPid.OutSpeed) / 2)*1.0;	//ƽ���ٶ�*ʱ��=�ɻ����о���  ���ֺ���ǵ�ǰ��ʵ�ʸ߶�
	//printf("%f\t%f\t%f\n",VirtualPid.err, VirtualPid.OutSpeed, VirtualPid.ActualHeight);
	return VirtualPid.ActualHeight;
}