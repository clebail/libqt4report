//------------------------------------------------------------------------------
#ifndef __CDOCUMENT_H__
#define __CDOCUMENT_H__
//------------------------------------------------------------------------------
#include <QHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include "CDocBand.h"
#include "CFields.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CDocument {
		public:
			CDocument(QString pageWidth, QString pageHeight);
			~CDocument(void);
			int getNbPage(void);
			QString toSvg(int pageIdx);
			QString getLastError(void) { return lastError; }
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
		private:
			CDocBand *pageHeader;
			CDocBand *docHeader;
			CDocBand *docBody;
			CDocBand *docFooter;
			CDocBand *pageFooter;
			QSqlDatabase database;
			QString sqlQuery;
			QString lastError;
			QStringList pages;
			QString pageWidth, pageHeight;
			
			void createPages(QSqlQuery * query);
			void processFields(QSqlRecord *record) { CFields::getInstance()->process(record); }
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CDOCUMENT_H__
//------------------------------------------------------------------------------
