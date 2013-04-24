//------------------------------------------------------------------------------
#include "CPage.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	CPage::CPage(void) {
		rendererObjects=new QList<CRendererObject *>();
		svg="";
	}
	//------------------------------------------------------------------------------
	CPage::~CPage(void) {
		int i;
		
		for(i=0;i<rendererObjects->size();i++) {
			delete rendererObjects->at(i);
		}
		
		svg.clear();
		rendererObjects->clear();
		delete rendererObjects;
	}
}// namespace
//------------------------------------------------------------------------------