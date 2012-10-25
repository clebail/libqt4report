//------------------------------------------------------------------------------
#ifndef __CMAINFRM_H__
#define __CMAINFRM_H__
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QFile>
#include <QScrollArea>
#include <QSvgWidget>
#include <CReport.h>
#include "ui_CMainFrm.h"
//------------------------------------------------------------------------------
class CMainFrm : public QMainWindow, private Ui::MainFrm {
	Q_OBJECT
	public:
		CMainFrm(QString fileName=QString());
		~CMainFrm(void);
	private:
		QFile *xmlFile;
		QScrollArea *scrollArea;
		libqt4report::CReport *report;
		int nbPage;
		int curPage;
		QSvgWidget *svgWidget;
		
		void loadFile(QString fileName);
		void showPage(void);
		void enableAction(void);
	private slots:
		void on_actionQuitter_triggered(bool checked=false);
		void on_actionOuvrir_triggered(bool checked=false);
		void on_actionRecharger_triggered(bool checked=false);
		void on_actionFermer_triggered(bool checked=false);
		void on_actionPremierePage_triggered(bool checked=false);
		void on_actionPagePrecedente_triggered(bool checked=false);
		void on_actionPageSuivante_triggered(bool checked=false);
		void on_actionDernierePage_triggered(bool checked=false);
		void on_actionPrint_triggered(bool checked=false);
};
//------------------------------------------------------------------------------
#endif // __CMAINFRM_H__
//------------------------------------------------------------------------------
