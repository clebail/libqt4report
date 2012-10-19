//------------------------------------------------------------------------------
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QAbstractMessageHandler>
#include <QBuffer>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QtDebug>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include "CReport.h"
#include "CDocumentParser.h"
#include "CDocument.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	//------------------------------------------------------------------------------
	static CDocument * document;
	//------------------------------------------------------------------------------
	CReport::CReport(void) {
		log4cpp::PropertyConfigurator::configure((QString(DATADIR)+"/log4cpp.properties").toStdString());
		document=0;
	}
	//------------------------------------------------------------------------------
	CReport::~CReport(void) {
		if(document != 0) {
			delete document;
		}
	}
	//------------------------------------------------------------------------------
	bool CReport::validDocument(QFile *docFile) {
		QXmlSchema *xmlSchema=new QXmlSchema();
		QFile *xsdFile=new QFile(QString(DATADIR)+"/schema/libqt4report.xsd");
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
				lastError=document->getLastError();
			}
		}else {
			lastError="Unable to parse the file : "+parser->errorString();
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
			return document->toSvg(pageIdx);
		}
		return "";
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