# qimhangul project file

TEMPLATE = lib
TARGET = qimhangul
VERSION = 0.0.2
DISTFILES += ChangeLog COPYING AUTHORS

target.path = $(QTDIR)/plugins/inputmethods
INSTALLS += target

CONFIG += debug plugin qt thread

# Input
HEADERS += \
    qinputcontexthangul.h \
    qinputcontextpluginhangul.h \
    hangul.h \
    hangulkeyboard.h \
    candidatetable.h \
    candidatelist.h

SOURCES += \
    qinputcontexthangul.cpp \
    qinputcontextpluginhangul.cpp \
    hangulctype.c \
    hangulinputcontext.c \
    candidatelist.cpp

unix {
    SOURCES += qinputcontexthangul_x11.cpp
}
