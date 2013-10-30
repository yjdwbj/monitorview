#-------------------------------------------------
#
# Project created by QtCreator 2013-10-28T10:51:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebCameraClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    setting_panel.cpp \
    viewframe.cpp \
    addnewcamera.cpp \
    searchcamera.cpp

HEADERS  += mainwindow.h \
    setting_panel.h \
    global.h \
    viewframe.h \
    addnewcamera.h \
    searchcamera.h

FORMS    +=

RESOURCES += \
    default.qrc
