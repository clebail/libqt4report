CC = mingw32-g++

QT_INC_PATH = C:\QtSDK\Desktop\Qt\4.8.1\mingw\include
QT_LIB_PATH = C:\QtSDK\Desktop\Qt\4.8.1\mingw\lib

BUILD_DIR = build
ALL=$(BUILD_DIR)\viewer.exe
CFLAGS = -Wall -I. -I$(BUILD_DIR) -I.. -I..\.. -I..\..\win32  -DQT_SHARED -I$(QT_INC_PATH) -I$(QT_INC_PATH)\QtCore -I$(QT_INC_PATH)\QtGui -I$(QT_INC_PATH)\QtSvg  -DDATADIR='"."'
LDFLAGS = -mthreads -Wl,-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -mwindows -Wl,-s
LIBS = -L$(QT_LIB_PATH) -lQtCore4 -lQtGui4 -lQtSvg4 -L..\..\win32\build -lqt4report

RES=viewer.res

UI = $(BUILD_DIR)/ui_CMainFrm.h

MOC =	moc_CMainFrm.cpp

QRC = qrc_viewer.cpp

SRC = 	viewer.cpp \
	CMainFrm.cpp \
	$(MOC) \
	$(QRC)

OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC)) 

all: $(BUILD_DIR) $(UI) $(ALL)

.PHONY: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(ALL): $(OBJ) $(RES)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)
	
$(BUILD_DIR)/%.o: ../%.cpp
	$(CC) -o $@ $(CFLAGS) -c $<

$(BUILD_DIR)/moc_%.o: $(BUILD_DIR)/moc_%.cpp
	$(CC) -o $@ $(CFLAGS) -c $<

$(BUILD_DIR)/qrc_%.o: $(BUILD_DIR)/qrc_%.cpp
	$(CC) -o $@ $(CFLAGS) -c $<

$(BUILD_DIR)/moc_%.cpp: ../%.h
	moc $< -o $@

$(BUILD_DIR)/ui_%.h: ../%.ui
	uic -o $@ $<

$(BUILD_DIR)/qrc_%.cpp: ../%.qrc
	rcc -o $@ $<
	
$(RES): viewer.rc
	windres $< -O coff -o $@
	
clean:
	rmdir /S /Q $(BUILD_DIR)
