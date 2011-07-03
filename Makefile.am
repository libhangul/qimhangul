# qimhangul

moduledir = $(QT_PLUGIN_DIR)/inputmethods
module_LTLIBRARIES = libqimhangul.la

libqimhangul_la_SOURCES = \
	candidatelist.cpp \
	candidatelist.h \
	qinputcontexthangul.cpp \
	qinputcontexthangul.h \
	qinputcontextpluginhangul.cpp \
	qinputcontextpluginhangul.h \
	$(NUL)

if QT_X11
libqimhangul_la_SOURCES += qinputcontexthangul_x11.cpp
endif

libqimhangul_la_CXXFLAGS = \
	-DQT_PLUGIN \
	-DQT_GUI_LIB \
	-DQT_CORE_LIB \
	$(QT_CFLAGS) \
	$(LIBHANGUL_CFLAGS) \
	$(NUL)

libqimhangul_la_LDFLAGS = -module -avoid-version
libqimhangul_la_LIBADD = $(QT_LIBS) $(LIBHANGUL_LIBS)

DISTCHECK_CONFIGURE_FLAGS = \
	--with-qt-plugin-dir=$${dc_install_base}/lib/qt4/plugins

EXTRA_DIST = qimhangul.pro

dist-hook:
	if test -d .svn; then \
		export LANG=C ; \
		export LC_CTYPE=en_US.UTF-8 ; \
		svn log -v  > $(distdir)/ChangeLog ; \
	fi
