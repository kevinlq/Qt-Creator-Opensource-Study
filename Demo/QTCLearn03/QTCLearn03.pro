include($$PWD/QTC.pri)

TEMPLATE  = subdirs

SUBDIRS += \
    $$PWD/Misc \
    $$PWD/Plugins \
    $$PWD/App
	
CONFIG   += ordered
