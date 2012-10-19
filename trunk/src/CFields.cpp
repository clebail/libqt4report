//------------------------------------------------------------------------------
#include <QtDebug>
#include "CFields.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	CFields *CFields::instance=0;
	//------------------------------------------------------------------------------
	CField * CFields::getField(QString key) { 
		if(!keyMap->contains(key)) {
			throw new QString("Field "+key+" do not declared !");
		}
		return map->at(keyMap->value(key)); 
	}
	//------------------------------------------------------------------------------
	void CFields::addField(QString key, CField *field) {
		int nextIdx=map->size();
		keyMap->insert(key, nextIdx);
		map->append(field); 
	}
	//------------------------------------------------------------------------------
	void CFields::process(QSqlRecord *record) {
		int i;
		
		for(i=0;i<map->size();i++) {
			map->at(i)->process(record);
		}
	}
	//------------------------------------------------------------------------------
	bool CFields::processDepends(void) {
		int i;
		
		resetPoids();
		
		try {
			for(i=0;i<map->size();i++) {
				CField *field=map->at(i);
				QList<CField *> depends=field->getDepends();
				QString fieldId=field->getAttribute("id");
				int j;
				
				for(j=0;j<depends.size();j++) {
					calculPoids(fieldId, depends.at(j));
				}
			}
		}catch(QString *e) {
			dependsError=*e;
			return false;
		}
		
		quickSort(0, map->size()-1);
		
		return true;
	}
	//------------------------------------------------------------------------------
	void CFields::cleanup(void) {
		int i;
		
		for(i=0;i<map->size();i++) {
			delete map->at(i);
		}
		
		delete map;
		delete keyMap;
		delete this;
		
		instance=0;
	}
	//------------------------------------------------------------------------------
	CFields * CFields::getInstance(void) {
		if(instance == 0) {
			instance=new CFields();
		}
		return instance;
	}
	//------------------------------------------------------------------------------
	CFields::CFields(void) {
		map=new QList<CField *>();
		keyMap=new QHash<QString, int>();
	}
	//------------------------------------------------------------------------------
	void CFields::resetPoids(void) {
		int i;
		
		for(i=0;i<map->size();i++) {
			map->at(i)->resetPoid();
		}
	}
	//------------------------------------------------------------------------------
	void CFields::calculPoids(QString fieldIdRef, CField *field) {
		int i;
		QList<CField *> depends;
		
		if(fieldIdRef == field->getAttribute("id")) {
			throw new QString("The Field "+fieldIdRef+" cannot depend on himself");
			return;
		}
		field->decPoid();
		depends=field->getDepends();
		for(i=0;i<depends.size();i++) {
			calculPoids(fieldIdRef, depends.at(i));
		}
	}
	//------------------------------------------------------------------------------
	void CFields::quickSort(int premier, int dernier) {
		if(premier < dernier) {
			int pivot=qrand()%(dernier-premier)+premier;
			
			pivot=partitionner(premier, dernier, pivot);
			quickSort(premier, pivot-1);
			quickSort(pivot+1, dernier);
		}
	}
	//------------------------------------------------------------------------------
	int CFields::partitionner(int premier, int dernier, int pivot) {
		swap(map->at(pivot), map->at(dernier));
		int j=premier;
		for(int i=premier;i<dernier;i++) {
			if(map->at(i)->getPoid() < map->at(dernier)->getPoid()) {
				swap(map->at(i), map->at(j));
				j++;
			}
		}
		swap(map->at(dernier), map->at(j));
		return j;
	}
	//------------------------------------------------------------------------------
	void CFields::swap(CField *f1, CField *f2) {
		if(f1 != f2) {
			QString idF1, idF2;
			int idxF1, idxF2;
			CField *fTemp;
			
			idF1=f1->getAttribute("id");
			idF2=f2->getAttribute("id");
			
			idxF1=keyMap->take(idF1);
			idxF2=keyMap->take(idF2);
			
			fTemp=map->at(idxF1);
			map->replace(idxF1, f2);
			map->replace(idxF2, fTemp);
			
			keyMap->insert(idF2, idxF1);
			keyMap->insert(idF1, idxF2);
		}
	}
	//------------------------------------------------------------------------------
}//namespace
//------------------------------------------------------------------------------