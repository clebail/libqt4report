//------------------------------------------------------------------------------
#ifndef __CMAINFRM_H__
#define __CMAINFRM_H__
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QFile>
#include "ui_CMainFrm.h"
//------------------------------------------------------------------------------
class CMainFrm : public QMainWindow, private Ui::MainFrm {
	Q_OBJECT
	public:
		CMainFrm(void);
		~CMainFrm(void);
	private:
		QFile *xmlFile;
	private slots:
		void on_actionQuitter_triggered(bool checked=false);
		void on_actionOuvrir_triggered(bool checked=false);
};
//------------------------------------------------------------------------------
#endif // __CMAINFRM_H__
//------------------------------------------------------------------------------