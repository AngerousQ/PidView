﻿#include "../stdafx.h"
#include "flatui.h"


FlatUI *FlatUI::self = 0;
FlatUI::FlatUI(QObject *parent) : QObject(parent)
{

}

void FlatUI::setPushButtonQss(QPushButton *btn,
                              QString normalColor, QString normalTextColor,
                              QString hoverColor, QString hoverTextColor,
                              QString pressedColor, QString pressedTextColor)
{
    QStringList qss;
    qss.append(QString("QPushButton{border-style:none;padding:8px;border-radius:5px;color:%1;background:%2;}").arg(normalTextColor).arg(normalColor));
    qss.append(QString("QPushButton:hover{color:%1;background:%2;}").arg(hoverTextColor).arg(hoverColor));
    qss.append(QString("QPushButton:pressed{color:%1;background:%2;}").arg(pressedTextColor).arg(pressedColor));
    btn->setStyleSheet(qss.join(""));//多个字符串由“”内的符号拼接。
}

void FlatUI::setLineEditQss(QLineEdit *txt, QString normalColor, QString focusColor)
{
    QStringList qss;
    qss.append(QString("QLineEdit{border-style:none;padding:3px;border-radius:5px;border:2px solid %1;}").arg(normalColor));
    qss.append(QString("QLineEdit:focus{border:2px solid %1;}").arg(focusColor));
    txt->setStyleSheet(qss.join(""));
}

void FlatUI::setProgressBarQss(QProgressBar *bar, QString normalColor, QString chunkColor)
{
    int barHeight = 8;
    int barRadius = 8;

    QStringList qss;
    qss.append(QString("QProgressBar{font:9pt;max-height:%2px;background:%1;border-radius:%3px;text-align:center;border:1px solid %1;}").arg(normalColor).arg(barHeight).arg(barRadius));
    qss.append(QString("QProgressBar:chunk{border-radius:%2px;background-color:%1;}").arg(chunkColor).arg(barRadius));
    bar->setStyleSheet(qss.join(""));
}

