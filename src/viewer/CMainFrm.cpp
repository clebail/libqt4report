//--------------------------------------------------------------------------------------------------------------
#include <QtDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QPaintEngine>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QSvgRenderer>
#include <QInputDialog>
#include <config.h>
#include "CMainFrm.h"
//--------------------------------------------------------------------------------------------------------------
CMainFrm::CMainFrm(QString fileName) : QMainWindow() {
	setupUi(this);
	
	xmlFile=new QFile();
	scrollArea=new QScrollArea(this);
	scrollArea->setAlignment(Qt::AlignCenter);
	
	report=new libqt4report::CReport();
	connect(report, SIGNAL(queryParam(QString,QVariant&)), this, SLOT(onReportQueryParam(QString,QVariant&)));
	QApplication::installTranslator(libqt4report::CReport::getTranslator());
	
	setCentralWidget(scrollArea);
	
	setWindowTitle("viewer - "+QString(PACKAGE_VERSION));
	
	if(!fileName.isEmpty()) {
		if(loadFile(fileName)) {
			on_actionRecharger_triggered();
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
CMainFrm::~CMainFrm(void) {
	if(xmlFile->isOpen()) {
		xmlFile->close();
	}
	delete xmlFile;
	delete report;
}
//--------------------------------------------------------------------------------------------------------------
bool CMainFrm::loadFile(QString fileName) {
	if(xmlFile->isOpen()) {
		xmlFile->close();
	}
	
	xmlFile->setFileName(fileName);
	
	if(xmlFile->open(QIODevice::ReadOnly)) {
		if(!report->validDocument(xmlFile)) {
			QMessageBox::critical(this, tr("Validation error"), tr("Impossible to validate document, are your sure that is it a lxqr file ?"));
		}else {
			xmlFile->seek(0);
			actionRecharger->setEnabled(true);
			
			Ui_MainFrm::statusBar->showMessage(fileName);
			
			return true;
		}
	}else {
		QMessageBox::critical(this, tr("Error"), tr("Impossible to open document"));
	}
	
	return false;
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::showPage() {
	svgWidget->load(report->toSvg(curPage).toUtf8());

	scrollArea->setWidget(svgWidget);
	
	enableAction();
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::enableAction(void) {
	actionPremierePage->setEnabled(curPage != 0);
	actionPagePrecedente->setEnabled(actionPremierePage->isEnabled());
	actionPageSuivante->setEnabled(curPage < nbPage-1);
	actionDernierePage->setEnabled(actionPageSuivante->isEnabled());
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionQuitter_triggered(bool) {
	close();
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionOuvrir_triggered(bool) {
	QString fileName=QFileDialog::getOpenFileName(this, tr("Open"), QDir::home().path(), tr("xlqr file (*.xlqr)"));
	
	if(!fileName.isEmpty()) {
		if(loadFile(fileName)) {
			on_actionRecharger_triggered();
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionRecharger_triggered(bool) {	
	xmlFile->seek(0);
	
	if(report->process(xmlFile)) {
		nbPage=report->getNbPage();
		svgWidget=new QSvgWidget();
		on_actionPremierePage_triggered();
		actionPrint->setEnabled(true);
	}else {
		QMessageBox::critical(this, report->getLastSourceError(), report->getLastError());
	}
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionFermer_triggered(bool) {
	if(xmlFile->isOpen()) {
		xmlFile->close();
	}
	
	Ui_MainFrm::statusBar->clearMessage();
	actionRecharger->setEnabled(false);
	actionPrint->setEnabled(false);
	nbPage=curPage=0;
	enableAction();
	delete svgWidget; 
	scrollArea->setWidget(0); 
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionPremierePage_triggered(bool) {
	curPage=0;
	showPage();
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionPagePrecedente_triggered(bool) {
	if(curPage > 0) {
		curPage--;
		showPage();
	}
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionPageSuivante_triggered(bool) {
	if(curPage < nbPage-1) {
		curPage++;
		showPage();
	}
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionDernierePage_triggered(bool) {
	curPage=nbPage-1;
	showPage();
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionPrint_triggered(bool) {
	QPrintDialog *printDialog=new QPrintDialog(this);
	
	if(printDialog->exec() == QDialog::Accepted)
	{
		QPrinter *printer=printDialog->printer();
		
		printer->setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
		printer->setPaperSize(report->getPagesSize(), QPrinter::DevicePixel);
		printer->setResolution(72);
		
		QPainter p;
		p.begin(printer);
		p.setRenderHint((QPainter::RenderHint)0, true);
		
		int f=qMin(printer->fromPage(), nbPage);
		int t=qMin(printer->toPage(), nbPage);
		int nbPageToPrint;
		int sens=1;
		int i,j;
		QSvgRenderer svgRenderer;
		
		switch(printer->printRange())
		{
			default:
			case QPrinter::AllPages:
				f=0;
				t=nbPage;
				break;
			case QPrinter::PageRange:
				f--;
				break;
		}
		
		nbPageToPrint=t*printer->numCopies();
		
		if(printer->pageOrder() == QPrinter::LastPageFirst)
		{
			qSwap(f, t);
			f--;
			t--;
			sens=-1;
		}
		
		for(i=1,j=f;i<=nbPageToPrint;i++)
		{
			svgRenderer.load(report->toSvg(j).toUtf8());
			svgRenderer.render(&p);
			
			if(i<nbPageToPrint) 
			{
				printer->newPage();
			}
			
			if(printer->collateCopies())
			{
				j+=sens*(i%printer->numCopies() == 0 ? 1 : 0);
			}else {
				j+=sens;
				if(j == t)
				{
					j=f;
				}
			}
		}
		p.end();
	}
	delete printDialog;
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::onReportQueryParam(QString paramName, QVariant& value) {
	QString sValue=QInputDialog::getText(this, tr("Value for")+" "+paramName+" ?", paramName);
	
	value=QVariant(sValue);
}
//--------------------------------------------------------------------------------------------------------------