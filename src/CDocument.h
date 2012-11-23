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
#include "CGroup.h"
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
			CDocBand * createPageHeader(void) { return pageHeader=new CDocBand(); }
			CDocBand * createDocHeader(void) { return docHeader=new CDocBand(); }
			CDocBand * createDocBody(void) { return docBody=new CDocBand(); }
			CDocBand * createDocFooter(void) { return docFooter=new CDocBand(); }
			CDocBand * createPageFooter(void) { return pageFooter=new CDocBand(); }
			void cleanup(void);
			QSize getPagesSize(void) { return pagesSize; }
			void setParamValue(QString paramName, QVariant value) { params[paramName]=value; }
			void addGroupBand(CGroup *group, CDocBand *docBand) { groupBands->insert(group, docBand); }
			CDocBand * getGroupBand(CGroup *group) { return groupBands->value(group); }
		private:
			CDocBand *pageHeader;
			CDocBand *docHeader;
			CDocBand *docBody;
			CDocBand *docFooter;
			CDocBand *pageFooter;
			QHash<CGroup *, CDocBand *> *groupBands;
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
