include(../../common.pri)

TEMPLATE = lib
TARGET = $${NAME}
CONFIG += qt create_prl no_install_prl create_pc static
QT = core network

DEFINES += NEWSFLOW_LIBRARY

target.path = $${LIBDIR}

HEADERS += newsflow_global.h \
    ifeedsource.h \
    iarticlescorer.h \
    processerror.h \
    feeddata.h \
    feeddata_p.h \
    feed_p.h \
    feed.h \
    article_p.h \
    article.h \
    articledata.h \
    articledata_p.h \
    feedmanager_p.h \
    feedmanager.h \
    feedadder.h \
    feedadder_p.h \
    feedmodel.h \
    feedmodel_p.h \
    articlesmodel.h \
    articlesmodel_p.h \
    rsssupport.h \
    rsssupport_p.h \
    articleformatter.h \
    abstractfeedfetcher.h \
    abstractfeedfetcher_p.h \
    abstractarticlescorer.h \
    abstractarticlescorer_p.h

SOURCES += \
    feeddata.cpp \
    feed.cpp \
    article.cpp \
    articledata.cpp \
    feedmanager.cpp \
    feedadder.cpp \
    feedmodel.cpp \
    articlesmodel.cpp \
    rsssupport.cpp \
    articleformatter.cpp \
    abstractfeedfetcher.cpp \
    abstractarticlescorer.cpp

QMAKE_PKGCONFIG_NAME = lib$$TARGET
QMAKE_PKGCONFIG_DESCRIPTION = News flow library
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

headers.files = $$PWD/*.h
headers.path = $${INCLUDEDIR}

INSTALLS += target headers
