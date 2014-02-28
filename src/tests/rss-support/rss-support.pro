include(../../../common.pri)

TEMPLATE = app
TARGET = tst_rss-support

QT = core gui network testlib

INCLUDEPATH += ../../lib
LIBS += -L../../lib -l$${NAME}

SOURCES += tst_rss-support.cpp


