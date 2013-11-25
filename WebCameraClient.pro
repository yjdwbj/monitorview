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
    detail_setting/camera_setting.cpp \
    global.cpp \
    detail_setting/setting_trap_time.cpp \
    detail_setting/alarmaction.cpp \
    system_setting.cpp \
    control_widget.cpp

HEADERS  += mainwindow.h \
    setting_panel.h \
    global.h \
    viewframe.h \
    addnewcamera.h \
    searchcamera.h \
    detail_setting/camera_setting.h \
    detail_setting/setting_trap_time.h \
    detail_setting/alarmaction.h \
    system_setting.h \
    control_widget.h


INCLUDEPATH += O:/tool/WebCameraClient/sdk/include

LIBS +=   -LO:/tool/WebCameraClient/sdk/lib \
          O:/tool/WebCameraClient/sdk/lib/libvlccore.lib \
          O:/tool/WebCameraClient/sdk/lib/libvlc.lib


#LIBS += -lvlc
RESOURCES += \
    default.qrc
