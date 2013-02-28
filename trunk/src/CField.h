//------------------------------------------------------------------------------
#ifndef __CFIELD_H__
#define __CFIELD_H__
//------------------------------------------------------------------------------
#include <QHash>
#include <QSqlRecord>
#include <QStringList>
#include <QVariant>
#include <QXmlAttributes>
#include "CGroup.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CField {
		public:
			virtual ~CField(void) {}
			void setAttribute(QString name, QString value) { attributes.insert(name, value); }
			QString getAttribute(QString name) { return attributes.value(name); }
			virtual void process(QSqlRecord *record) = 0;
			QVariant getFieldValue(void) { return value; }
			void resetPoid(void) { poid=0; }
			void decPoid(void) { poid--; }
			int getPoid(void) { return poid; }
			virtual const QList<CField *> getDepends(void) { return QList<CField *>(); }
			virtual void processAttributes(const QHash<QString, QString>& atts);
			virtual void serialize(QDataStream &out);
			virtual void fromCache(QDataStream &in);
			static const QHash<QString, QString> fromXmlAttributes(const QXmlAttributes& atts);
		protected:
			QHash<QString ,QString> attributes;
			int poid;
			QVariant value;
			
			void setFieldValue(QVariant value) { this->value=value; }
	};
	//------------------------------------------------------------------------------
	class CDbFieldObject : public CField {
		public:
			void process(QSqlRecord *record);
			virtual void serialize(QDataStream &out);
	};
	//------------------------------------------------------------------------------
	class CCalculatedFieldObject : public CField {
		public:
			void process(QSqlRecord *record);
			void setExpression(QString expression);
			QString getExpression(void) { return expression; }
			const QList<CField *> getDepends(void) { return depends; }
			virtual void serialize(QDataStream &out);
		private:
			QString expression;
			QList<CField *> depends;
	};
	//------------------------------------------------------------------------------
	class CTotalFieldObject : public CField {
		public:
			CTotalFieldObject(void) { value=0; firstTime=true; sum=0; nb=0; groupToResetOn=groupToAccumulateOn=0; }
			void process(QSqlRecord *record);
			const QList<CField *> getDepends(void);
			void setGroupToResetOn(CGroup *group) { groupToResetOn=group; }
			void setGroupToAccumulateOn(CGroup *group) { groupToAccumulateOn=group; }
			virtual void processAttributes(const QHash<QString, QString>& atts);
			virtual void serialize(QDataStream &out);
		private:
			double value;
			double sum;
			int nb;
			bool firstTime;
			CGroup *groupToResetOn;
			CGroup *groupToAccumulateOn;
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CFIELD_H__
//------------------------------------------------------------------------------