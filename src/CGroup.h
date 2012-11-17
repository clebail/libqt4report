//------------------------------------------------------------------------------
#ifndef __CGROUP_H__
#define __CGROUP_H__
//------------------------------------------------------------------------------
#include <QString>
//------------------------------------------------------------------------------
namespace libqt4report {
	class CGroup {
		public:
			CGroup(void);
			void setId(QString id) { this->id=id; }
			QString getId(void) { return id ;}
			void setRefer(QString refer) { this->refer=refer; }
			QString getRefer(void) { return refer; }
			void setChild(CGroup * child) { this->child=child; }
			CGroup * getChild(void) { return child; }
			void setParent(CGroup * parent) { this->parent=parent; }
			CGroup * getParent(void) { return parent; }
		private:
			QString id;
			QString refer;
			CGroup *child;
			CGroup *parent;
	};
	//------------------------------------------------------------------------------
}// namespace
//------------------------------------------------------------------------------
#endif //__CGROUP_H__
//------------------------------------------------------------------------------