include(../../../common.pri)

TEMPLATE = lib
TARGET = $${NAME}-time
CONFIG += plugin static
INCLUDEPATH += ../../lib

HEADERS += \
    time.h

SOURCES += \
    time.cpp

target.path = $${PLUGINSDIR}
INSTALLS += target

