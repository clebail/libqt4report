//------------------------------------------------------------------------------
#ifndef __CREPORT_H__
#define __CREPORT_H__
//------------------------------------------------------------------------------
#include <QFile>
#include <QTranslator>
#include <QSize>
#include <QPainter>
//------------------------------------------------------------------------------
namespace libqt4report {
	class CReport {
		public:
			CReport(void);
			~CReport(void);
			bool validDocument(QFile *docFile);
			bool process(QFile *docFile);
			int getNbPage(void);
			QString toSvg(int pageIdx);
			QString getLastError(void) { return lastError; }
			QString getLastSourceError(void) { return lastSourceError; }
			QSize getPagesSize(void);
			void renderPage(int pageIdx, QPainter *painter);
			
			static QTranslator * getTranslator(void);
		private:
			QString lastError;
			QString lastSourceError;
			
			void cleanup(void);
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CREPORT_H__
//------------------------------------------------------------------------------