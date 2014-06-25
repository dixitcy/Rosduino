#include "regui/regui_frame.h"

#include <QPointF>
#include <QObject>
#include <QtGui>

#include <ros/package.h>
#include <cstdlib>

namespace regui
{

GuiFrame::GuiFrame(QWidget* parent, Qt::WindowFlags f) : QFrame(parent,f)
{
    setMinimumSize(100,100);
    setFixedSize(500,500);
    setWindowTitle("ROS Basic GUI");
    slider = new QSlider(Qt::Horizontal);
    number1 = new QDoubleSpinBox;
    number2 = new QDoubleSpinBox;
    layout = new QVBoxLayout;
    addition = new QHBoxLayout;
    button = new QPushButton("&Sum them up!",this);
    answer = new QLineEdit;

    slider->setFocusPolicy(Qt::StrongFocus);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(10);
    slider->setSingleStep(0.1);
    button->setDefault(true);
    answer->setReadOnly(true);
    answer->setFixedWidth(50);

    layout->addWidget(slider);
    addition->addWidget(number1);
    addition->addWidget(number2);
    addition->addWidget(button);
    addition->addWidget(answer);

    mainlayout = new QGridLayout;
    mainlayout->addLayout(layout,0,0);
    mainlayout->addLayout(addition,1,0);
    setLayout(mainlayout);

    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(publishVal(int)));
    connect(button,SIGNAL(clicked()),this,SLOT(sendSumRequest()));
    //To put some objects etc

    slider_val_pub_ = nh_.advertise<SliderVal>("slider_value",1);
    addition_client_ = nh_.serviceClient<TwoInts>("add_two_ints");

}

GuiFrame::~GuiFrame()
{
    delete slider;
    delete layout;
    delete number1;
    delete number2;
    delete button;
    delete answer;
    delete addition;
    delete mainlayout;
}

void GuiFrame::publishVal(int value){
    SliderVal sliderval;
    sliderval.val = value;
    slider_val_pub_.publish(sliderval);
}

void GuiFrame::sendSumRequest(){
    srv.request.n1=static_cast<int>(number1->value());
    srv.request.n2=static_cast<int>(number2->value());
    ROS_INFO("Sending Request to add");
    if(addition_client_.call(srv)){
        ROS_INFO("Sum is %d",srv.response.sum);
        answer->setText(QString("%1").arg(srv.response.sum));
    }else{
        ROS_ERROR("Failed to call service add_two_ints");
    }
}

}
