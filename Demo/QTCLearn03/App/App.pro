include(../QTC.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $$IDE_APP_NAME
DESTDIR = $$IDE_BIN_PATH

CONFIG += c++11

TEMPLATE = app

INCLUDEPATH +=$$PWD/../Misc/
INCLUDEPATH +=$$PWD/../Plugins/


SOURCES += \
    main.cpp


LIBS *= -l$$qtLibraryName(Misc) -l$$qtLibraryName(Plugins)
