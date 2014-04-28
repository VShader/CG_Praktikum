#include "controlWidget.hpp"
#include "ui_controlWidget.h"

ControlWidget::ControlWidget() :
    ui(new Ui::ControlWidget())
{
    ui->setupUi(this);

//    connect(ui->horizontalSlider_Speed, &QSlider::valueChanged,
//            ui->spinBox_Speed, &QSpinBox::setValue);
//    connect(ui->spinBox_Speed, &QSpinBox::valueChanged,
//            ui->horizontalSlider_Speed, &QSlider::setValue);
}

ControlWidget::~ControlWidget()
{
    delete ui;
}
