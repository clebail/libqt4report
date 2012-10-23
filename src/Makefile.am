ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libqt4report.la

AM_CPPFLAGS = $(QT4_CFLAGS) $(LOG4CPP_CFLAGS) -DDATADIR='"$(datarootdir)"'

moc_FILES =

TRANSLATE =

lrelease_FILES = $(patsubst %.ts,%.qm, $(TRANSLATE))

libqt4report_la_SOURCES =	CReport.cpp CReport.h \
				CDocument.cpp CDocument.h \
				CDocumentParser.cpp CDocumentParser.h \
				CPrintableObject.h \
				CDocBand.cpp CDocBand.h \
				CItem.cpp CItem.h \
				CField.cpp CField.h \
				CFields.cpp CFields.h  \
				CFont.h \
				CFonts.cpp CFonts.h \
				CScript.cpp CScript.h \
				CValueType.cpp CValueType.h
						
nodist_libqt4report_la_SOURCES = $(moc_FILES)
			
libqt4report_la_LIBADD = $(QT4_LIBS) $(LOG4CPP_LIBS) 

libqt4report_ladir = $(datarootdir)/@PACKAGE@

libqt4report_la_DATA = $(lrelease_FILES) log4cpp.properties

include_dir = $(prefix)/include

moc_%.cpp: %.h
	$(MOC) -o $@ $<
	
%.qm: %.ts
	$(LRELEASE) $< -qm $@

$(libqt4report_la_OBJECTS): $(moc_FILES) $(lrelease_FILES)
	
clean-qt-extra:
	rm -f *.o
	rm -f *.lo
	rm -f moc_*.cpp
	rm -f *.qm
	rm -f $(SCHEMA)
	
clean-am: clean-qt-extra

install-data-hook:
	mkdir -p $(libqt4report_ladir)/schema; \
	mkdir -p $(libqt4report_ladir)/example; \
	mkdir -p $(include_dir); \
	cp $(srcdir)/CReport.h $(include_dir); \
	cp $(srcdir)/schema/libqt4report.xsd $(libqt4report_ladir)/schema; \
	cp $(srcdir)/log4cpp.properties $(libqt4report_ladir); \
	cp $(srcdir)/viewer/test.xlqr $(libqt4report_ladir)/example
	
dist-hook:
	mkdir $(distdir)/schema; \
	cp $(srcdir)/log4cpp.properties $(distdir); \
	cp $(srcdir)/schema/libqt4report.xsd $(distdir)/schema
