#include "controlWidget.hpp"
#include "ui_controlWidget.h"
#include <QDebug>

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget())
{
    ui->setupUi(this);

//    connect(ui->horizontalSlider_Speed, &QSlider::valueChanged,
//            ui->spinBox_Speed, &QSpinBox::setValue);
//    connect(ui->spinBox_Speed, &QSpinBox::valueChanged,
//            ui->horizontalSlider_Speed, &QSlider::setValue);
    connect(ui->spinBox_Speed, SIGNAL(valueChanged(int)),
            this, SIGNAL(speedChanged(int)));
//    connect(ui->spinBox_X, &QSpinBox::valueChanged, this, &ControlWidget::setRotation);
    connect(ui->spinBox_X, SIGNAL(valueChanged(int)), this, SLOT(setRotation(int)));
    connect(ui->spinBox_Y, SIGNAL(valueChanged(int)), this, SLOT(setRotation(int)));
    connect(ui->spinBox_Z, SIGNAL(valueChanged(int)), this, SLOT(setRotation(int)));
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::closeEvent(QCloseEvent *event)
{
    //    closing();
}

void ControlWidget::setRotation(int)
{
    rotaionChanged(ui->spinBox_X->value(),
                   ui->spinBox_Y->value(),
                   ui->spinBox_Z->value());
}
