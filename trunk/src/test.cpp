#include <QApplication>
#include <QTextCodec>
#include <QTextStream>
#include <iostream>
#include <CReport.h>

void process(char *filename) {
	QFile *xmlFile=new QFile(filename);
	if(xmlFile->open(QIODevice::ReadOnly)) {
		libqt4report::CReport *report=new libqt4report::CReport();
		
		QApplication::installTranslator(report->getTranslator());
		
		if(report->process(xmlFile)) {
			for(int i=0;i<report->getNbPage();i++) {
				QString outFileName=QString(filename)+"_"+QString::number(i+1)+".svg";
				QFile *outFile=new QFile(outFileName);
				
				if(outFile->open(QIODevice::WriteOnly)) {
					QTextStream out(outFile);
					
					out << report->toSvg(i);
					
					outFile->close();
				}else {
					std::cout << "Unable to write file " << outFileName.toStdString() << std::endl;
					delete outFile;
					delete report;
					delete xmlFile;
					
					return;
				}
				
				delete outFile;
			}
		}else {
			std::cout << report->getLastSourceError().toStdString() << std::endl;
			std::cout << report->getLastError().toStdString() << std::endl;
		}
		
		delete report;
	}
	delete xmlFile;
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		std::cout << "Usage : " << argv[0] << " xlqrfile " << std::endl;
		
		return 1;
	}
	
	QApplication app(argc, argv);
	
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
		
	process(argv[1]);
}