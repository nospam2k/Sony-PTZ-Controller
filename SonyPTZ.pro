#-------------------------------------------------
#
# Project created by QtCreator 2019-04-12T07:25:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SonyPTZ
TEMPLATE = app
DESTDIR = $$PWD/../rundir

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
RC_ICONS = app.ico
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sonycam.cpp \
    presetsetupdlg.cpp \
    sonyptzapp.cpp \
    appsettings.cpp \
    listitem.cpp \
    camerasetupdlg.cpp \
    camerabtn.cpp \
    sonycameramanager.cpp

HEADERS += \
        mainwindow.h \
    sonycam.h \
    presetsetupdlg.h \
    sonyptzapp.h \
    appsettings.h \
    listitem.h \
    camerasetupdlg.h \
    camerabtn.h \
    sonycameramanager.h

FORMS += \
        mainwindow.ui \
    presetsetupdlg.ui \
    listitem.ui \
    camerasetupdlg.ui

RESOURCES += \
    sonyptz.qrc
