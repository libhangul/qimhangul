# qimhangul project file

TEMPLATE = lib
TARGET = qimhangul
VERSION = 0.0.1

target.path = $(QTDIR)/plugins/inputmethods
INSTALLS += target

CONFIG += debug plugin qt

# Input
HEADERS += \
    qinputcontexthangul.h \
    qinputcontextpluginhangul.h \
    hangul.h \
    hangulcomposer.h \
    candidatelist.h

SOURCES += \
    qinputcontexthangul.cpp \
    qinputcontextpluginhangul.cpp \
    hangul.c \
    hangulcomposer.cpp \
    candidatelist.cpp

unix {
    SOURCES += qinputcontexthangul_x11.cpp
}
