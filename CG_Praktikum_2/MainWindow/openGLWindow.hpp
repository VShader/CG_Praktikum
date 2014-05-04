#ifndef OPENGLWINDOW_HPP
#define OPENGLWINDOW_HPP

#include <QWindow>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <string>

class OpenGLWindow : public QWindow
{
public:
    OpenGLWindow();

    std::string loadShader(const std::string &source);

private:
    QOpenGLContext context;
    QOpenGLVertexArrayObject vao;

    void render();
};

#endif // OPENGLWINDOW_HPP
