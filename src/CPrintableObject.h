//------------------------------------------------------------------------------
#ifndef __CPRINTABLEOBJECT_H__
#define __CPRINTABLEOBJECT_H__
//------------------------------------------------------------------------------
#include <QList>
#include <QString>
#include "CRendererObject.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CPrintableObject {
		public:
			virtual ~CPrintableObject(void) {}
			virtual QString toSvg(int y, double coef) = 0;
			virtual void prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef) = 0;
			QString xmlEncode(QString src) { return src.replace("&", "&amp;").replace("\"", "&quot;").replace("'", "&apos;").replace("<", "&lt;").replace(">", "&gt;"); }
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CPRINTABLEOBJECT_H__
//------------------------------------------------------------------------------