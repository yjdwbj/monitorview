#-------------------------------------------------
#
# Project created by QtCreator 2013-10-28T10:51:20
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebCameraClient
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    setting_panel.cpp \
    viewframe.cpp \
    addnewcamera.cpp \
    searchcamera.cpp \
    global.cpp \
    system_setting.cpp \
    control_widget.cpp \
    lansearchcamera.cpp \
    alarmaction.cpp \
    camera_settings.cpp \
    sqldriver.cpp \
    alarmtrapsettings.cpp

HEADERS  += mainwindow.h \
    setting_panel.h \
    global.h \
    viewframe.h \
    addnewcamera.h \
    searchcamera.h \
    system_setting.h \
    control_widget.h \
    lansearchcamera.h \
    alarmaction.h \
    camera_settings.h \
    sqldriver.h \
    alarmtrapsettings.h


INCLUDEPATH += $$PWD/sdk/include

LIBS +=   -L$$PWD/lib \
          $$PWD/sdk/lib/libvlccore.lib \
          $$PWD/sdk/lib/libvlc.lib \



#LIBS += -lvlc
RESOURCES += \
    default.qrc

FORMS += \
    lansearchcamera.ui \
    camera_settings.ui \
    viewcameralist.ui \
    alarmtrapsettings.ui \
    record_time.ui

