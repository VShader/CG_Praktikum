#include <QApplication>

#include "mainWindow.hpp"
#include "openGLWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    MainWindow win;
//    win.show();

    OpenGLWindow gl;
    gl.show();

    return app.exec();
}
