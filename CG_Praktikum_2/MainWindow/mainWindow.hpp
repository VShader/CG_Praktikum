#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow();
    ~MainWindow();


private:
    Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_HPP
