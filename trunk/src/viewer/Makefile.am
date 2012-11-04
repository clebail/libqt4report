bin_PROGRAMS = viewer

AM_CPPFLAGS = $(QT4_CFLAGS) -I..  -DDATADIR='"$(datarootdir)"'

moc_FILES = moc_CMainFrm.cpp
		
INTERFACES = CMainFrm.ui
		
ui_FILES = ui_CMainFrm.h

RESSOURCES = viewer.qrc
		
qrc_FILES = qrc_viewer.cpp

TRANSLATE = 	viewer_fr_FR.ts \
		viewer_cs_CZ.ts

lrelease_FILES =	viewer_fr_FR.qm \
			viewer_cs_CZ.qm

viewer_SOURCES = 	viewer.cpp \
			CMainFrm.cpp CMainFrm.h
			
nodist_viewer_SOURCES = $(moc_FILES) $(qrc_FILES)
			
viewer_LDADD = $(QT4_LIBS) ../.libs/libqt4report.la

viewerdir = $(datarootdir)/@PACKAGE@

viewer_DATA = $(lrelease_FILES)

moc_%.cpp: %.h
	$(MOC) -o $@ $<
	
ui_%.h: %.ui
	$(UI) -o $@ $<
	
qrc_%.cpp: %.qrc
	$(RCC) -o $@ $<
	
%.qm: %.ts
	$(LRELEASE) $< -qm $@
	
$(viewer_OBJECTS): $(ui_FILES) $(moc_FILES) $(qrc_FILES) $(lrelease_FILES)

clean-qt-extra:
	rm -rf moc_*.cpp
	rm -rf ui_*.h
	rm -rf *.qm
	rm -rf qrc_*.cpp
	rm -rf *.o
	
clean-am: clean-qt-extra

install-data-hook:
	cp $(srcdir)/$(lrelease_FILES)	$(viewerdir)

dist-hook:
	mkdir $(distdir)/images; \
	cp $(srcdir)/$(INTERFACES) $(distdir); \
	cp $(srcdir)/$(TRANSLATE) $(distdir); \
	cp $(srcdir)/$(RESSOURCES) $(distdir); \
	cp $(srcdir)/images/*.png $(distdir)/images; \
	cp $(srcdir)/test.xlqr $(distdir);
