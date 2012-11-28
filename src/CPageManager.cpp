//------------------------------------------------------------------------------
#include <QtDebug>
#include "CDocument.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	CPageManager::CPageManager(CDocument *document) {
		this->document=document;
		pageHeight=document->getPageHeight();
		pageWidth=document->getPageWidth();
		newPage=true;
		endPage=false;
		hFooter=0;
		coef=document->getCoef();
		if(document->getPageFooter() != 0) {
			hFooter=document->getPageFooter()->getHeight(coef);
		}
		
		hSpecified=false;
		hPage=0;
		if(pageHeight != "100%") {
			hSpecified=true;
			hPage=pageHeight.toInt()*coef;
		}
		wPage=pageWidth.toInt()*coef;
	}
	//------------------------------------------------------------------------------
	void CPageManager::process(CPrintableObject *printableObject, bool firstEnr, bool lastEnr) {
		CPage *page;
		QString svg;
		
		if(endPage ) {
			page=document->last();
			
			endPage=false;
			if(lastEnr) {
				draw(document->getDocFooter());
			}
			draw(document->getPageFooter());
			
			svg=page->getSvg()+"</svg>";
			svg.replace("${height}", QString::number(hSpecified ? hPage : hPage=y));
			
			page->setSvg(svg);
		}
		
		if(newPage) {
			page=new CPage();
			svg;
			QString w=QString::number(wPage);
			
			svg="<?xml version='1.0' encoding='utf-8'?>";
			svg+="<svg xmlns='http://www.w3.org/2000/svg' version='1.2' ";
			svg+="baseProfile='tiny' width='"+w+"' ";
			svg+="height='${height}'>";
			svg+="<rect x='0' y='0' width='"+w+"' height='${height}' fill='white' stroke='none' />";
			
			page->setSvg(svg);
			
			document->append(page);
			
			y=0;
			newPage=false;
			draw(document->getPageHeader());
			if(firstEnr) {
				draw(document->getDocHeader());
			}
		}
		
		if(printableObject != 0) {
			bool resteOk=true;
			if(hSpecified) {
				resteOk=(hPage-y-printableObject->getHeight(coef)-hFooter) >= 0;
			}
			
			if(!resteOk) {
				endPage=true;
				newPage=true;
				process(printableObject, firstEnr, lastEnr);
			}else {
				draw(printableObject);
			}
		}
	}
	//------------------------------------------------------------------------------
	void CPageManager::end(void) {
		endPage=true;
		process(0, false, true);
	}
	//------------------------------------------------------------------------------
	void CPageManager::draw(CPrintableObject *printableObject) {
		if(printableObject != 0) {
			CPage *page=document->last();
			QString svg=page->getSvg();
			
			svg+=printableObject->toSvg(y, coef);
			page->setSvg(svg);
			
			printableObject->prepareRender(page->getRendererObjects(), y, coef);
			y+=printableObject->getHeight(coef);
		}
	}
	//------------------------------------------------------------------------------
} //namespace
//------------------------------------------------------------------------------