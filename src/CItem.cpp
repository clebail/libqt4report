//------------------------------------------------------------------------------
#include <QtDebug>
#include <QVariant>
#include <QDate>
#include "CItem.h"
#include "CFields.h"
#include "CFonts.h"
#include "CValueType.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	QString CItemText::toSvg(int y, double coef) {
		QString value=xmlEncode(getValue());
		QString align="";
		QString style="style='";
		CFont *font=CFonts::getInstance()->getFont(getAttribute("fontId"));
		QString color;
		
		if(hasAttribute("color")) {
			color="stroke:"+getAttribute("color")+";fill:"+getAttribute("color")+";";
		}
		
		style+="font-family:"+font->getFamily()+";font-size:"+QString::number(font->getSize())+"pt;";
		style+="font-weight:"+font->getWeight()+";";
		style+="font-style:"+font->getStyle()+";";
		style+=color+"'";
		
		if(hasAttribute("align")) {
			align="text-anchor='"+getAttribute("align")+"'";
		}
		
		return QString("<text x='%1' y='%2' "+align+" "+style+">%3</text>")
			.arg((int)(getAttribute("x").toDouble()*coef))
			.arg((int)((getAttribute("y").toDouble())*coef)+y)
			.arg(value);
	}
	//------------------------------------------------------------------------------
	QString CItemTextFixedObject::getValue(void) {
		return getAttribute("value");
	}
	//------------------------------------------------------------------------------
	CItemTextFieldObject::~CItemTextFieldObject(void) {
		delete value;
	}
	//------------------------------------------------------------------------------
	void CItemTextFieldObject::createValue(QString fieldId) {
		QString className="CValueType"+fieldId.left(1).toUpper()+fieldId.mid(1);
		
		int id=QMetaType::type(className.toUtf8().data());
		
		if(id != 0) {
			value=static_cast<CValueType *>(QMetaType::construct(id));
		}
	}
	//------------------------------------------------------------------------------
	QString CItemTextFieldObject::getValue(void) {
		QString fieldId=getAttribute("fieldId");
		CField *field=CFields::getInstance()->getField(fieldId);
		value->setValue(field->getFieldValue());
		
		return value->toFormatedString(getAttribute("format"));
	}
	//------------------------------------------------------------------------------
	QString CItemLineObject::toSvg(int y, double coef) {
		int height=(int)((getAttribute("height").toDouble())*coef);
		int x1, y1, x2, y2;
		QString color="black";
		
		x1=(int)(getAttribute("x").toDouble()*coef);
		y1=(int)((getAttribute("y").toDouble())*coef)+y;
		x2=x1+(int)(getAttribute("width").toDouble()*coef);
		y2=y1+height;
		
		if(hasAttribute("color")) {
			color=getAttribute("color");
		}
		
		return QString("<line x1='%1' y1='%2' x2='%3' y2='%4' style='stroke:"+color+";stroke-width:1;' />")
			.arg(x1).arg(y1).arg(x2).arg(y2);
	}
	//------------------------------------------------------------------------------
	int CItemLineObject::getHeight(double coef) { 
		int y, height;
		
		y=(int)((getAttribute("y").toDouble())*coef);
		height=(int)((getAttribute("height").toDouble())*coef);
		
		return y+height;
	}
	//------------------------------------------------------------------------------
	QString CItemRectObject::toSvg(int y, double coef) {
		int x, yR, width, height;
		QString color="black";
		
		x=(int)(getAttribute("x").toDouble()*coef);
		yR=(int)((getAttribute("y").toDouble())*coef)+y;
		width=(int)(getAttribute("width").toDouble()*coef);
		height=(int)((getAttribute("height").toDouble())*coef);
		
		if(hasAttribute("color")) {
			color=getAttribute("color");
		}
		
		return QString("<rect x='%1' y='%2' width='%3' height='%4' style='stroke:"+color+";stroke-width:1;fill:none;' />")
		.arg(x).arg(yR).arg(width).arg(height);
	}
	//------------------------------------------------------------------------------
}//namespace
//------------------------------------------------------------------------------