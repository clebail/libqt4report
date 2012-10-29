ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libqt4report.la

AM_CPPFLAGS = $(QT4_CFLAGS) $(LOG4CPP_CFLAGS) -DDATADIR='"$(datarootdir)"'

moc_FILES =

TRANSLATE = libqt4report_fr_FR.ts

lrelease_FILES = libqt4report_fr_FR.qm

libqt4report_la_SOURCES =	libqt4report.cpp libqt4report.h \
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

example_dir = $(srcdir)/example

example_file = $(example_dir)/test.cpp

moc_%.cpp: %.h
	$(MOC) -o $@ $<
	
%.qm: %.ts
	$(LRELEASE) $< -qm $@

$(libqt4report_la_OBJECTS): $(moc_FILES) $(lrelease_FILES) $(example_file)

$(example_file): $(example_file).in
	sed 's|@DATAROOT_DIR@|$(libqt4report_ladir)|g' $< > $@
	
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
	mkdir -p $(libqt4report_ladir)/doc; \
	mkdir -p $(includedir); \
	mkdir -p $(libdir)/pkgconfig; \
	cp $(srcdir)/libqt4report.h $(includedir); \
	cp $(srcdir)/schema/libqt4report.xsd $(libqt4report_ladir)/schema; \
	cp $(srcdir)/log4cpp.properties $(libqt4report_ladir); \
	cp $(srcdir)/viewer/test.xlqr $(libqt4report_ladir)/example; \
	cp $(srcdir)/example/test.cpp $(libqt4report_ladir)/example; \
	cp $(srcdir)/example/makefile $(libqt4report_ladir)/example; \
	cp $(srcdir)/doc/libqt4report.html $(libqt4report_ladir)/doc; \
	cp $(srcdir)/$(lrelease_FILES) $(libqt4report_ladir); \
	cp $(srcdir)/libqt4report.pc $(libdir)/pkgconfig
	
dist-hook:
	mkdir $(distdir)/schema; \
	mkdir $(distdir)/doc; \
	mkdir $(distdir)/example; \
	cp $(srcdir)/$(TRANSLATE) $(distdir); \
	cp $(srcdir)/log4cpp.properties $(distdir); \
	cp $(srcdir)/schema/libqt4report.xsd $(distdir)/schema; \
	cp $(srcdir)/doc/libqt4report.html $(distdir)/doc; \
	cp $(example_file).in $(distdir)/example
	cp $(example_dir)/makefile $(distdir)/example
