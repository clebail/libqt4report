//------------------------------------------------------------------------------
#ifndef __CVALUETYPE_H__
#define __CVALUETYPE_H__
//------------------------------------------------------------------------------
#include <QVariant>
#include <QDate>
#include <QDateTime>
//------------------------------------------------------------------------------
namespace libqt4report {
	class CValueType {
		public:
			virtual ~CValueType(void) {};
			virtual void setValue(QVariant value) = 0;
			virtual QString toFormatedString(QString format) = 0;
	};
	//------------------------------------------------------------------------------
	class CValueTypeString : public CValueType {
		public:
			void setValue(QVariant value) { this->value=value.toString(); }
			QString toFormatedString(QString format) { return value; }
		private:
			QString value;
	};
	//------------------------------------------------------------------------------
	class CValueTypeInteger: public CValueType {
		public:
			void setValue(QVariant value) { this->value=value.toInt(); }
			QString toFormatedString(QString format) { return QString("%L1").arg(value); }
		private:
			int value;
	};
	//------------------------------------------------------------------------------
	class CValueTypeReal: public CValueType {
		public:
			void setValue(QVariant value) { this->value=value.toDouble(); }
			QString toFormatedString(QString format);
		private:
			double value;
	};
	//------------------------------------------------------------------------------
	class CValueTypeDate: public CValueType {
		public:
			void setValue(QVariant value) { this->value=value.toDate(); }
			QString toFormatedString(QString format);
		private:
			QDate value;
	};
	//------------------------------------------------------------------------------
	class CValueTypeDateTime: public CValueType {
		public:
			void setValue(QVariant value) { this->value=value.toDateTime(); }
			QString toFormatedString(QString format);
		private:
			QDateTime value;
	};
}//namespace
//------------------------------------------------------------------------------
#endif //__CVALUETYPE_H__
//------------------------------------------------------------------------------