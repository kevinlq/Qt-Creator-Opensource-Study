include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$QTC_LIB_NAME

include(../QTC.pri)

DESTDIR = $$IDE_LIBRARY_PATH
DEFINES += MISC_LIBRARY


TARGET = $$qtLibraryTargetName($$TARGET)

TEMPLATE = lib
CONFIG += shared dll

SOURCES += \
    MiscUtil.cpp

HEADERS += \
    MiscUtil.h
