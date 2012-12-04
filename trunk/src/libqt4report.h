//------------------------------------------------------------------------------
#ifndef __CREPORT_H__
#define __CREPORT_H__
//------------------------------------------------------------------------------
#include <QFile>
#include <QTranslator>
#include <QSize>
#include <QPainter>
#include <QHash>
#include <QVariant>
//------------------------------------------------------------------------------
namespace libqt4report {
	class CReport : public QObject {
		Q_OBJECT
		public:
			CReport(QString connectionName=QString(), bool forceReload=true);
			~CReport(void);
			bool validDocument(QFile *docFile);
			bool process(QFile *docFile);
			int getNbPage(void);
			QString toSvg(int pageIdx);
			QString getLastError(void) { return lastError; }
			QString getLastSourceError(void) { return lastSourceError; }
			QSize getPagesSize(void);
			void renderPage(int pageIdx, QPainter *painter);
			void setParamValue(QString paramName, QVariant value) { params[paramName]=value; }
			
			static QTranslator * getTranslator(void);
		private:
			QString lastError;
			QString lastSourceError;
			QString connectionName;
			QHash<QString, QVariant> params;
			bool forceReload;
			
			void cleanup(void);
		private slots:
			void onParserQueryParam(QString paramName, QVariant& value);
		signals:
			void queryParam(QString paramName, QVariant& value);
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CREPORT_H__
//------------------------------------------------------------------------------