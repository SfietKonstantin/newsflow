include(../../../common.pri)

TEMPLATE = lib
TARGET = $${NAME}-rss
CONFIG += plugin static
INCLUDEPATH += ../../lib

HEADERS += \
    rss.h

SOURCES += \
    rss.cpp

target.path = $${PLUGINSDIR}
INSTALLS += target

