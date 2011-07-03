#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([qimhangul], [0.2.0], [http://kldp.net/projects/qimhangul/bugs])
AC_CONFIG_SRCDIR([qinputcontextpluginhangul.cpp])
AM_INIT_AUTOMAKE([])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

# libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Checks for libraries.
PKG_CHECK_MODULES(QT, QtGui >= 4.0.0,,
	AC_MSG_ERROR([qimhangul needs Qt 4.0.0 or higher]))

PKG_CHECK_MODULES(LIBHANGUL, libhangul >= 0.0.12,,
	AC_MSG_ERROR([qimhangul needs libhangul 0.0.12 or higher]))

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for qmake
AC_CHECK_PROG(have_qmake, qmake, yes, no,,)

# default QT_PLUGIN_DIR
AC_MSG_CHECKING(for Qt plugin install directory)
QT_PLUGIN_DIR="${libdir}/qt4/plugins"
if test "$have_qmake" = "yes"; then
	QT_PLUGIN_DIR=`qmake -query QT_INSTALL_PLUGINS`
fi

AC_ARG_WITH(qt_plugin_dir, [  --with-qt-plugin-dir=DIR  select Qt plugin dir to install @<:@default=auto detect@:>@])
if test -n "$with_qt_plugin_dir"; then
	QT_PLUGIN_DIR="${with_qt_plugin_dir}"
fi
AC_SUBST(QT_PLUGIN_DIR)
AC_MSG_RESULT($QT_PLUGIN_DIR)

# Check whether Qt is configured with X11
AC_MSG_CHECKING(whether Qt is configured with X11)
if pkg-config QtGui --variable=qt_config | grep x11 >& /dev/null; then
	qt_have_X11=yes
else
	qt_have_X11=no
fi
AC_MSG_RESULT($qt_have_X11)
AM_CONDITIONAL([QT_X11], [test "$qt_have_X11" = "yes"])

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
