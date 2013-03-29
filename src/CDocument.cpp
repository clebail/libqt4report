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
		logger.debug("Delete CDocument instance");
		
		
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
		
		for(int i=egbHeader;i<=egbFooter;i++) {
			QHashIterator<CGroup *, CDocBand *> ite(*groupBands[i]);
			while (ite.hasNext()) {
				ite.next();
				ite.value()->cleanup();
				delete ite.value();
			}
			groupBands[i]->clear();
		}
		
		CGroups::getInstance()->cleanup();
		
		for(int i=0;i<count();i++) {
			delete at(i);
		}
		
		clear();
	}
	//------------------------------------------------------------------------------
	void CDocument::serialize(QDataStream &out) {
		QHashIterator<QString, QVariant> i(params);
		logger.debug("Serialize document");
		
		out << pageWidth;
		out << pageHeight;
		out << unit;
		
		out << database.driverName();
		out << database.hostName();
		out << database.userName();
		out << database.password();
		out << database.databaseName();
		
		out << sqlQuery;
		
		out << (qint32)params.size();
		while(i.hasNext()) {
			i.next();
			out << i.key();
		}
		
		CFonts::getInstance()->serialize(out);
		CGroups::getInstance()->serialize(out);
		CFields::getInstance()->serialize(out);
		
		if(pageHeader != 0) {
			logger.debug("Serialize pageHeader");
			pageHeader->serialize(out);
		}else {
			out << qint32(0);
		}
		
		if(docHeader != 0) {
			logger.debug("Serialize docHeader");
			docHeader->serialize(out);
		}else {
			out << qint32(0);
		}
		
		logger.debug("Serialize docBody");
		docBody->serialize(out);
		
		if(docFooter != 0) {
			logger.debug("Serialize docFooter");
			docFooter->serialize(out);
		}else {
			out << qint32(0);
		}
		
		if(pageFooter != 0) {
			logger.debug("Serialize pageFooter");
			pageFooter->serialize(out);
		}else {
			out << qint32(0);
		}
		
		serializeGroupBands(out);
	}
	//------------------------------------------------------------------------------
	CDocument * CDocument::fromCache(QDataStream &in, QString reportPath, QString connectionName) {
		qint32 docBandSize;
		qint32 fieldCount;
		qint32 fontCount;
		qint32 paramCount;
		qint32 groupCount;
		qint32 i;
		QString pageWidth;
		QString pageHeight;
		QString unit;
		CDocument *document;
		QString driverName;
		QString hostName;
		QString userName;
		QString password;
		QString databaseName;
		QString sqlQuery;
		
		logger.debug("Create document from cache");
		
		in >> pageWidth;
		in >> pageHeight;
		in >> unit;
		
		in >> driverName;
		in >> hostName;
		in >> userName;
		in >> password;
		in >> databaseName;
		
		logger.debug((driverName+" "+hostName+" "+userName+" "+password+" "+databaseName).toStdString());
		
		document=new CDocument(pageWidth, pageHeight, unit, connectionName, reportPath);
		document->setDatabaseInfos(driverName, hostName, userName, password, databaseName);
		
		in >> sqlQuery;
		document->setQuery(sqlQuery);
		
		in >> paramCount;
		for(i=0;i<paramCount;i++) {
			QString key;
			
			in >> key;
			document->setParamValue(key, QVariant());
		}
		
		in >> fontCount;
		CFonts::getInstance()->fromCache(in, fontCount);
		
		in >> groupCount;
		CGroups::getInstance()->fromCache(in, groupCount);
		
		in >> fieldCount;
		CFields::getInstance()->fromCache(in, fieldCount);
		
		in >> docBandSize;
		if(docBandSize != 0) {
			logger.debug("PageHeader from cache");
			document->createPageHeader()->fromCache(in, docBandSize);
		}
		
		in >> docBandSize;
		if(docBandSize != 0) {
			logger.debug("DocHeader from cache");
			document->createDocHeader()->fromCache(in, docBandSize);
		}
		
		in >> docBandSize;
		logger.debug("DocBody from cache");
		document->createDocBody()->fromCache(in, docBandSize);
		
		in >> docBandSize;
		if(docBandSize != 0) {
			logger.debug("DocFooter from cache");
			document->createDocFooter()->fromCache(in, docBandSize);
		}
		
		in >> docBandSize;
		if(docBandSize != 0) {
			logger.debug("PageFooter from cache");
			document->createPageFooter()->fromCache(in, docBandSize);
		}
		
		groupBandsFromCache(document, in);
		
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
	void CDocument::serializeGroupBands(QDataStream &out) {
		int i;
		for(i=egbHeader;i<=egbFooter;i++) {
			QHashIterator<CGroup *, CDocBand *> it(*groupBands[i]);
			
			out << groupBands[i]->size();
			while(it.hasNext()) {
				it.next();
				out << it.key()->getId();
				it.value()->serialize(out);
			}
		}
	}
	//------------------------------------------------------------------------------
	void CDocument::groupBandsFromCache(CDocument *document, QDataStream &in) {
		int i;
		for(i=egbHeader;i<=egbFooter;i++) {
			int groupBandSize;
			int j;
			
			in >> groupBandSize;
			for(j=0;j<groupBandSize;j++) {
				QString groupId;
				int docBandSize;
				CDocBand *docBand=new CDocBand();
				
				in >> groupId;
				in >> docBandSize;
				
				logger.debug((QString("Fill group band ")+groupId+QString(" from cache")).toStdString());
				
				docBand->fromCache(in, docBandSize);
				document->addGroupBand(CGroups::getInstance()->getGroup(groupId), (CDocument::EGBType)i, docBand);
			}
		}
	}
	//------------------------------------------------------------------------------
}