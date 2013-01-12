Name "libqt4report"

OutFile "setup-1.0.0beta3.exe"

InstallDir $PROGRAMFILES\libqt4report

DirText "Choose a destination folder for the installation of libqt4report"

Section ""

SetOutPath $INSTDIR

CreateDirectory $INSTDIR\libqt4report
CreateDirectory $INSTDIR\libqt4report\doc
CreateDirectory $INSTDIR\libqt4report\example
CreateDirectory $INSTDIR\libqt4report\include
CreateDirectory $INSTDIR\libqt4report\lib
CreateDirectory $INSTDIR\libqt4report\schema
CreateDirectory $INSTDIR\sqldrivers

File libgcc_s_dw2-1.dll
File liblog4cpp.dll
File libmysql.dll
File libqt4report.dll
File mingwm10.dll
File QtCore4.dll
File QtGui4.dll
File QtNetwork4.dll
File QtScript4.dll
File QtSql4.dll
File QtSvg4.dll
File QtXml4.dll
File QtXmlPatterns4.dll
File viewer.exe

SetOutPath $INSTDIR\libqt4report
File libqt4report\libqt4report_cs_CZ.qm
File libqt4report\libqt4report_fr_FR.qm
File libqt4report\log4cpp.properties
File libqt4report\viewer_cs_CZ.qm
File libqt4report\viewer_fr_FR.qm

SetOutPath $INSTDIR\libqt4report\doc
File libqt4report\doc\libqt4report.html

SetOutPath $INSTDIR\libqt4report\example
File libqt4report\example\makefile
File libqt4report\example\test.cpp
File libqt4report\example\testMySQL.xlqr
File libqt4report\example\testSQLite.xlqr
File libqt4report\example\createMySQL.sql
File libqt4report\example\createSQLite.sql
File libqt4report\example\testSQLite.db
File libqt4report\example\image.png

SetOutPath $INSTDIR\libqt4report\include
File libqt4report\include\libqt4report.h

SetOutPath $INSTDIR\libqt4report\lib
File libqt4report\lib\libqt4report.a

SetOutPath $INSTDIR\libqt4report\schema
File libqt4report\schema\libqt4report.xsd

SetOutPath $INSTDIR\sqldrivers
File sqldrivers\qsqlmysql4.dll
File sqldrivers\qsqlite4.dll

WriteUninstaller $INSTDIR\uninstall.exe

SectionEnd

Section "Uninstall"

Delete $INSTDIR\libgcc_s_dw2-1.dll
Delete $INSTDIR\liblog4cpp.dll
Delete $INSTDIR\libmysql.dll
Delete $INSTDIR\libqt4report.dll
Delete $INSTDIR\mingwm10.dll
Delete $INSTDIR\QtCore4.dll
Delete $INSTDIR\QtGui4.dll
Delete $INSTDIR\QtNetwork4.dll
Delete $INSTDIR\QtScript4.dll
Delete $INSTDIR\QtSql4.dll
Delete $INSTDIR\QtSvg4.dll
Delete $INSTDIR\QtXml4.dll
Delete $INSTDIR\QtXmlPatterns4.dll
Delete $INSTDIR\viewer.exe
Delete $INSTDIR\libqt4report\libqt4report_cs_CZ.qm
Delete $INSTDIR\libqt4report\libqt4report_fr_FR.qm
Delete $INSTDIR\libqt4report\log4cpp.properties
Delete $INSTDIR\libqt4report\viewer_cs_CZ.qm
Delete $INSTDIR\libqt4report\viewer_fr_FR.qm
Delete $INSTDIR\libqt4report\doc\libqt4report.html
Delete $INSTDIR\libqt4report\example\makefile
Delete $INSTDIR\libqt4report\example\test.cpp
Delete $INSTDIR\libqt4report\example\testMySQL.xlqr
Delete $INSTDIR\libqt4report\example\testSQLite.xlqr
Delete $INSTDIR\libqt4report\example\createMySQL.sql
Delete $INSTDIR\libqt4report\example\createSQLite.sql
Delete $INSTDIR\libqt4report\example\testSQLite.db
Delete $INSTDIR\libqt4report\example\image.png
Delete $INSTDIR\libqt4report\include\libqt4report.h
Delete $INSTDIR\libqt4report\lib\libqt4report.a
Delete $INSTDIR\libqt4report\schema\libqt4report.xsd
Delete $INSTDIR\sqldrivers\qsqlmysql4.dll
Delete $INSTDIR\sqldrivers\qsqlite4.dll
Delete $INSTDIR\uninstall.exe

RMDir $INSTDIR\libqt4report\doc
RMDir $INSTDIR\libqt4report\example
RMDir $INSTDIR\libqt4report\include
RMDir $INSTDIR\libqt4report\lib
RMDir $INSTDIR\libqt4report\schema
RMDir $INSTDIR\libqt4report
RMDir $INSTDIR\sqldrivers
RMDir $INSTDIR

SectionEnd
