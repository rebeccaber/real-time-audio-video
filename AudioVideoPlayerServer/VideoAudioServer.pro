#-------------------------------------------------
#
# Project created by QtCreator 2018-02-17T18:13:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = VideoAudioServer
TEMPLATE = app

LIBS += -lasound
LIBS += -lpthread

SOURCES += main.cpp \
    videoviewer.cpp \
    processor.cpp \
    videoprocessor.cpp \
    AudioInterface.cpp \
    audioprocessor.cpp \
    tcpinprocessor.cpp \
    tcpsocketserver.cpp

HEADERS  += \
    videoviewer.h \
    processor.h \
    videoprocessor.h \
    AudioInterface.h \
    audioconfig.h \
    audioprocessor.h \
    tcpinprocessor.h \
    tcpsocketserver.h

FORMS    += mainwindow.ui
