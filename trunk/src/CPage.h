//------------------------------------------------------------------------------
#ifndef __CPAGE_H__
#define __CPAGE_H__
//------------------------------------------------------------------------------
#include <QString>
#include <QList>
#include "CPrintableObject.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CPage {
		public:
			CPage(void);
			~CPage(void);
			QString getSvg(void) { return svg; }
			void setSvg(QString svg) { this->svg=svg; }
			QList<CRendererObject *> * getRendererObjects(void) { return rendererObjects; }
		private:
			QString svg;
			QList<CRendererObject *> *rendererObjects;
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CPAGE_H__
//------------------------------------------------------------------------------