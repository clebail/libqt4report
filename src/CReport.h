//------------------------------------------------------------------------------
#ifndef __CREPORT_H__
#define __CREPORT_H__
//------------------------------------------------------------------------------
#include <QFile>
#include <QTranslator>
#include <QSize>
//------------------------------------------------------------------------------
namespace libqt4report {
	typedef struct _SPage {
		QString svg;
		QSize size;
	}SPage;
	//------------------------------------------------------------------------------
	class CReport {
		public:
			CReport(void);
			~CReport(void);
			bool validDocument(QFile *docFile);
			bool process(QFile *docFile);
			int getNbPage(void);
			SPage * getPage(int pageIdx);
			QString getLastError(void) { return lastError; }
			QString getLastSourceError(void) { return lastSourceError; }
			QTranslator * getTranslator(void) { return translator; }
		private:
			QString lastError;
			QString lastSourceError;
			QTranslator *translator;
			
			void cleanup(void);
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CREPORT_H__
//------------------------------------------------------------------------------