void FlatUI::setSliderQss(QSlider *slider, QString normalColor, QString grooveColor, QString handleColor)
{
    int sliderHeight = 8;
    int sliderRadius = sliderHeight / 2;
    int handleWidth = (sliderHeight * 3) / 2 + (sliderHeight / 5);
    int handleRadius = handleWidth / 2;
    int handleOffset = handleRadius / 2;

    QStringList qss;
    qss.append(QString("QSlider::groove:horizontal{height:%2px;border-radius:%3px;background:%1;}")
               .arg(normalColor).arg(sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::add-page:horizontal{height:%2px;border-radius:%3px;background:%1;}")
               .arg(normalColor).arg(sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::sub-page:horizontal{height:%2px;border-radius:%3px;background:%1;}")
               .arg(grooveColor).arg(sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::handle:horizontal{width:%2px;margin-top:-%3px;margin-bottom:-%3px;border-radius:%4px;"
                       "background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 #FFFFFF,stop:0.8 %1);}")
               .arg(handleColor).arg(handleWidth).arg(handleOffset).arg(handleRadius));

    //偏移一个像素
    handleWidth = handleWidth + 1;
    qss.append(QString("QSlider::groove:vertical{width:%2px;border-radius:%3px;background:%1;}")
               .arg(normalColor).arg(sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::add-page:vertical{width:%2px;border-radius:%3px;background:%1;}")
               .arg(normalColor).arg(sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::sub-page:vertical{width:%2px;border-radius:%3px;background:%1;}")
               .arg(grooveColor).arg(sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::handle:vertical{height:%2px;margin-left:-%3px;margin-right:-%3px;border-radius:%4px;"
                       "background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 #FFFFFF,stop:0.8 %1);}")
               .arg(handleColor).arg(handleWidth).arg(handleOffset).arg(handleRadius));

    slider->setStyleSheet(qss.join(""));
}

void FlatUI::setRadioButtonQss(QRadioButton *rbtn, QString normalColor, QString checkColor)
{
    int indicatorRadius = 8;
    int indicatorWidth = indicatorRadius * 2;

    QStringList qss;
    qss.append(QString("QRadioButton::indicator{border-radius:%1px;width:%2px;height:%2px;}").arg(indicatorRadius).arg(indicatorWidth));
    qss.append(QString("QRadioButton::indicator::unchecked{background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 #FFFFFF,stop:0.7 %1);}").arg(normalColor));
    qss.append(QString("QRadioButton::indicator::checked{background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0 %1,stop:0.3 %1,stop:0.4 #FFFFFF,stop:0.6 #FFFFFF,stop:0.7 %1);}").arg(checkColor));
    qss.append(QString("QRadioButton::indicator::disabled::unchecked{background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 #FFFFFF,stop:0.7 %1);}").arg("#D7DBDE"));//自定义
    qss.append(QString("QRadioButton::indicator::disabled::checked{background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0 %1,stop:0.3 %1,stop:0.4 #FFFFFF,stop:0.6 #FFFFFF,stop:0.7 %1);}").arg("#D7DBDE"));

    rbtn->setStyleSheet(qss.join(""));
}

void FlatUI::setCheckBoxQss(QCheckBox *ck,QString unitColor,int disEnableType)
{
    //(normalColor);
    //Q_UNUSED(checkColor);
    //int indicatorRadius = 3;
    //int indicatorWidth = indicatorRadius * 6;
    QString type;
    if(disEnableType){
        type = QString("QCheckBox::indicator:disEnabled {image: url(:/image/rsources/checkBoxChecked.png);}");
    }else{
        type = QString("QCheckBox::indicator:disEnabled {image: url(:/image/rsources/close11.png);}");
    }
    QStringList qss;
    qss.append(QString("QCheckBox{spacing: 5px;color:")+unitColor+"}");
    qss.append(QString("QCheckBox:hover{background:rgb(30,30,30);border-radius:3px}"));
    qss.append(QString("QCheckBox::indicator{width: 17px;height:17px}"));
    //qss.append(QString("QCheckBox::indicator{border-radius:%1px;width:%2px;height:%2px;}").arg(indicatorRadius).arg(indicatorWidth));
    //qss.append(QString("QCheckBox::indicator::unchecked{background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 #FFFFFF,stop:0.7 %1);}").arg(normalColor));
    //qss.append(QString("QCheckBox::indicator::checked{background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0 %1,stop:0.3 %1,stop:0.4 #FFFFFF,stop:0.6 #FFFFFF,stop:0.7 %1);}").arg(checkColor));
    qss.append(QString("QCheckBox::indicator:enabled:unchecked {image: url(:/image/rsources/checkBoxHover.png);}"));
    qss.append(QString("QCheckBox::indicator:enabled:unchecked:hover {image: url(:/image/rsources/checkBox.png);}"));
    qss.append(QString("QCheckBox::indicator:enabled:unchecked:pressed {image: url(:/image/rsources/checkBoxPressed.png);}"));
    qss.append(QString("QCheckBox::indicator:enabled:checked {image: url(:/image/rsources/checkBoxIndeterminate.png);}"));
    qss.append(QString("QCheckBox::indicator:enabled:checked:hover {image: url(:/image/rsources/checkBoxIndeterminateHover.png);}"));
    qss.append(QString("QCheckBox::indicator:enabled:checked:pressed {image: url(:/image/rsources/checkBoxIndeterminatePressed.png);}"));
    qss.append(type);    //复选框 第三种状态样式表
    ck->setStyleSheet(qss.join(""));
}

void FlatUI::setScrollBarQss(QScrollBar *scroll, QString bgColor, QString handleNormalColor, QString handleHoverColor, QString handlePressedColor)
{
    QStringList qss;
    qss.append(QString("QScrollBar:horizontal{background:rgba(0,0,0,0);padding:3px 3px 3px 3px;}"));
    qss.append(QString("QScrollBar::handle:horizontal{background:%1;min-width:120px;}").arg(handleNormalColor));
    qss.append(QString("QScrollBar::handle:horizontal:hover{background:%1;}").arg(handleHoverColor));
    qss.append(QString("QScrollBar::handle:horizontal:pressed{background:%1;}").arg(handlePressedColor));
    qss.append(QString("QScrollBar::add-page:horizontal{background:%1;}").arg(bgColor));
    qss.append(QString("QScrollBar::sub-page:horizontal{background:%1;}").arg(bgColor));
    qss.append(QString("QScrollBar::add-line:horizontal{background:none;}"));
    qss.append(QString("QScrollBar::sub-line:horizontal{background:none;}"));

    qss.append(QString("QScrollBar:vertical{background:rgba(0,0,0,0);padding:3px 3px 3px 3px;}"));
    qss.append(QString("QScrollBar::handle:vertical{background:%1;min-height:120px;}").arg(handleNormalColor));
    qss.append(QString("QScrollBar::handle:vertical:hover{background:%1;}").arg(handleHoverColor));
    qss.append(QString("QScrollBar::handle:vertical:pressed{background:%1;}").arg(handlePressedColor));
    qss.append(QString("QScrollBar::add-page:vertical{background:%1;}").arg(bgColor));
    qss.append(QString("QScrollBar::sub-page:vertical{background:%1;}").arg(bgColor));
    qss.append(QString("QScrollBar::add-line:vertical{background:none;}"));
    qss.append(QString("QScrollBar::sub-line:vertical{background:none;}"));

    scroll->setStyleSheet(qss.join(""));
}
