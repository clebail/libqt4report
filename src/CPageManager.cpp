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
		if(document->getPageFooter() != 0) {
			hFooter=document->getPageFooter()->getHeight(document->getCoef());
		}
		
		hSpecified=false;
		hPage=0;
		if(pageHeight != "100%") {
			hSpecified=true;
			hPage=pageHeight.toInt()*document->getCoef();
		}
	}
	//------------------------------------------------------------------------------
	void CPageManager::process(CPrintableObject *printableObject, double coef, bool firstEnr, bool lastEnr) {
		if(endPage) {
			endPage=false;
			if(lastEnr) {
				draw(document->getDocFooter(), coef);
			}
			draw(document->getPageFooter(), coef);
			
			CPage *page=document->last();
			QString svg=page->getSvg();
			
			svg+="</svg>";
			svg.replace("${height}", QString::number(hSpecified ? hPage : hPage=y));
			
			page->setSvg(svg);
		}
		
		if(newPage) {
			CPage *page=new CPage();
			QString svg;
			int wPage=pageWidth.toInt()*coef;
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
			draw(document->getPageHeader(), coef);
			if(firstEnr) {
				draw(document->getDocHeader(), coef);
			}
		}
		
		if(printableObject != 0) {
			bool resteOk=true;
			if(hSpecified) {
				resteOk=(pageHeight.toInt()-y-printableObject->getHeight(coef)-hFooter) >= 0;
			}
			
			if(!resteOk) {
				endPage=true;
				newPage=true;
				process(printableObject, coef, firstEnr, lastEnr);
			}else {
				draw(printableObject, coef);
			}
		}
	}
	//------------------------------------------------------------------------------
	void CPageManager::draw(CPrintableObject *printableObject, double coef) {
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