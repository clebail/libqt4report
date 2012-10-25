//--------------------------------------------------------------------------------------------------------------
#include <QtDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QPaintEngine>
#include <config.h>
#include "CMainFrm.h"
//--------------------------------------------------------------------------------------------------------------
CMainFrm::CMainFrm(QString fileName) : QMainWindow() {
	setupUi(this);
	
	xmlFile=new QFile();
	scrollArea=new QScrollArea(this);
	scrollArea->setAlignment(Qt::AlignCenter);
	
	report=new libqt4report::CReport();
	QApplication::installTranslator(report->getTranslator());
	
	setCentralWidget(scrollArea);
	
	setWindowTitle("viewer - "+QString(PACKAGE_VERSION));
	
	if(!fileName.isEmpty()) {
		loadFile(fileName);
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
void CMainFrm::loadFile(QString fileName) {
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
		}
	}else {
		QMessageBox::critical(this, tr("Error"), tr("Impossible to open document"));
	}
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::showPage() {
	svgWidget->load(report->toSvg(curPage).toUtf8());

	scrollArea->setWidget(svgWidget); 
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionQuitter_triggered(bool) {
	close();
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionOuvrir_triggered(bool) {
	QString fileName=QFileDialog::getOpenFileName(this, tr("Open"), QDir::home().path(), tr("xlqr file (*.xlqr)"));
	
	if(!fileName.isEmpty()) {
		loadFile(fileName);
	}
}
//--------------------------------------------------------------------------------------------------------------
void CMainFrm::on_actionRecharger_triggered(bool) {	
	xmlFile->seek(0);
	if(report->process(xmlFile)) {
		nbPage=report->getNbPage();
		svgWidget=new QSvgWidget();
		on_actionPremierePage_triggered();
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