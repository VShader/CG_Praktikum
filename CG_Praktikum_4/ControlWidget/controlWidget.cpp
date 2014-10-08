#include "controlWidget.hpp"
#include "ui_controlWidget.h"
#include <QDebug>
#include <QApplication>

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget())
{
    ui->setupUi(this);

    connect(ui->spinBox_Speed, SIGNAL(valueChanged(int)),
            this, SIGNAL(speedChanged(int)));
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::closeEvent(QCloseEvent *event)
{
    qApp->quit();
}
