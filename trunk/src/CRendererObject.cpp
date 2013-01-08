//------------------------------------------------------------------------------
#include <QtDebug>
#include "CRendererObject.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	//------------------------------------------------------------------------------
	void CRendererObjectText::draw(QPainter * painter) {
		QPen oldPen=painter->pen();
		QPen newPen(color);
		
		painter->setPen(newPen);
		painter->setFont(font);
		painter->drawText(point, text);
		
		painter->setPen(oldPen);
	}
	//------------------------------------------------------------------------------
	void CRendererObjectLine::draw(QPainter * painter) {
		QPen oldPen=painter->pen();
		QPen newPen(color);
		
		painter->setPen(newPen);
		painter->drawLine(startPoint, endPoint);
		
		painter->setPen(oldPen);
	}
	//------------------------------------------------------------------------------
	void CRendererObjectRect::draw(QPainter * painter) {
		QPen oldPen=painter->pen();
		QPen newPen(color);
		
		painter->setPen(newPen);
		painter->drawRect(rect);
		
		painter->setPen(oldPen);
	}
	//------------------------------------------------------------------------------
	void CRendererObjectImage::draw(QPainter * painter) {
		painter->drawImage(rect, image);
	}
	//------------------------------------------------------------------------------
}// namespace
//------------------------------------------------------------------------------