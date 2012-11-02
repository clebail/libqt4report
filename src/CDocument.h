//------------------------------------------------------------------------------
#ifndef __CDOCUMENT_H__
#define __CDOCUMENT_H__
//------------------------------------------------------------------------------
#include <QHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QSize>
#include "CDocBand.h"
#include "CFields.h"
#include "CPage.h"
//------------------------------------------------------------------------------
#define COEF_MM		3.515625 	//90 dpi 1 pouce = 25.6 mm => 90/25.6
#define COEF_IN		90.0		//90 dpi
#define COEF_PX		1	
//------------------------------------------------------------------------------
namespace libqt4report {
	class CDocument {
		public:
			CDocument(QString pageWidth, QString pageHeight, QString unit, QString connectionName);
			~CDocument(void);
			int getNbPage(void);
			CPage *getPage(int pageIdx);
			QString getLastError(void) { return lastError; }
			QString getLastSourceError(void) { return lastSourceError; }
			void setDatabaseInfos(QString driver, QString host, QString userName, QString password, QString dbName);
			void setQuery(QString sqlQuery) { this->sqlQuery=sqlQuery.trimmed(); }
			bool process(void);
			void createPageHeader(void) { pageHeader=new CDocBand(); }
			void createDocHeader(void) { docHeader=new CDocBand(); }
			void createDocBody(void) { docBody=new CDocBand(); }
			void createDocFooter(void) { docFooter=new CDocBand(); }
			void createPageFooter(void) { pageFooter=new CDocBand(); }
			CDocBand * getPageHeader(void) { return pageHeader; }
			CDocBand * getDocHeader(void) { return docHeader; }
			CDocBand * getDocBody(void) { return docBody; }
			CDocBand * getDocFooter(void) { return docFooter; }
			CDocBand * getPageFooter(void) { return pageFooter; }
			void cleanup(void);
			QSize getPagesSize(void) { return pagesSize; }
			void setParamValue(QString paramName, QVariant value) { params[paramName]=value; }
		private:
			CDocBand *pageHeader;
			CDocBand *docHeader;
			CDocBand *docBody;
			CDocBand *docFooter;
			CDocBand *pageFooter;
			QSqlDatabase database;
			QString sqlQuery;
			QString lastError;
			QString lastSourceError;
			QList<CPage *> pages;
			QString pageWidth, pageHeight;
			double coef;
			QSize pagesSize;
			QString connectionName;
			QHash<QString, QVariant> params;
			
			void createPages(QSqlQuery * query);
			void processFields(QSqlRecord *record) { CFields::getInstance()->process(record); }
			void setUnit(QString unit) { coef=(unit == "mm" ? COEF_MM : (unit == "in" ? COEF_IN : COEF_PX)); }
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CDOCUMENT_H__
//------------------------------------------------------------------------------
