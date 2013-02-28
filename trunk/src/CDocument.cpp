//------------------------------------------------------------------------------
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <log4cpp/Category.hh>
#include "CDocument.h"
#include "CFonts.h"
#include "CScript.h"
#include "CGroups.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	//------------------------------------------------------------------------------
	static log4cpp::Category& logger = log4cpp::Category::getInstance("CDocument");
	static QString reportPath;
	//------------------------------------------------------------------------------
	CDocument::CDocument(QString pageWidth, QString pageHeight, QString unit, QString connectionName, QString _reportPath) : QList<CPage *>() {
		logger.debug("Create CDocument instance");
		
		pageHeader=docHeader=docBody=docFooter=pageFooter=0;

		groupBands[egbHeader]=new QHash<CGroup *, CDocBand *>();
		groupBands[egbFooter]=new QHash<CGroup *, CDocBand *>();
		
		this->pageWidth=pageWidth;
		this->pageHeight=pageHeight;
		this->unit=unit;
		this->connectionName=connectionName;
		reportPath=_reportPath;
		setUnit(unit);
	}
	//------------------------------------------------------------------------------
	CDocument::~CDocument(void) {
		int i;
		
		logger.debug("Delete CDocument instance");
		
		for(i=0;i<count();i++) {
			delete at(i);
		}
		
		clear();
		
		delete groupBands[egbHeader];
		delete groupBands[egbFooter];
	}
	//------------------------------------------------------------------------------
	void CDocument::setDatabaseInfos(QString driver, QString host, QString userName, QString password, QString dbName) {
		logger.debug("Set database info");
		
		if(connectionName.isEmpty()) {
			logger.debug("Embedded database info");
			database=QSqlDatabase::addDatabase(driver);
			database.setHostName(host);
			database.setUserName(userName);
			database.setPassword(password);
			database.setDatabaseName(dbName);
		}else {
			logger.debug("Extern database info");
			database=QSqlDatabase::database(connectionName);
		}
	}
	//------------------------------------------------------------------------------
	bool CDocument::process(void) {
		logger.debug("Start process document");
		
		if(connectionName.isEmpty()) {
			if(!database.open()) {
				lastSourceError=QObject::tr("Database error");
				lastError=database.lastError().databaseText();
			
				logger.debug((QString("Database error ")+lastError).toStdString());
				
				return false;
			}
		}
		
		QSqlQuery query(database);
		query.prepare(sqlQuery);
		
		QHashIterator<QString, QVariant> i(params);
		while (i.hasNext()) {
			i.next();
			query.bindValue(":"+i.key(), i.value());
		}
		
		if(!query.exec()) {
			lastSourceError=QObject::tr("Database error");
			lastError=query.lastError().databaseText();
			
			return false;
		}
		
		try {
			createPages(&query);
		}catch(QString *e) {
			lastSourceError=QObject::tr("Processing error");
			lastError=*e;
			
			return false;
		}
		
		query.clear();
		
		if(connectionName.isEmpty()) {
			database.close();
		}
		
		return true;
	}
	//------------------------------------------------------------------------------
	void CDocument::cleanup(void) {
		CGroups::getInstance()->cleanup();
		
		if(pageHeader != 0) {
			pageHeader->cleanup();
			delete pageHeader;
		}
		
		if(docHeader != 0) {
			docHeader->cleanup();
			delete docHeader;
		}
		
		if(docFooter != 0) {
			docFooter->cleanup();
			delete docFooter;
		}
		
		if(pageFooter != 0) {
			pageFooter->cleanup();
			delete pageFooter;
		}
		
		docBody->cleanup();
		delete docBody;
		
		CFonts::getInstance()->cleanup();
		CFields::getInstance()->cleanup();
		CScript::getInstance()->cleanup();
		
		for(int i=0;i<2;i++) {
			QHashIterator<CGroup *, CDocBand *> ite(*groupBands[i]);
			while (ite.hasNext()) {
				ite.next();
				delete ite.value();
			}
			groupBands[i]->clear();
		}
	}
	//------------------------------------------------------------------------------
	void CDocument::serialize(QDataStream &out) {
		
		logger.debug("Serialize document");
		
		out << pageWidth;
		out << pageHeight;
		out << unit;
		out << connectionName;
		
		CFields::getInstance()->serialize(out);
		
		if(pageHeader != 0) {
			pageHeader->serialize(out);
		}else {
			out << qint32(0);
		}
		
		if(docHeader != 0) {
			docHeader->serialize(out);
		}else {
			out << qint32(0);
		}
		
		docBody->serialize(out);
		
		if(docFooter != 0) {
			docFooter->serialize(out);
		}else {
			out << qint32(0);
		}
		
		if(pageFooter != 0) {
			pageFooter->serialize(out);
		}else {
			out << qint32(0);
		}
	}
	//------------------------------------------------------------------------------
	CDocument * CDocument::fromCache(QDataStream &in, QString reportPath) {
		qint32 docBandSize;
		qint32 fieldsCount;
		QString pageWidth;
		QString pageHeight;
		QString unit;
		QString connectionName;
		CDocument *document;
		
		logger.debug("Create document from cache");
		
		in >> pageWidth;
		in >> pageHeight;
		in >> unit;
		in >> connectionName;
		
		document=new CDocument(pageWidth, pageHeight, unit, connectionName, reportPath);
		
		
		in >> fieldsCount;
		CFields::getInstance()->fromCache(in, fieldsCount);
		
		in >> docBandSize;
		if(docBandSize != 0) {
			logger.debug("Serialize PageHeader");
			document->createPageHeader()->fromCache(in, docBandSize);
		}
		
		in >> docBandSize;
		if(docBandSize != 0) {
			logger.debug("Serialize DocHeader");
			document->createDocHeader()->fromCache(in, docBandSize);
		}
		
		in >> docBandSize;
		logger.debug("Serialize DocBody");
		document->createDocBody()->fromCache(in, docBandSize);
		
		in >> docBandSize;
		if(docBandSize != 0) {
			logger.debug("Serialize DocFooter");
			document->createDocFooter()->fromCache(in, docBandSize);
		}
		
		in >> docBandSize;
		if(docBandSize != 0) {
			logger.debug("Serialize PageFooter");
			document->createPageFooter()->fromCache(in, docBandSize);
		}
		
		return document;
	}
	//------------------------------------------------------------------------------
	QString CDocument::getReportPath(void) { 
		return reportPath;
	}
	//------------------------------------------------------------------------------
	void CDocument::createPages(QSqlQuery *query) {
		CPageManager *pageManager=new CPageManager(this);
		QSqlRecord record[2];
		bool fini=false;
		int lastRec=query->size()-1;
		int idxRec=0;
		CGroup *pGroup;
		CGroup *groupChanged=CGroups::getInstance()->getFirstGroup();
		
		if(query->next()) {
			record[1]=QSqlRecord();
			record[0]=query->record();
			
			processFields(&record[0]);
			
			while(!fini) {
				if(groupChanged != 0) {
					pGroup=groupChanged;
					while(pGroup) {
						
						logger.debug((QString("process ")+pGroup->getRefer()+QString(" header")).toStdString());
						
						pageManager->process(getGroupBand(pGroup, egbHeader), idxRec == 0, idxRec == lastRec);
						pGroup->setChanged(false);
						pGroup=pGroup->getChild();
					}
					groupChanged=0;
				}
				
				pageManager->process(docBody, idxRec == 0, idxRec == lastRec);
				
				record[1]=record[0];
				
				if(query->next()) {
					idxRec++;
					record[0]=query->record();
					
					groupChanged=testGroupChanged(&record[0], &record[1]);
					if(groupChanged != 0) {
						bool printGroupFooterFini=false;
						pGroup=CGroups::getInstance()->getLastGroup();
						do {
							printGroupFooterFini = (pGroup == groupChanged);
							
							logger.debug((QString("process ")+pGroup->getRefer()+QString(" footer")).toStdString());
							
							pageManager->process(getGroupBand(pGroup, egbFooter), idxRec == 0, idxRec == lastRec);
							pGroup->setChanged(true);
							pGroup=pGroup->getParent();
						}while(!printGroupFooterFini);
					}
					
					processFields(&record[0]);
				}else {
					fini=true;
				}
			}
			
			pGroup=CGroups::getInstance()->getLastGroup();
			while(pGroup) {
				pageManager->process(getGroupBand(pGroup, egbFooter), idxRec == 0, idxRec == lastRec);
				pGroup=pGroup->getParent();
			}	
			
			pageManager->end();
		}
		
		pagesSize=pageManager->getPageSize();
		
		delete pageManager;
	}
	//------------------------------------------------------------------------------
	CGroup * CDocument::testGroupChanged(QSqlRecord *rec0, QSqlRecord *rec1) {
		CGroup *pGroup=CGroups::getInstance()->getFirstGroup();
		
		while(pGroup != 0) {
			QString fieldName=CFields::getInstance()->getField(pGroup->getRefer())->getAttribute("fieldName");
			
			if(rec0->value(fieldName) != rec1->value(fieldName)) {
				logger.debug((QString("group ")+pGroup->getRefer()+QString(" changed")).toStdString());
				return pGroup;
			}
			
			pGroup=pGroup->getChild();
		}
		
		return 0;
	}
	//------------------------------------------------------------------------------
}