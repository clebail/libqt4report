//------------------------------------------------------------------------------
#include <QDebug>
#include <QMetaEnum>
#include "CFont.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	//------------------------------------------------------------------------------
	void CFont::setWeight(QString sWeight) {
		QMetaEnum metaEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Weight"));
		
		weight=(Weight)metaEnum.keyToValue(sWeight.toUtf8().data());
	}
	//------------------------------------------------------------------------------
	void CFont::setStyle(QString sStyle) {
		QMetaEnum metaEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Style"));
		
		style=(Style)metaEnum.keyToValue(sStyle.toUtf8().data());
	}
	//------------------------------------------------------------------------------
	void CFont::toQFont(QFont &font) {
		font=QFont(family, size, (QFont::Weight)weight);
		font.setStyle((QFont::Style)style);
	}
	//------------------------------------------------------------------------------
	QString CFont::toSvgWeight(Weight weight) {
		switch(weight) {
			case Light:
				return "lighter";
			case Normal:
				return "normal";
			case DemiBold:
				return "bold";
			case Bold:
				return "bolder";
			case Black:
				return "900";
		}
	}
	//------------------------------------------------------------------------------
	QString CFont::toSvgStyle(Style style) {
		switch(style) {
			case StyleNormal:
				return "normal";
			case StyleItalic:
				return "italic";
			case StyleOblique:
				return "oblique";
		}
	}
	//------------------------------------------------------------------------------
}// namespace
//------------------------------------------------------------------------------