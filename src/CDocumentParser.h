//------------------------------------------------------------------------------
#ifndef __CDOCUMENTPARSER_H__
#define __CDOCUMENTPARSER_H__
//------------------------------------------------------------------------------
#include <QXmlDefaultHandler>
#include "CDocument.h"
#include "CItem.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CDocumentParser : public QXmlDefaultHandler {
		public:
			CDocumentParser(QString connectionName);
			bool startDocument(void);
			bool endDocument(void);
			bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts);
			bool startCDATA(void);
			bool endCDATA(void);
			bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);
			bool characters(const QString& ch);
			CDocument *getDocument(void) { return document; }
			QString errorString(void) { return lastError; }
		private:
			enum EDBType { edbtNone, edbtPageHeader, edbtDocHeader, edbtDocBody, edbtDocFooter, edbtPageFooter };
			CDocument *document;
			bool inFonts;
			bool inFields;
			bool inDatabase;
			bool inQuery;
			bool inBody;
			bool inField;
			bool inFieldExpression;
			bool inCDATA;
			EDBType curDocBand;
			CField *curField;
			QString lastError;
			QString connectionName;
			
			libqt4report::CItem * parseItem(const QXmlAttributes& atts);
			libqt4report::CField * parseField(const QXmlAttributes& atts);
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CDOCUMENTPARSER_H__
//------------------------------------------------------------------------------