#-------------------------------------------------
#
# Project created by QtCreator 2017-07-26T12:33:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets  xml

QT += multimedia

TARGET = ballitem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ballitem.cpp \
    ballshape.cpp \
    boxitem.cpp \
    ballgrid.cpp \
    sceneinfotrans.cpp

HEADERS  += mainwindow.h \
    ballitem.h \
    ballshape.h \
    shape.h \
    boxitem.h \
    ballgrid.h \
    shapelink.h \
    shapeball.h \
    sceneinfotrans.h

RESOURCES += \
    sound.qrc
