include(../../../common.pri)

TEMPLATE = lib
TARGET = $${NAME}-time
CONFIG += plugin static
INCLUDEPATH += ../../lib

HEADERS += \
    timeplugin.h \
    timearticlescorer.h

SOURCES += \
    timeplugin.cpp \
    timearticlescorer.cpp


