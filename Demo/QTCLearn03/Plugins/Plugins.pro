include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$QTC_PLUGIN_NAME

include(../QTC.pri)

DESTDIR = $$IDE_LIBRARY_PATH
TARGET = $$qtLibraryTargetName($$TARGET)
TEMPLATE = lib
CONFIG += shared dll
DEFINES *= PLUGIN_LIBRARY

INCLUDEPATH +=$$PWD/../Misc


HEADERS += \
    $$PWD/Plugins.h

SOURCES += \
    $$PWD/Plugins.cpp
