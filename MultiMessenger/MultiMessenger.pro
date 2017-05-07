#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T19:50:16
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiMessenger
TEMPLATE = app


SOURCES += main.cpp\
        messenger.cpp \
    account.cpp \
    relation.cpp \
    person.cpp \
    message.cpp \
    personbutton.cpp \
    personwidget.cpp \
    settings.cpp \
    startdialog.cpp \
    facebookaccount.cpp \
    googleaccount.cpp \
    addaccount.cpp \
    addperson.cpp \
    accountmanager.cpp \
    programsettings.cpp \
    personmanager.cpp \
    addrelation.cpp

HEADERS  += messenger.h \
    baseelement.h \
    account.h \
    relation.h \
    person.h \
    message.h \
    personbutton.h \
    personwidget.h \
    settings.h \
    startdialog.h \
    facebookaccount.h \
    googleaccount.h \
    addaccount.h \
    addperson.h \
    programsettings.h \
    accountmanager.h \
    personmanager.h \
    addrelation.h

FORMS    += messenger.ui \
    personwidget.ui \
    settings.ui \
    startdialog.ui \
    addaccount.ui \
    addperson.ui \
    accountmanager.ui \
    addrelation.ui

RESOURCES += \
    res.qrc

LIBS += -L../QPurple/ -lQPurple
INCLUDEPATH += ../QPurple
DEPENDPATH += ../QPurple

unix{
    CONFIG += link_pkgconfig
    PKGCONFIG += glib-2.0

    INCLUDEPATH += /usr/include/libpurple/
    LIBS +=     -lpurple

    target.path = /usr/bin
    INSTALLS += target
}
