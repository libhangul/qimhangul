# qimhangul project file

TEMPLATE = lib
TARGET = qimhangul
VERSION = 0.0.3
DISTFILES += ChangeLog COPYING AUTHORS

target.path = $(QTDIR)/plugins/inputmethods
INSTALLS += target

CONFIG += debug plugin qt thread

# Input
HEADERS += \
    qinputcontexthangul.h \
    qinputcontextpluginhangul.h \
    candidatetable.h \
    candidatelist.h

SOURCES += \
    qinputcontexthangul.cpp \
    qinputcontextpluginhangul.cpp \
    candidatelist.cpp

INCLUDEPATH += $$system(pkg-config libhangul --cflags-only-I | sed -e 's/-I//')
LIBS += $$system(pkg-config libhangul --libs)

unix {
    SOURCES += qinputcontexthangul_x11.cpp
}
