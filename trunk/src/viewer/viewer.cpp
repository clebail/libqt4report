//------------------------------------------------------------------------------
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QDir>
#include <config.h>
#include "CMainFrm.h"
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
	QTranslator translator;
	int ret;
	QString fileName;
	QApplication app(argc, argv);
	
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	
	translator.load("viewer_"+QLocale::system().name(), QDir(QString(DATADIR)+"/"+QString(PACKAGE)).absolutePath());
	
	app.installTranslator(&translator);
	
	app.setApplicationName("viewer");
	app.setApplicationVersion(PACKAGE_VERSION);
	
	if(argc > 1) {
		fileName=argv[1];
	}
	
	CMainFrm *mainFrm=new CMainFrm(fileName);
	mainFrm->showMaximized();

	ret=app.exec();
	
	return ret;
}
//------------------------------------------------------------------------------