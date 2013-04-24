bin_PROGRAMS = memoryTest

AM_CPPFLAGS = $(QT4_CFLAGS) -I..  -DDATADIR='"$(datarootdir)"'

memoryTest_SOURCES = memoryTest.cpp 
			
memoryTest_LDADD = $(QT4_LIBS) -L../.libs -lqt4report

install-data-hook:
	rm $(bindir)/memoryTest