//------------------------------------------------------------------------------
#include <QtDebug>
#include <QVariant>
#include <QDate>
#include <QFontMetrics>
#include "CItem.h"
#include "CFields.h"
#include "CFonts.h"
#include "CValueType.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	void CItem::processAttributes(const QXmlAttributes& atts) {
		int i;
		
		for(i=0;i<atts.count();i++) {
			setAttribute(atts.localName(i), atts.value(i));
		}
	}
	//------------------------------------------------------------------------------
	void CItem::serialize(QDataStream &out) {
		QHashIterator<QString, QString> i(attributes);
		
		while(i.hasNext()) {
			i.next();
			out << i.key();
			out << i.value();
		}
	}
	//------------------------------------------------------------------------------
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
		style+="font-weight:"+CFont::toSvgWeight(font->getWeight())+";";
		style+="font-style:"+CFont::toSvgStyle(font->getStyle())+";";
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
	void CItemText::prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef) {
		CRendererObjectText * rendererObject=new CRendererObjectText();
		QFont font;
		QPoint point((int)(getAttribute("x").toDouble()*coef), (int)((getAttribute("y").toDouble())*coef)+y);
		QString text=getValue();
		
		CFonts::getInstance()->getFont(getAttribute("fontId"))->toQFont(font);
				
		if(hasAttribute("align")) {
			QString align=getAttribute("align");
			
			QFontMetrics fm(font);
			int width=fm.width(text);
			
			if(align == "end") {
				point.rx()-=width;
			}else if(align == "middle") {
				point.rx()-=width/2;
			}
		}
		
		rendererObject->setText(text);
		rendererObject->setFont(font);
		rendererObject->setPoint(point);
		rendererObject->setColor(hasAttribute("color") ? getAttribute("color") : "#000000");
		
		rendererObjects->append(rendererObject);
	}
	//------------------------------------------------------------------------------
	QString CItemTextFixedObject::getValue(void) {
		return getAttribute("value");
	}
	//------------------------------------------------------------------------------
	void CItemTextFixedObject::serialize(QDataStream &out) {
		out << "CItemTextFixedObject";
		CItem::serialize(out);
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
	void CItemTextFieldObject::processAttributes(const QXmlAttributes& atts) {
		int i;
		
		for(i=0;i<atts.count();i++) {
			setAttribute(atts.localName(i), atts.value(i));
			
			if(atts.localName(i) == "fieldId") {
				CField *field=CFields::getInstance()->getField(atts.value(i));
				createValue(field->getAttribute("dataType"));
			}
		}
	}
	//------------------------------------------------------------------------------
	void CItemTextFieldObject::serialize(QDataStream &out) {
		out << "CItemTextFieldObject";
		CItem::serialize(out);
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
	void CItemLineObject::prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef) {
		CRendererObjectLine * rendererObject=new CRendererObjectLine();
		int height=(int)((getAttribute("height").toDouble())*coef);
		QPoint startPoint;
		QPoint endPoint;
		
		startPoint.setX((int)(getAttribute("x").toDouble()*coef));
		startPoint.setY((int)((getAttribute("y").toDouble())*coef)+y);
		endPoint.setX(startPoint.x()+(int)(getAttribute("width").toDouble()*coef));
		endPoint.setY(startPoint.y()+height);
		
		rendererObject->setStartPoint(startPoint);
		rendererObject->setEndPoint(endPoint);
		
		rendererObject->setColor(hasAttribute("color") ? getAttribute("color") : "#000000");
		
		rendererObjects->append(rendererObject);
	}
	//------------------------------------------------------------------------------
	int CItemLineObject::getHeight(double coef) { 
		int y, height;
		
		y=(int)((getAttribute("y").toDouble())*coef);
		height=(int)((getAttribute("height").toDouble())*coef);
		
		return y+height;
	}
	//------------------------------------------------------------------------------
	void CItemLineObject::serialize(QDataStream &out) {
		out << "CItemLineObject";
		CItem::serialize(out);
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
	void CItemRectObject::prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef) {
		CRendererObjectRect * rendererObject=new CRendererObjectRect();
		QRect rect;
		
		rect.setX((int)(getAttribute("x").toDouble()*coef));
		rect.setY((int)((getAttribute("y").toDouble())*coef)+y);
		rect.setWidth((int)(getAttribute("width").toDouble()*coef));
		rect.setHeight((int)((getAttribute("height").toDouble())*coef));
		
		rendererObject->setRect(rect);
		
		rendererObject->setColor(hasAttribute("color") ? getAttribute("color") : "#000000");
		
		rendererObjects->append(rendererObject);
	}
	//------------------------------------------------------------------------------
	void CItemRectObject::serialize(QDataStream &out) {
		out << "CItemRectObject";
		CItem::serialize(out);
	}
	//------------------------------------------------------------------------------
}//namespace
//------------------------------------------------------------------------------