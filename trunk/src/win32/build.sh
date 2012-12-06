#!/bin/sh

cd ../..
./configure --host=i586-mingw32msvc CXXFLAGS=-I/opt/win32/include QT4_CFLAGS="-DQT_SHARED -I/home/corentin/VBoxShared/Qt/4.8.4/include -I/home/corentin/VBoxShared/Qt/4.8.4/include/QtCore -I/home/corentin/VBoxShared/Qt/4.8.4/include/QtGui -I/home/corentin/VBoxShared/Qt/4.8.4/include/QtNetwork -I/home/corentin/VBoxShared/Qt/4.8.4/include/QtXml -I/home/corentin/VBoxShared/Qt/4.8.4/include/QtXmlPatterns -I/home/corentin/VBoxShared/Qt/4.8.4/include/QtSvg -I/home/corentin/VBoxShared/Qt/4.8.4/include/QtSql -I/home/corentin/VBoxShared/Qt/4.8.4/include/QtScript" QT4_LIBS="-L/home/corentin/VBoxShared/Qt/4.8.4/lib -lQtCore4 -lQtGui4 -lQtNetwork4 -lQtXml4 -lQtXmlPatterns4 -lQtSvg4 -lQtSql4 -lQtScript4" --prefix=/opt/win32/libqt4report LOG4CPP_CFLAGS=/opt/win32/include LOG4CPP_LIBS="-L/opt/win32/lib -llog4cpp -lpthread -lws2_32"
#LDFLAGS=-static-libgcc

make clean && make
