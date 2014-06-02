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

#include "config.hpp"
#include "mainwindow.h"
#include "controlWidget.hpp"

#include <QtGui>
#include <ctime>

#include "sunsystem.hpp"
#include "fileLoader.hpp"
#include <iostream>
#include <QDebug>


MainWindow::MainWindow()
    : fullScreen(false), m_program(0), m_frame(0),
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
    delete myMesh;
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
    glEnable(GL_CULL_FACE);
    m_program = new QOpenGLShaderProgram(this);
    cg::FileLoader loader;
    std::string address = RESOURCES;
    std::string temp = loader.loadShader(address+"vertexShader.glsl");
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, temp.c_str());
    temp = loader.loadShader(address+"fragmentShader.glsl");
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, temp.c_str());
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

//    myMesh = new cg::Mesh(loader.loadObj(address+"sphere.obj"));
    myMesh = new cg::Mesh(loader.multi(address+"sphere.obj"));
    if(myMesh->vn.size() == 0)  loader.calcNormals(*myMesh);

    //Buffer
        GLuint positionBuffer;
        GLuint indexBuffer;
        glGenBuffers(1, &positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, myMesh->v.size()*sizeof(GLfloat), &myMesh->v[0], GL_STATIC_DRAW);
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myMesh->v_f.size()*sizeof(GLuint), &myMesh->v_f[0], GL_STATIC_DRAW);
}

void MainWindow::render()
{
    glViewport(0, 0, width(), height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();


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
    scale.scale(4.0f);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet sonne("sonne", &myMesh->v[0], scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.5f);
    orbitRotMatrix.rotate(rotateY * move, 0, 1, 0);
    orbitMatrix.translate(5.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet erde("erde", &myMesh->v[0], scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.3f);
    orbitRotMatrix.rotate(rotateY * move, 0, 1, 0);
    orbitMatrix.translate(1.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet mond("mond", &myMesh->v[0], scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.5f);
    orbitRotMatrix.rotate(0.5 * rotateY * move + 15, 0, 1, 0);
    orbitMatrix.translate(10.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet mars("mars", &myMesh->v[0], scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.3f);
    orbitRotMatrix.rotate(rotateY * move, 0, 1, 0);
    orbitMatrix.translate(1.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet deimos("deimos", &myMesh->v[0], scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    scale.setToIdentity();
    orbitRotMatrix.setToIdentity();
    orbitMatrix.setToIdentity();
    rotMatrix.setToIdentity();
    scale.scale(0.15f);
    orbitRotMatrix.rotate(rotateY * move *15, 0, 1, 0);
    orbitMatrix.translate(1.0f, 0, 0);
    rotMatrix.rotate(rotateY * move, 0, 1, 0);
    cg::Planet phobos("phobos", &myMesh->v[0], scale, rotMatrix, orbitMatrix, orbitRotMatrix);

    std::vector<cg::Planet*> vecPlanet {&sonne, &erde, &mond, &mars, &deimos, &phobos};
    cg::Sunsystem sys;
    sys.addChild(&sonne)->addChild(&erde)->addChild(&mond);
    sys.addChild(&mars)->addChild(&deimos);
    sys.getChild(1)->addChild(&phobos);
    sys.run();


    QMatrix4x4 camMatrix;
    camMatrix.perspective(60.f, 4.0f/3.0f, 0.1f, 100.0f);
    camMatrix.translate(0, 0, -15);
    camMatrix.rotate(45, 1, 0, 0);



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, &myMesh->v[0]);
//    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);


    for(cg::Planet *n : vecPlanet)
    {
        m_program->setUniformValue(m_matrixUniform, camMatrix * n->resultmatrix);
        //glDrawArrays(GL_TRIANGLES, 0, myMesh->v.size()/3);
        glDrawElements(GL_TRIANGLES, myMesh->v_f.size(), GL_UNSIGNED_INT, 0);
//        glDrawElements(GL_TRIANGLES, myMesh->v_f.size(), GL_UNSIGNED_SHORT, &myMesh->v_f[0]);
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



void MainWindow::keyPressEvent(QKeyEvent &event)
{
    qDebug() <<"Hi";
    if((event.key()==Qt::Key_Return) && (event.modifiers()==Qt::AltModifier))
    {
        fullScreen = !fullScreen;
        toggleFullScreen(fullScreen);
    }

}

void MainWindow::toggleFullScreen(bool fullScreen)
{
    if(fullScreen) showFullScreen();
    else show();
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    control->close();
}
