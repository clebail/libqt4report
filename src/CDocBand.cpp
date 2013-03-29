//------------------------------------------------------------------------------
#include <QtDebug>
#include <log4cpp/Category.hh>
#include "CDocBand.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	//------------------------------------------------------------------------------
	static log4cpp::Category& logger = log4cpp::Category::getInstance("CDocBand");
	//------------------------------------------------------------------------------
	QString CDocBand::toSvg(int y, double coef) {
		QString svg="<g>";
		QHashIterator<QString, CItem *> i(*this);
		
		while (i.hasNext()) {
			i.next();
			svg+=i.value()->toSvg(y, coef);
		}
		
		svg+="</g>";
		
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
		clear();
	}
	//------------------------------------------------------------------------------
	void CDocBand::prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef) {
		QHashIterator<QString, CItem *> i(*this);
		
		while (i.hasNext()) {
			i.next();
			i.value()->prepareRender(rendererObjects, y, coef);
		}
	}
	//------------------------------------------------------------------------------
	void CDocBand::serialize(QDataStream &out) {
		QHashIterator<QString, CItem *> i(*this);
		
		out << qint32(count());
		while(i.hasNext()) {
			i.next();
			i.value()->serialize(out);
		}
	}
	//------------------------------------------------------------------------------
	void CDocBand::fromCache(QDataStream &in, qint32 size) {
		int i;
		
		for(i=0;i<size;i++) {
			QString itemClassName;
			
			in >> itemClassName;
			int id=QMetaType::type(itemClassName.toUtf8().data());
			
			logger.debug((QString("Fill ")+itemClassName+QString(" from cache")).toStdString());
			
			if(id != 0) {
				CItem *item=static_cast<CItem *>(QMetaType::construct(id));
				item->fromCache(in);
				
				insert(item->getAttribute("id"), item);
			}
		}
	}
	//------------------------------------------------------------------------------
} //namespace