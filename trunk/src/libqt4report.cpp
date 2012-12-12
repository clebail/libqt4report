//------------------------------------------------------------------------------
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QAbstractMessageHandler>
#include <QBuffer>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QtDebug>
#include <QDir>
#include <QApplication>
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
	static log4cpp::Category& logger = log4cpp::Category::getInstance("CReport");
	//------------------------------------------------------------------------------
	CReport::CReport(QString connectionName, bool forceReload) : QObject() {
		log4cpp::PropertyConfigurator::configure(QDir::toNativeSeparators(QString(DATADIR)+"/"+QString(PACKAGE)+"/log4cpp.properties").toStdString());
		
		#ifdef WIN32
			QApplication::addLibraryPath(QApplication::applicationDirPath());
		#endif //WIN32
		
		qRegisterMetaType<CItemTextFixedObject>("CItemTextFixedObject");
		qRegisterMetaType<CItemTextFieldObject>("CItemTextFieldObject");
		
		qRegisterMetaType<CDbFieldObject>("CDbFieldObject");
		qRegisterMetaType<CItemLineObject>("CItemLineObject");
		qRegisterMetaType<CItemRectObject>("CItemRectObject");
		qRegisterMetaType<CCalculatedFieldObject>("CCalculatedFieldObject");
		qRegisterMetaType<CTotalFieldObject>("CTotalFieldObject");
		
		qRegisterMetaType<CValueTypeString>("CValueTypeString");
		qRegisterMetaType<CValueTypeInteger>("CValueTypeInteger");
		qRegisterMetaType<CValueTypeReal>("CValueTypeReal");
		qRegisterMetaType<CValueTypeDate>("CValueTypeDate");
		qRegisterMetaType<CValueTypeDateTime>("CValueTypeDateTime");
		
		this->connectionName=connectionName;
		this->forceReload=forceReload;
		
		document=0;
	}
	//------------------------------------------------------------------------------
	CReport::~CReport(void) {
		logger.debug("Delete CReport instance");
		if(document != 0) {
			delete document;
		}
	}
	//------------------------------------------------------------------------------
	bool CReport::validDocument(QFile *docFile) {
		QXmlSchema *xmlSchema=new QXmlSchema();
		QFile *xsdFile=new QFile((QString(DATADIR)+"/"+QString(PACKAGE)+"/schema/libqt4report.xsd"));
		xsdFile->open(QIODevice::ReadOnly);
		bool ret=false;
		
		logger.debug("Start validating report");
		
		xmlSchema->load(xsdFile, QUrl::fromLocalFile(xsdFile->fileName()));
		if(xmlSchema->isValid()) {
			QXmlSchemaValidator *validator=new QXmlSchemaValidator(*xmlSchema);
			
			if(validator->validate(docFile, QUrl::fromLocalFile(docFile->fileName()))) {
				ret=true;
			}
			
			delete validator;
		}

		logger.debug((QString("Validating result : ")+(ret ? "OK" : "NOK")).toStdString());
		
		delete xsdFile;
		delete xmlSchema;	
		
		return ret;
	}
	//------------------------------------------------------------------------------
	bool CReport::process(QFile *docFile) {
		QXmlSimpleReader *xmlReader=new QXmlSimpleReader();
		QXmlInputSource *source = new QXmlInputSource(docFile);
		CDocumentParser *parser=new CDocumentParser(connectionName);
		bool ret=false;
		
		logger.debug("Start process report");
		connect(parser, SIGNAL(queryParam(QString,QVariant&)), this, SLOT(onParserQueryParam(QString,QVariant&)));
		
		if(document != 0) {
			delete document;
			document=0;
		}
		
		if(!forceReload) {
			QFile f("/home/corentin/file.dat");
			if(f.open(QIODevice::ReadOnly)) {
				QDataStream in(&f);
				document=CDocument::fromCache(in);
				
				f.close();
			}
		}
		
		if(document == 0) {
			logger.debug("Start parse report file");
			xmlReader->setContentHandler(parser);
			xmlReader->setLexicalHandler(parser);
			if(xmlReader->parse(source)) {
				document=parser->getDocument();
				
				QHashIterator<QString, QVariant> i(params);
				while (i.hasNext()) {
					i.next();
					logger.debug((tr("Param")+" "+i.key()+" = "+i.value().toString()).toStdString());
					document->setParamValue(i.key(), i.value());
				}
				
				if(document->process()) {
					ret=true;
				}else {
					lastSourceError=document->getLastSourceError();
					lastError=document->getLastError();
				}
			}else {
				lastSourceError=QObject::tr("Invalid file");
				lastError=QObject::tr("Unable to parse the file : ")+parser->errorString();
				logger.debug(("Unable to parse the file : "+parser->errorString()).toStdString());
			}
			
			QFile f("/home/corentin/file.dat");
			if(f.open(QIODevice::WriteOnly)) {
				QDataStream out(&f);
				document->serialize(out);
					
				f.close();
			}
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
			return document->count();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	QString CReport::toSvg(int pageIdx) {
		if(document != 0 && pageIdx >= 0 && pageIdx < document->count()) {
			return document->at(pageIdx)->getSvg();
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
			CPage *page=document->at(pageIdx);
			if(page != 0) {
				int i;
				QList<CRendererObject *> * rendererObjects=page->getRendererObjects();
				
				for(i=0;i<rendererObjects->count();i++) {
					rendererObjects->at(i)->draw(painter);
				}
			}
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
	void CReport::onParserQueryParam(QString paramName, QVariant& value) {
		logger.debug(("Emit query param for "+paramName).toStdString());
		emit queryParam(paramName, value);
	}
	//------------------------------------------------------------------------------
} //namespace
//------------------------------------------------------------------------------