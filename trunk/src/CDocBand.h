//------------------------------------------------------------------------------
#ifndef __CDOCBAND_H__
#define __CDOCBAND_H__
//------------------------------------------------------------------------------
#include <QHash>
#include <QSqlRecord>
#include "CPrintableObject.h"
#include "CItem.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CDocBand : public CPrintableObject, public QHash<QString, CItem *> {
		public:
			QString toSvg(int y, double coef);
			int getHeight(double coef);
			void cleanup(void);
			void prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef);
		private:
			int height;
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CDOCBAND_H__
//------------------------------------------------------------------------------