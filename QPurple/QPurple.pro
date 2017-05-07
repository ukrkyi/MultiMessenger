#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T16:03:23
#
#-------------------------------------------------

TARGET = QPurple
TEMPLATE = lib
#CONFIG += staticlib

DEFINES += QPURPLE_LIBRARY

SOURCES += qpurple.cpp \
    purpleinteraction.cpp \
    purpleionotifier.cpp

HEADERS += qpurple.h\
        qpurple_global.h \
    purpleinteraction.h \
    purpleionotifier.h

unix {
MACHINE = $$system( uname -m )
 contains( MACHINE, x86_64 ) {
  target.path = /usr/lib64
 } else {
  target.path = /usr/lib
 }
    INSTALLS += target
    CONFIG += link_pkgconfig
    PKGCONFIG += glib-2.0
    INCLUDEPATH += /usr/include/libpurple/
    LIBS +=     -lpurple
}
