# qimhangul project file

TEMPLATE = lib

target.path = $(QTDIR)/plugins/inputmethods
INSTALLS += target

CONFIG += debug plugin qt thread

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
