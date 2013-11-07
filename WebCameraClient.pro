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
    searchcamera.cpp \
    detail_setting/camera_setting.cpp \
    global.cpp \
    detail_setting/setting_trap_time.cpp \
    detail_setting/alarmaction.cpp \
    system_setting.cpp

HEADERS  += mainwindow.h \
    setting_panel.h \
    global.h \
    viewframe.h \
    addnewcamera.h \
    searchcamera.h \
    detail_setting/camera_setting.h \
    detail_setting/setting_trap_time.h \
    detail_setting/alarmaction.h \
    system_setting.h


INCLUDEPATH += $$PWD\sdk\include

LIBS +=   -L$$PWD/sdk/lib \
          $$PWD/sdk/lib/libvlccore.lib \
          $$PWD/sdk/lib/libvlc.lib
#          -l"O:/tool/WebCameraClient-build-Desktop_Qt_5_0_1_MinGW_32bit-Debug/debug/libvlc.dll" \
#          -l"O:/tool/WebCameraClient-build-Desktop_Qt_5_0_1_MinGW_32bit-Debug/debug/libccore.dll"

#LIBS += -lvlc
RESOURCES += \
    default.qrc
