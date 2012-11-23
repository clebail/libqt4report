//------------------------------------------------------------------------------
#ifndef __CGROUPS_H__
#define __CGROUPS_H__
//------------------------------------------------------------------------------
#include "CGroup.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CGroups {
		public:
			static CGroups * getInstance(void);
			
			void addGroup(QString id, QString refer);
			void cleanup(void);
			CGroup *getFirstGroup(void) { return firstGroup; }
			CGroup *getLastGroup(void) { return lastGroup; }
			CGroup *getGroup(QString id);
		private:
			static CGroups *instance;
			CGroup *firstGroup;
			CGroup *lastGroup;
			
			CGroups(void);
	};
	//------------------------------------------------------------------------------
}// namespace
//------------------------------------------------------------------------------
#endif// __CGROUPS_H__
//------------------------------------------------------------------------------