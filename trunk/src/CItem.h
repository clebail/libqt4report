//------------------------------------------------------------------------------
#ifndef __CITEM_H__
#define __CITEM_H__
//------------------------------------------------------------------------------
#include <QHash>
#include <QVariant>
#include <QXmlAttributes>
#include "CPrintableObject.h"
#include "CValueType.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CItem : public CPrintableObject {
		public:
			virtual ~CItem(void) {}
			void setAttribute(QString name, QString value) { attributes.insert(name, value); }
			QString getAttribute(QString name) { return attributes.value(name) ;}
			bool hasAttribute(QString name) { return attributes.contains(name); }
			static const QHash<QString, QString> fromXmlAttributes(const QXmlAttributes& atts);
			virtual void processAttributes(const QHash<QString, QString>& atts);
			virtual void serialize(QDataStream &out);
			virtual void fromCache(QDataStream &in);
		private:
			QHash<QString ,QString> attributes;
	};
	//------------------------------------------------------------------------------
	class CItemText : public CItem {
		public:
			virtual ~CItemText(void) {}
			QString toSvg(int y, double coef);
			int getHeight(double coef) { return (int)(getAttribute("y").toDouble()*coef); }
			void prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef);
			virtual void serialize(QDataStream &out);
		protected:
			virtual QString getValue(void) = 0;
	};
	//------------------------------------------------------------------------------
	class CItemTextFixedObject : public CItemText {
		public:
			virtual void serialize(QDataStream &out);
		protected:
			QString getValue(void);
	};
	//------------------------------------------------------------------------------
	class CItemTextFieldObject : public CItemText {
		public:
			~CItemTextFieldObject(void);
			void createValue(QString fieldId);
			virtual void processAttributes(const QHash<QString, QString>& atts);
			virtual void serialize(QDataStream &out);
		protected:
			CValueType *value;
			QString getValue(void);
	};
	//------------------------------------------------------------------------------
	class CItemLineObject : public CItem {
		public:
			QString toSvg(int y, double coef);
			int getHeight(double coef);
			void prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef);
			virtual void serialize(QDataStream &out);
	};
	//------------------------------------------------------------------------------
	class CItemRectObject : public CItemLineObject {
		public:
			QString toSvg(int y, double coef);
			void prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef);
			virtual void serialize(QDataStream &out);
	};
	//------------------------------------------------------------------------------
	class CItemImageObject : public CItem {
		public:
			QString toSvg(int y, double coef);
			int getHeight(double coef);
			void prepareRender(QList<CRendererObject *> *rendererObjects, int y, double coef);
			virtual void serialize(QDataStream &out);
			virtual void processAttributes(const QHash<QString, QString>& atts);
		private:
			QString absolutePath;
	};
	//------------------------------------------------------------------------------
} //namespace
//------------------------------------------------------------------------------
#endif //__CITEM_H__
//------------------------------------------------------------------------------