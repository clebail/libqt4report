//------------------------------------------------------------------------------
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QAbstractMessageHandler>
#include <QBuffer>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QtDebug>
#include <QDir>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <config.h>
#include "libqt4report.h"
#include "CDocumentParser.h"
#include "CDocument.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	//------------------------------------------------------------------------------
	static CDocument * document;
	static QTranslator *translator=0;
	//------------------------------------------------------------------------------
	CReport::CReport(void) {
		log4cpp::PropertyConfigurator::configure((QString(DATADIR)+"/"+QString(PACKAGE)+"/log4cpp.properties").toStdString());
		
		document=0;
	}
	//------------------------------------------------------------------------------
	CReport::~CReport(void) {
		if(document != 0) {
			delete document;
		}
		
		
		if(translator != 0) {
			delete translator;
		}
	}
	//------------------------------------------------------------------------------
	bool CReport::validDocument(QFile *docFile) {
		QXmlSchema *xmlSchema=new QXmlSchema();
		QFile *xsdFile=new QFile((QString(DATADIR)+"/"+QString(PACKAGE)+"/schema/libqt4report.xsd"));
		xsdFile->open(QIODevice::ReadOnly);
		bool ret=false;
		
		xmlSchema->load(xsdFile, QUrl::fromLocalFile(xsdFile->fileName()));
		if(xmlSchema->isValid()) {
			QXmlSchemaValidator *validator=new QXmlSchemaValidator(*xmlSchema);
			
			if(validator->validate(docFile, QUrl::fromLocalFile(docFile->fileName()))) {
				ret=true;
			}
			
			delete validator;
		}
		
		delete xsdFile;
		delete xmlSchema;	
		
		return ret;
	}
	//------------------------------------------------------------------------------
	bool CReport::process(QFile *docFile) {
		QXmlSimpleReader *xmlReader=new QXmlSimpleReader();
		QXmlInputSource *source = new QXmlInputSource(docFile);
		CDocumentParser *parser=new CDocumentParser();
		bool ret=false;
		
		if(document != 0) {
			delete document;
		}
		xmlReader->setContentHandler(parser);
		xmlReader->setLexicalHandler(parser);
		if(xmlReader->parse(source)) {
			document=parser->getDocument();;
			
			if(document->process()) {
				ret=true;
			}else {
				lastSourceError=document->getLastSourceError();
				lastError=document->getLastError();
			}
		}else {
			lastSourceError=QObject::tr("Invalid file");
			lastError=QObject::tr("Unable to parse the file : ")+parser->errorString();
		}
		
		cleanup();
		
		delete parser;
		delete source;
		delete xmlReader;
		
		return ret;
	}
	//------------------------------------------------------------------------------
	int CReport::getNbPage(void) {
		if(document != 0) {
			return document->getNbPage();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	QString CReport::toSvg(int pageIdx) {
		if(document != 0 && pageIdx >= 0 && pageIdx < document->getNbPage()) {
			return document->getPage(pageIdx)->getSvg();
		}
		return "";
	}
	//------------------------------------------------------------------------------
	QSize CReport::getPagesSize(void) {
		if(document != 0) {
			return document->getPagesSize();
		}
		
		return QSize();
	}
	//------------------------------------------------------------------------------
	void CReport::renderPage(int pageIdx, QPainter *painter) {
		if(document != 0) {
		}
	}
	//------------------------------------------------------------------------------
	QTranslator * CReport::getTranslator(void) {
		if(translator == 0) {
			translator=new QTranslator();
			translator->load("libqt4report_"+QLocale::system().name(), QDir(QString(DATADIR)+"/"+QString(PACKAGE)).absolutePath());
		}
		return translator;
	}
	//------------------------------------------------------------------------------
	void CReport::cleanup(void) {
		if(document != 0) {
			document->cleanup();
		}
	}
	//------------------------------------------------------------------------------
} //namespace
//------------------------------------------------------------------------------