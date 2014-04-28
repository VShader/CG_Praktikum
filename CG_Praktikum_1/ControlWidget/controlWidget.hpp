#ifndef CONTROLWIDGET_HPP
#define CONTROLWIDGET_HPP

#include <QWidget>

namespace Ui {
    class ControlWidget;
}

class ControlWidget : public QWidget
{
public:
    ControlWidget();
    ~ControlWidget();



private:
    Ui::ControlWidget *ui;
};

#endif  // CONTROLWIDGET_HPP
