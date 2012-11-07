//------------------------------------------------------------------------------
#include <QRegExp>
#include <QtDebug>
#include "CValueType.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	QString CValueTypeReal::toFormatedString(QString format) {
		QChar f='g';
		int precision=-1;
		
		if(!format.isEmpty()) {
			QRegExp regExp("r([eEfgG]?)([0-9]*)");
			regExp.indexIn(format);
				
			if(!regExp.cap(1).isEmpty()) {
				f=regExp.cap(1).at(0);
			}
			
			if(!regExp.cap(2).isEmpty()) {
				precision=regExp.cap(2).toInt();
			}
		}

		return QString("%L1").arg(value, 0, f.toAscii(), precision);
	}
	//------------------------------------------------------------------------------
	QString CValueTypeDate::toFormatedString(QString format) {
		if(format.isEmpty()) {
			return value.toString(Qt::SystemLocaleShortDate);
		}
		
		return value.toString(format.mid(1));
	}
	//------------------------------------------------------------------------------
	QString CValueTypeDateTime::toFormatedString(QString format) {
		if(format.isEmpty()) {
			return value.toString(Qt::SystemLocaleShortDate);
		}
		
		return value.toString(format.mid(1));
	}
	//------------------------------------------------------------------------------
}//namespace
//------------------------------------------------------------------------------