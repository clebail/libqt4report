//------------------------------------------------------------------------------
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>
#include "CDocument.h"
#include "CFonts.h"
#include "CScript.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	CDocument::CDocument(QString pageWidth, QString pageHeight, QString unit) {
		pageHeader=docHeader=docBody=docFooter=pageFooter=0;
		
		this->pageWidth=pageWidth;
		this->pageHeight=pageHeight;
		setUnit(unit);
	}
	//------------------------------------------------------------------------------
	CDocument::~CDocument(void) {
		pages.clear();
	}
	//------------------------------------------------------------------------------
	int CDocument::getNbPage(void) {
		return pages.size();;
	}
	//------------------------------------------------------------------------------
	QString CDocument::toSvg(int pageIdx) {
		if(pageIdx >= 0 &&  pageIdx < pages.size()) {
			return pages.at(pageIdx);
		}
		return "";
	}
	//------------------------------------------------------------------------------
	void CDocument::setDatabaseInfos(QString driver, QString host, QString userName, QString password, QString dbName) {
		database=QSqlDatabase::addDatabase(driver);
		database.setHostName(host);
		database.setUserName(userName);
		database.setPassword(password);
		database.setDatabaseName(dbName);
	}
	//------------------------------------------------------------------------------
	bool CDocument::process(void) {
		if(!database.open()) {
			lastError=database.lastError().databaseText();
			
			return false;
		}
		
		QSqlQuery query(sqlQuery, database);
		if(!query.exec()) {
			lastError=query.lastError().databaseText();
			
			return false;
		}
		
		try {
			createPages(&query);
		}catch(QString *e) {
			lastError=*e;
			
			return false;
		}
		query.clear();
		database.close();
		
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
	}
	//------------------------------------------------------------------------------
	void CDocument::createPages(QSqlQuery *query) {
		QString w=QString::number(pageWidth.toInt()*coef);
		int hPage=0;
		int y;
		int hDocBody, hDocFooter=0, hPageFooter=0;
		int hFooter;
		bool fini;
		bool nouvellePage=true, finPage=false;
		QString svg;
		int idxRec, lastRec;
		bool hSpecified=false;
		
		if(pageHeight != "100%") {
			hSpecified=true;
			hPage=pageHeight.toInt()*coef;
		}
		
		hDocBody=docBody->getHeight(coef);
		
		if(docFooter != 0) {
			hDocFooter=docFooter->getHeight(coef);
		}
		
		if(pageFooter != 0) {
			hPageFooter=pageFooter->getHeight(coef);
		}
		
		hFooter=hPageFooter;
		
		lastRec=query->size()-1;
		idxRec=0;
		fini=(lastRec == -1);
		
		while(!fini) {
			query->next();
			QSqlRecord record=query->record();
			
			processFields(&record);
			
			if(nouvellePage) {
				svg="<?xml version='1.0' encoding='utf-8'?>";
				svg+="<svg xmlns='http://www.w3.org/2000/svg' version='1.2' ";
				svg+="baseProfile='tiny' width='"+w+"' ";
				svg+="height='${height}'>";
				svg+="<rect x='0' y='0' width='"+w+"' height='${height}' fill='white' stroke='none' />";
			
				y=0;
				
				if(pageHeader != 0) {
					svg+=pageHeader->toSvg(y, coef);
				}
				
				if(idxRec == 0 && docHeader != 0) {
					svg+=docHeader->toSvg(y, coef);
				}
				
				nouvellePage=false;
			}
			svg+=docBody->toSvg(y, coef);
			
			hFooter=hPageFooter + (idxRec == lastRec-1 ? hDocFooter : 0);
			
			if(idxRec == lastRec || (hSpecified && hPage - y - hFooter - hDocBody < 0)) {
				finPage=true;
			}
			
			if(finPage) {
				if(idxRec == lastRec) {
					if(docFooter != 0) {
						svg+=docFooter->toSvg(y, coef);
					}
					fini=true;
				}else {
					nouvellePage=true;
				}
				
				if(pageFooter != 0) {
					svg+=pageFooter->toSvg(y, coef);
				}
				
				svg+="</svg>";
				svg.replace("${height}", QString::number(hSpecified ? hPage : y));
				
				pages.append(svg);
				
				//qDebug() << svg;
				
				finPage=false;
			}
			
			idxRec++;
		}
		
	}
	//------------------------------------------------------------------------------
}