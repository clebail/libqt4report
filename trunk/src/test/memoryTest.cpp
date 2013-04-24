//------------------------------------------------------------------------------
#include <QApplication>
#include <QTextCodec>
#include <QTextStream>
#include <QDir>
#include <QSqlDatabase>
#include <iostream>
#include <libqt4report.h>
//------------------------------------------------------------------------------
void process(QString filename) {
	QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL", "test");
	db.setDatabaseName("test");
	db.open("test","");
	
	for(int i=0;i<10000;i++) {
		QFile *xmlFile=new QFile(filename);
		if(xmlFile->open(QIODevice::ReadOnly)) {
			libqt4report::CReport *report=new libqt4report::CReport("test");

			report->setParamValue("fromCommande", QVariant(1));
			report->setParamValue("toCommande", QVariant(1));
			
			if(!report->process(xmlFile)) {
				std::cout << report->getLastSourceError().toStdString() << std::endl;
				std::cout << report->getLastError().toStdString() << std::endl;
			}

			delete report;
		}else {
			std::cout << "Unable to open report file" << std::endl;
		}
		delete xmlFile;
	}
}
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

	QApplication::installTranslator(libqt4report::CReport::getTranslator());

	process(QDir::toNativeSeparators("../example/testMySQL.xlqr"));

	return 0;
}
//------------------------------------------------------------------------------