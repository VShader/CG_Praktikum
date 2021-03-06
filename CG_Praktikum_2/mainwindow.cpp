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

#include "sunsystem.hpp"

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
    glEnable(GL_DEPTH_TEST);
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat cube[] = {
        //front
        -0.25f, -0.25f, -0.25f,
        0.25f, -0.25f, -0.25f,
        -0.25f, 0.25f, -0.25f,
        -0.25f, 0.25f, -0.25f,
        0.25f, -0.25f, -0.25f,
        0.25f, 0.25f, -0.25f,
        //back
        -0.25f, -0.25f, 0.25f,
        0.25f, -0.25f, 0.25f,
        -0.25f, 0.25f, 0.25f,
        -0.25f, 0.25f, 0.25f,
        0.25f, -0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        //left
        -0.25f, -0.25f, -0.25f,
        -0.25f, -0.25f, 0.25f,
        -0.25f, 0.25f, -0.25f,
        -0.25f, 0.25f, -0.25f,
        -0.25f, -0.25f, 0.25f,
        -0.25f, 0.25f, 0.25f,
        //right
        0.25f, -0.25f, -0.25f,
        0.25f, -0.25f, 0.25f,
        0.25f, 0.25f, -0.25f,
        0.25f, 0.25f, -0.25f,
        0.25f, -0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        //top
        -0.25f, 0.25f, -0.25f,
        0.25f, 0.25f, -0.25f,
        -0.25f, 0.25f, 0.25f,
        -0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, -0.25f,
        //botom
        -0.25f, -0.25f, -0.25f,
        0.25f, -0.25f, -0.25f,
        -0.25f, -0.25f, 0.25f,
        -0.25f, -0.25f, 0.25f,
        0.25f, -0.25f, 0.25f,
        0.25f, -0.25f, -0.25f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    time_t now;
//    time(&now);
    difftime(time, now);
    double move = 1;
    if(speed != 0)
    {
        move = speed * m_frame / screen()->refreshRate();
    }



    QMatrix4x4 scale;
    QMatrix4x4 orbitRotMatrix;
    QMatrix4x4 orbitMatrix;
    QMatrix4x4 rotMatrix;
    scale.scale(5.0f);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet sonne("sonne", vertices, scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.5f);
    orbitRotMatrix.rotate(rotateY * move, 0, 1, 0);
    orbitMatrix.translate(3.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet erde("erde", vertices, scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.3f);
    orbitRotMatrix.rotate(rotateY * move, 0, 1, 0);
    orbitMatrix.translate(1.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet mond("mond", vertices, scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.5f);
    orbitRotMatrix.rotate(rotateY * move, 0, 1, 0);
    orbitMatrix.translate(7.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet mars("mars", vertices, scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.3f);
    orbitRotMatrix.rotate(rotateY * move, 0, 1, 0);
    orbitMatrix.translate(1.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet deimos("deimos", vertices, scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.15f);
    orbitRotMatrix.rotate(rotateY * move *15, 0, 1, 0);
    orbitMatrix.translate(1.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet phobos("phobos", vertices, scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    std::vector<cg::Planet*> vecPlanet {&sonne, &erde, &mond, &mars, &deimos, &phobos};
    cg::Sunsystem sys;
    sys.addChild(&sonne)->addChild(&erde)->addChild(&mond);
    sys.addChild(&mars)->addChild(&deimos);
    sys.getChild(1)->addChild(&phobos);
    sys.run();


    QMatrix4x4 camMatrix;
    camMatrix.perspective(60.f, 4.0f/3.0f, 0.1f, 100.0f);
//    camMatrix.rotate(25, 1, 0, 0);
    camMatrix.translate(0, 0, -10);





    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, cube);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    for(cg::Planet *n : vecPlanet)
    {
        m_program->setUniformValue(m_matrixUniform, camMatrix * n->resultmatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }


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
