AC_INIT([libqt4report], [1.0.0-preAlpha2], [corentin.lebail@gmail.com],,[https://sourceforge.net/projects/libqt4report/]) 

LT_INIT
LT_LANG([C++])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE()

AC_PROG_CC
AC_PROG_CXX
AC_PROG_MAKE_SET

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([Makefile src/Makefile src/viewer/Makefile])

AC_CHECK_PROG([MOC], [moc-qt4], [moc-qt4], [no])
if test "$MOC" = no; then
	AC_CHECK_PROG([MOC], [moc], [moc], [no])
	if test "$MOC" = no; then
		AC_MSG_ERROR([moc not found! Make sure you have Qt-devel-tools installed!])
	fi
fi

AC_CHECK_PROG([UI], [uic-qt4], [uic-qt4], [no])
if test "$UI" = no; then
	AC_CHECK_PROG([UI], [uic], [uic], [no])
	if test "$UI" = no; then
		AC_MSG_ERROR([uic not found! Make sure you have Qt-devel-tools installed!])
	fi
fi

AC_CHECK_PROG([LRELEASE], [lrelease-qt4], [lrelease-qt4], [no])
if test "$LRELEASE" = no; then
	AC_CHECK_PROG([LRELEASE], [lrelease], [lrelease], [no])
	if test "$LRELEASE" = no; then
		AC_MSG_ERROR([lrelease not found! Make sure you have Qt-devel-tools installed!])
	fi
fi

AC_CHECK_PROG([RCC], [rcc], [rcc], [no])
if test "$MOC" = no; then
	AC_MSG_ERROR([moc not found! Make sure you have Qt-devel-tools installed!])
fi

PKG_CHECK_MODULES([QT4], [QtCore QtGui QtNetwork QtXml QtXmlPatterns QtSvg QtSql QtScript])
PKG_CHECK_MODULES([LOG4CPP], [log4cpp])

AC_OUTPUT
