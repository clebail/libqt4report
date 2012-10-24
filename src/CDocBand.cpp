//------------------------------------------------------------------------------
#include <QtDebug>
#include "CDocBand.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	QString CDocBand::toSvg(int &y, double coef) {
		QString svg="<g>";
		QHashIterator<QString, CItem *> i(*this);
		
		while (i.hasNext()) {
			i.next();
			svg+=i.value()->toSvg(y, coef);
		}
		
		svg+="</g>";
		
		y+=getHeight(coef);
		
		return svg;
	}
	//------------------------------------------------------------------------------
	int CDocBand::getHeight(double coef) {
		if(height == 0) {
			QHashIterator<QString, CItem *> i(*this);
		
			while (i.hasNext()) {
				int iH;
				i.next();
			
				if((iH=i.value()->getHeight(coef)) > height) {
					height=iH;
				}
			}
		}
		
		return height;
	}
	//------------------------------------------------------------------------------
	void CDocBand::cleanup(void) {
		QHashIterator<QString, CItem *> i(*this);
		
		while (i.hasNext()) {
			i.next();
			delete i.value();
		}
	}
	//------------------------------------------------------------------------------
} //namespace