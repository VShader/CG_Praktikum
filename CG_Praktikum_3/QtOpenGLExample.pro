QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenGLExample
TEMPLATE = app


SOURCES += main.cpp\
        openglwindow.cpp\
        mainwindow.cpp

HEADERS  += openglwindow.h\
		mainwindow.h
