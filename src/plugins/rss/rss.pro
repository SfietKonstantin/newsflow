include(../../../common.pri)

TEMPLATE = lib
TARGET = $${NAME}-rss
CONFIG += plugin static
INCLUDEPATH += ../../lib

HEADERS += \
    rssplugin.h \
    rssfeedfetcher.h \
    rssfeedfetcher_p.h

SOURCES += \
    rssplugin.cpp \
    rssfeedfetcher.cpp

target.path = $${PLUGINSDIR}
INSTALLS += target

