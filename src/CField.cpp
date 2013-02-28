//------------------------------------------------------------------------------
#include <QVariant>
#include <QtDebug>
#include <QRegExp>
#include <QDate>
#include <log4cpp/Category.hh>
#include "CField.h"
#include "CFields.h"
#include "CScript.h"
#include "CGroups.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	//------------------------------------------------------------------------------
	static log4cpp::Category& logger = log4cpp::Category::getInstance("CField");
	//------------------------------------------------------------------------------
	void CField::processAttributes(const QXmlAttributes& atts) {
		int i;
		
		for(i=0;i<atts.count();i++) {
			setAttribute(atts.localName(i), atts.value(i));
		}
	}
	//------------------------------------------------------------------------------
	void CField::serialize(QDataStream &out) {
		out << attributes;
	}
	//------------------------------------------------------------------------------
	void CField::fromCache(QDataStream &in) {
		QHash<QString, QString> hash;
		
		in >> hash;
		logger.debug((QString::number(hash.size())+" attributs filled").toStdString());
		//processAttributes((const QHash<QString, QString>&) hash);
	}
	//------------------------------------------------------------------------------
	void CDbFieldObject::process(QSqlRecord *record) {
		QString fieldName=getAttribute("fieldName");

		setFieldValue(record->value(fieldName));
	}
	//------------------------------------------------------------------------------
	void CDbFieldObject::serialize(QDataStream &out) {
		out << QString("CDbFieldObject");
		CField::serialize(out);
	}
	//------------------------------------------------------------------------------
	void CCalculatedFieldObject::process(QSqlRecord *record) {
		setFieldValue(CScript::getInstance()->eval(this));
	}
	//------------------------------------------------------------------------------
	void CCalculatedFieldObject::setExpression(QString expression) {
		this->expression=expression;
		QRegExp regExp("\\$\\{([a-zA-Z0-9]*)\\}");
		int pos = 0;
		
		while ((pos = regExp.indexIn(expression, pos)) != -1) {
			QString fieldId=regExp.cap(1);
			CField *field=CFields::getInstance()->getField(fieldId);
			
			pos += regExp.matchedLength();
			if(field == 0) {
				throw new QString("Field "+fieldId+" not exists !");
				return;
			}
			depends << field;
		}
	}
	//------------------------------------------------------------------------------
	void CCalculatedFieldObject::serialize(QDataStream &out) {
		out << QString("CCalculatedFieldObject");
		CField::serialize(out);
	}
	//------------------------------------------------------------------------------
	void CTotalFieldObject::process(QSqlRecord *record) {
		CField *field=CFields::getInstance()->getField(getAttribute("fieldId"));
		QString operation=getAttribute("operation");
		double fValue=field->getFieldValue().toDouble();
		
		if(groupToResetOn != 0) {
			if(groupToResetOn->isChanged()) {
				value=0;
				sum=0;
				nb=0;
				firstTime=true;
			}
		}
		
		if(groupToAccumulateOn == 0 || groupToAccumulateOn->isChanged()) {
			if(operation == "sum") {
				value+=fValue;
			}else if(operation == "avg") {
				sum+=fValue;
				nb++;
				value=sum/nb;
			}else if(operation == "count") {
				value++;
			}else if(operation == "min") {
				if(fValue < value || firstTime) {
					value=fValue;
					firstTime=false;
				}
			}else if(operation == "max") {
				if(fValue > value) {
					value=fValue;
				}
			}
		}
		
		setFieldValue(QVariant(value));
	}
	//------------------------------------------------------------------------------
	const QList<CField *> CTotalFieldObject::getDepends(void) {
		QList<CField *> depend;
		CField *field=CFields::getInstance()->getField(getAttribute("fieldId"));
		
		depend << field;
		
		return depend;
	}
	//------------------------------------------------------------------------------
	void CTotalFieldObject::processAttributes(const QXmlAttributes& atts) {
		int i;
		
		for(i=0;i<atts.count();i++) {
			setAttribute(atts.localName(i), atts.value(i));
			
			if(atts.localName(i) == "resetOn") {
				setGroupToResetOn(CGroups::getInstance()->getGroup(atts.value(i)));
			}
			
			if(atts.localName(i) == "accumulateOn") {
				setGroupToAccumulateOn(CGroups::getInstance()->getGroup(atts.value(i)));
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTotalFieldObject::serialize(QDataStream &out) {
		out << QString("CTotalFieldObject");
		CField::serialize(out);
	}
	//------------------------------------------------------------------------------
}//namespace
//------------------------------------------------------------------------------