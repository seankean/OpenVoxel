#-------------------------------------------------
#
# Project created by QtCreator 2011-05-15T19:40:59
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = GLSL_SimpleMandel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glrenderthread.cpp \
    glframe.cpp

HEADERS  += mainwindow.h \
    glrenderthread.h \
    glframe.h

FORMS    += mainwindow.ui

CONFIG += debug
