isEmpty(COMMON_PRI_INCLUDED) {
COMMON_PRI_INCLUDED = 1

# name
NAME = newsflow

# version
isEmpty(VERSION) {
    VERSION = 0.0.0
}

# DEPLOYMENT_PREFIX is used in debug builds for
# testing. It is defaulted as /usr, but could
# be a local path
isEmpty(DEPLOYMENT_PREFIX) {
    OVERRIDEN_DEPLOYMENT_PREFIX = /usr
} else {
    OVERRIDEN_DEPLOYMENT_PREFIX = $${DEPLOYMENT_PREFIX}
}

isEmpty(PREFIX) {
    CONFIG(release):PREFIX = /usr
    CONFIG(debug, debug|release):PREFIX = $${OVERRIDEN_DEPLOYMENT_PREFIX}
}

# Default directories
isEmpty(LIBDIR) {
    LIBDIR = $${PREFIX}/lib
}
isEmpty(DATADIR) {
    DATADIR = $${PREFIX}/share/$${NAME}
}
isEmpty(PLUGINSDIR) {
    PLUGINSDIR = $${PREFIX}/share/$${NAME}/plugins
}
isEmpty(INCLUDEDIR) {
    INCLUDEDIR = $${PREFIX}/include/$${NAME}
}
isEmpty(QMLPATH) {
    QMLPATH = org/SfietKonstantin/$${NAME}
}
isEmpty(QMLDIR) {
    CONFIG(release):QMLDIR = $$[QT_INSTALL_QML]/$$MODULENAME
    CONFIG(debug, debug|release):QMLDIR = $${DEPLOYMENT_PREFIX}/$$[QT_INSTALL_QML]/$$MODULENAME
}
isEmpty(BINDIR) {
    BINDIR = $${PREFIX}/bin
}

# Useful defines
DEFINES += 'VERSION=\'\"$${VERSION}\"\''
}
