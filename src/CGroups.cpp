//------------------------------------------------------------------------------
#include <QtDebug>
#include "CGroups.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	CGroups *CGroups::instance=0;
	//------------------------------------------------------------------------------
	CGroups * CGroups::getInstance(void) {
		if(instance == 0) {
			instance=new CGroups();
		}
		return instance;
	}
	//------------------------------------------------------------------------------
	void CGroups::addGroup(QString id, QString refer) {
		CGroup *nGroup=new CGroup();
			
		nGroup->setId(id);
		nGroup->setRefer(refer);
		
		
		if(firstGroup == 0) {
			firstGroup=nGroup;
		}else {
			CGroup *pGroup=firstGroup;
			
			while(pGroup->getChild() != 0) {
				pGroup=pGroup->getChild();
			}
			
			pGroup->setChild(nGroup);
			nGroup->setParent(pGroup);
		}
		
		lastGroup=nGroup;
	}
	//------------------------------------------------------------------------------
	void CGroups::cleanup(void) {
		CGroup *pGroup=firstGroup;
		CGroup *dGroup;
		
		if(pGroup != 0) {
			while(pGroup->getChild() != 0) {
				pGroup=pGroup->getChild();
			}
			
			dGroup=pGroup;
			do {
				pGroup=dGroup;
				delete dGroup;
				dGroup=pGroup->getParent();
			}while(dGroup != 0);
		}
	}
	//------------------------------------------------------------------------------
	CGroup *CGroups::getGroup(QString id) {
		CGroup *pGroup=firstGroup;
		
		while(pGroup != 0) {
			if(pGroup->getId() == id) {
				return pGroup;
			}
			
			pGroup=pGroup->getChild();
		}
		
		return 0;
	}
	//------------------------------------------------------------------------------
	CGroups::CGroups(void) {
		firstGroup=lastGroup=0;
	}
	//------------------------------------------------------------------------------
}// namespace
//------------------------------------------------------------------------------