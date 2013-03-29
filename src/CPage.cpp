//------------------------------------------------------------------------------
#include "CPage.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	CPage::CPage(void) {
		rendererObjects=new QList<CRendererObject *>();
	}
	//------------------------------------------------------------------------------
	CPage::~CPage(void) {
		int i;
		
		for(i=0;i<rendererObjects->size();i++) {
			delete rendererObjects->at(i);
		}
		
		rendererObjects->clear();
		delete rendererObjects;
	}
}// namespace
//------------------------------------------------------------------------------