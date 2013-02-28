//------------------------------------------------------------------------------
#ifndef __CFIELDS_H__
#define __CFIELDS_H__
//------------------------------------------------------------------------------
#include <QList>
#include <QHash>
#include <QSqlRecord>
#include "CField.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CFields {
		public:
			CField * getField(QString key);
			void addField(QString key, CField *field);
			void process(QSqlRecord *record);
			QString getDependsError(void) { return dependsError; }
			bool processDepends(void);
			void cleanup(void);
			static CFields * getInstance(void);
			void serialize(QDataStream &out);
			void fromCache(QDataStream &in, qint32 size);
		private:
			static CFields *instance;
			QList<CField *> *map;
			QHash<QString, int> *keyMap;
			QString dependsError;
			
			CFields(void);
			void resetPoids(void);
			void calculPoids(QString fieldIdRef, CField *field);
			void quickSort(int premier, int dernier);
			int partitionner(int premier, int dernier, int pivot);
			void swap(CField *f1, CField *f2);
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CFIELD_H__
//------------------------------------------------------------------------------