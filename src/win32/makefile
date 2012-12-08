CC = i586-mingw32msvc-g++

QT_INC_PATH = /home/corentin/VBoxShared/Qt/4.8.4/include
QT_LIB_PATH = /home/corentin/VBoxShared/Qt/4.8.4/lib
WIN32_INC_PATH = /opt/win32/include
WIN32_LIB_PATH = /opt/win32/lib

LIBNAME = build/libqt4report
ALL=$(LIBNAME).dll
CFLAGS = -Wall -I. -I.. -I../.. -I$(WIN32_INC_PATH) -DQT_SHARED -I$(QT_INC_PATH) -I$(QT_INC_PATH)/QtCore -I$(QT_INC_PATH)/QtGui -I$(QT_INC_PATH)/QtNetwork -I$(QT_INC_PATH)/QtXml -I$(QT_INC_PATH)/QtXmlPatterns -I$(QT_INC_PATH)/QtSvg -I/$(QT_INC_PATH)/QtSql -I/$(QT_INC_PATH)/QtScript -DDATADIR .
LDFLAGS = -Wl -Wl,-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -enable-stdcall-fixup -Wl,-s -shared -Wl,--out-implib,$(LIBNAME).a -mwindows
LIBS =  -L$(QT_LIB_PATH) -lQtCore4 -lQtGui4 -lQtNetwork4 -lQtXml4 -lQtXmlPatterns4 -lQtSvg4 -lQtSql4 -lQtScript4 -L$(WIN32_LIB_PATH) -lpthread  -llog4cpp -lws2_32 -lgcc_s
BUILD_DIR = build

MOC =	moc_libqt4report.cpp \
	moc_CDocumentParser.cpp \
	moc_CFont.cpp 

SRC = 	libqt4report.cpp \
	CDocument.cpp \
	CDocumentParser.cpp \
	CDocBand.cpp \
	CItem.cpp \
	CField.cpp \
	CFields.cpp  \
	CFont.cpp \
	CFonts.cpp \
	CScript.cpp \
	CValueType.cpp \
	CPage.cpp \
	CRendererObject.cpp \
	CGroup.cpp \
	CGroups.cpp \
	CPageManager.cpp \
	$(MOC)
	

OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC)) 

all: $(BUILD_DIR) $(ALL)

.PHONY: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(ALL): $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)
	
$(BUILD_DIR)/%.o: ../%.cpp
	$(CC) -o $@ $(CFLAGS) -c $<

$(BUILD_DIR)/moc_%.cpp: ../%.h
	moc $< -o $@
	
clean:
	rm -rf $(BUILD_DIR)
