#include <fstream>
#include <QDebug>
#include <QOpenGLFunctions>

#include "openGLWindow.hpp"
//#include "GL/glew"


OpenGLWindow::OpenGLWindow()
{
    setSurfaceType(OpenGLSurface);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    setFormat(format);
    create();

    context.setFormat(format);
    context.create();

    context.makeCurrent(this);

    qDebug() << loadShader("testShader.glsl").c_str();

    vao.create();
    vao.bind();



    while(true)
    {
        render();
        context.swapBuffers(this);
    }
}


std::string OpenGLWindow::loadShader(const std::string &source)
{
    std::ifstream ifs(source, std::ifstream::in);

    return std::string((std::istreambuf_iterator<char>(ifs)),
                   std::istreambuf_iterator<char>());
}



void OpenGLWindow::render()
{
    static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
//    glDrawArrays();
//    glClearBufferfv();

}
