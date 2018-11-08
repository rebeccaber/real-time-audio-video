TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    processor.cpp \
    MicProcessor.cpp \
    CameraProcessor.cpp \
    AudioInterface.cpp \
    Camera.cpp \
    tcpsocketserver.cpp \
    TCPClient.cpp \
    TCPOutProcessor.cpp

HEADERS  += \
    Camera.h \
    processor.h \
    MicProcessor.h \
    CameraProcessor.h \
    AudioInterface.h \
    audioconfig.h \
    tcpsocketserver.h \
    TCPClient.h \
    TCPOutProcessor.h

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -pthread
CONFIG += c++11

LIBS += -lpthread
LIBS += -lasound

# --- OPEN CV ---
INCLUDEPATH += /usr/include/opencv
LIBS += -L/usr/lib -lopencv_core -lopencv_highgui

include(deployment.pri)
qtcAddDeployment()

