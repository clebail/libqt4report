//------------------------------------------------------------------------------
#include <QtDebug>
#include <log4cpp/Category.hh>
#include "CGroups.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	CGroups *CGroups::instance=0;
	static log4cpp::Category& logger = log4cpp::Category::getInstance("CGroups");
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
		
		logger.debug((QString("add group ")+id).toStdString());
		
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
		nbGroup++;
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
		
		firstGroup=lastGroup=0;
		nbGroup=0;
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
	void CGroups::serialize(QDataStream &out) {
		CGroup *pGroup=firstGroup;
		
		logger.debug(("Serialize groups ("+QString::number(nbGroup)+")").toStdString());
		
		out << (qint32)nbGroup;
		
		while(pGroup != 0) {
			out << pGroup->getId();
			out << pGroup->getRefer();
			pGroup=pGroup->getChild();
		}
	}
	//------------------------------------------------------------------------------
	void CGroups::fromCache(QDataStream &in, qint32 size) {
		for(int i=0;i<size;i++) {
			QString groupId;
			QString groupRefer;
			
			in >> groupId;
			in >> groupRefer;
			
			logger.debug((QString("Fill group ")+groupId+QString(" refer ")+groupRefer+QString(" from cache")).toStdString());
			
			
			addGroup(groupId, groupRefer);
		}
	}
	//------------------------------------------------------------------------------
	CGroups::CGroups(void) {
		firstGroup=lastGroup=0;
		nbGroup=0;
	}
	//------------------------------------------------------------------------------
}// namespace
//------------------------------------------------------------------------------