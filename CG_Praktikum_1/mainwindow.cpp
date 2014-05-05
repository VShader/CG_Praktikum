/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "controlWidget.hpp"

#include <QtGui>
#include <ctime>

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

MainWindow::MainWindow()
    : m_program(0)
    , m_frame(0),
        control(new ControlWidget()),
        speed(1), rotateX(0), rotateY(0), rotateZ(0)
{
//    time(&time);
    control->show();

//    connect(control, &ControlWidget::closing, this, &MainWindow::close);
//    connect(this, &QWindow::close, control, &QWidget::close);
    connect(control, &ControlWidget::speedChanged, this, &MainWindow::setSpeed);
    connect(control, &ControlWidget::rotaionChanged, this, &MainWindow::setRotation);
}

MainWindow::~MainWindow()   {
    delete control;
}




GLuint MainWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}


void MainWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void MainWindow::render()
{
    glViewport(0, 0, width(), height());

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    QMatrix4x4 matrix2;
    matrix.perspective(60.f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix2.perspective(60.f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix2.translate(0, 0, -3);
    time_t now;
//    time(&now);
    difftime(time, now);
    matrix.rotate(rotateX * speed * m_frame / screen()->refreshRate(), 1, 0, 0);
    matrix.rotate(rotateY * speed * m_frame / screen()->refreshRate(), 0, 1, 0);
    matrix.rotate(rotateZ * speed * m_frame / screen()->refreshRate(), 0, 0, 1);
    matrix.translate(1.0f, 0, 0);
    matrix.rotate(rotateX * speed * m_frame / screen()->refreshRate(), 1, 0, 0);
    matrix.rotate(-rotateY * speed * m_frame / screen()->refreshRate(), 0, 1, 0);
    matrix.rotate(-rotateZ * speed * m_frame / screen()->refreshRate(), 0, 0, 1);
//    matrix.rotate(100.0f * rotateZ * speed / screen()->refreshRate(), 0, 0, 1);
    matrix2.rotate(rotateY * speed * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_program->setUniformValue(m_matrixUniform, matrix2);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
//    time(&time);
}

void MainWindow::setRotation(int x, int y, int z)
{
 rotateX = x;   rotateY = y;    rotateZ = z;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    control->close();
}
