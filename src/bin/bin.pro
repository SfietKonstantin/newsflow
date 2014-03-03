include(../../common.pri)

TARGET = harbour-$${NAME}

CONFIG(desktop):QT = core gui network quick
CONFIG(desktop):CONFIG += link_pkgconfig
CONFIG(desktop):PKGCONFIG += sailfishapp
CONFIG(desktop):DEFINES += DESKTOP
CONFIG(desktop):RESOURCES += res.qrc
CONFIG += sailfishapp

INCLUDEPATH += ../lib
LIBS += -L../plugins/rss -l$${NAME}-rss
LIBS += -L../plugins/time -l$${NAME}-time
LIBS += -L../lib -l$${NAME}

SOURCES += main.cpp

OTHER_FILES += qml/harbour-newsflow.qml \
    qml/MainPage.qml \
    qml/AddFeedPage.qml \
    qml/ArticlesPage.qml \
    qml/ArticlePage.qml

target.path = $${BINDIR}

qmlFiles.files = qml/*.qml
qmlFiles.path = $${DATADIR}/qml

desktopFile.files = $$PWD/data/$${TARGET}.desktop
desktopFile.path = /usr/share/applications

iconFile.files = $$PWD/data/$${TARGET}.png
iconFile.path = /usr/share/icons/hicolor/86x86/apps/

INSTALLS += target qmlFiles
!CONFIG(desktop):INSTALLS += desktopFile iconFile

# Translations
TS_FILE = $$OUT_PWD/newsflow.ts
EE_QM = $$PWD/newsflow-engineering-english.qm

ts.commands += lupdate $$PWD -ts $$TS_FILE
ts.CONFIG += no_check_exist
ts.output = $$TS_FILE
ts.input = .

QMAKE_EXTRA_TARGETS += ts
PRE_TARGETDEPS += ts

# Engineering english
engineering_english.commands += lrelease -idbased $$TS_FILE -qm $$EE_QM
engineering_english.CONFIG += no_check_exist
engineering_english.depends = ts

QMAKE_EXTRA_TARGETS += engineering_english
PRE_TARGETDEPS += engineering_english

engineering_english_install.path = $${DATADIR}/translations
engineering_english_install.files = $$EE_QM

#translations_install.path = /usr/share/$${TARGET}/translations
#translations_install.files = $$PWD/newsflow_*.qm

!CONFIG(desktop):INSTALLS += engineering_english_install
#INSTALLS += translations_install

