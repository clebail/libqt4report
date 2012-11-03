//------------------------------------------------------------------------------
#ifndef __CRENDEREROBJECT_H__
#define __CRENDEREROBJECT_H__
//------------------------------------------------------------------------------
#include <QPainter>
#include <QString>
#include <QPoint>
#include <QFont>
//------------------------------------------------------------------------------
namespace libqt4report {
	class CRendererObject {
		public:
			virtual void draw(QPainter * painter) = 0;
	};
	//------------------------------------------------------------------------------
	class CRendererObjectText : public CRendererObject {
		public:
			void setText(QString text) { this->text=text; }
			void setFont(QFont font) { this->font=font; }
			void setPoint(QPoint point) { this->point=point; }
			void draw(QPainter * painter);
		private:
			QString text;
			QFont font;
			QPoint point;
	};
	//------------------------------------------------------------------------------
	class CRendererObjectLine : public CRendererObject {
		public:
			void draw(QPainter * painter) {};
	};
	//------------------------------------------------------------------------------
	class CRendererObjectRect : public CRendererObject {
		public:
			void draw(QPainter * painter) {};
	};
	//------------------------------------------------------------------------------
} //namespace
//------------------------------------------------------------------------------
#endif //__CRENDEREROBJECT_H__
//------------------------------------------------------------------------------