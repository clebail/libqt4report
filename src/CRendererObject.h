//------------------------------------------------------------------------------
#ifndef __CRENDEREROBJECT_H__
#define __CRENDEREROBJECT_H__
//------------------------------------------------------------------------------
#include <QPainter>
#include <QString>
#include <QPoint>
#include <QFont>
#include <QImage>
#include <QtDebug>
//------------------------------------------------------------------------------
namespace libqt4report {
	class CRendererObject {
		public:
			virtual ~CRendererObject(void) {}
			void setColor(QString sColor) { color.setNamedColor(sColor); }
			virtual void draw(QPainter * painter) = 0;
		protected:
			QColor color;
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
			void setStartPoint(QPoint point) { startPoint=point; }
			void setEndPoint(QPoint point) { endPoint=point; }
			void draw(QPainter * painter);
		private:
			QPoint startPoint;
			QPoint endPoint;
	};
	//------------------------------------------------------------------------------
	class CRendererObjectRect : public CRendererObject {
		public:
			void setRect(QRect rect) { this->rect=rect; }
			void draw(QPainter * painter);
		private:
			QRect rect;
	};
	//------------------------------------------------------------------------------
	class CRendererObjectImage : public CRendererObject {
		public:
			void setImage(QImage image) { this->image=image; }
			void setRect(QRect rect) { this->rect=rect; }
			void draw(QPainter * painter);
		private:
			QImage image;
			QRect rect;
	};
	//------------------------------------------------------------------------------
} //namespace
//------------------------------------------------------------------------------
#endif //__CRENDEREROBJECT_H__
//------------------------------------------------------------------------------