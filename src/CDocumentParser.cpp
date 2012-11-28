//--------------------------------------------------------------------------------------------------------------
#include <QtDebug>
#include <QSqlDatabase>
#include <QMetaType>
#include <log4cpp/Category.hh>
#include "CDocumentParser.h"
#include "CFields.h"
#include "CFonts.h"
#include "CValueType.h"
#include "CGroups.h"
//--------------------------------------------------------------------------------------------------------------
namespace libqt4report {
	static log4cpp::Category& logger = log4cpp::Category::getInstance("CDocumentParser");
	//--------------------------------------------------------------------------------------------------------------
	CDocumentParser::CDocumentParser(QString connectionName) : QObject() {
		qRegisterMetaType<CItemTextFixedObject>("CItemTextFixedObject");
		qRegisterMetaType<CItemTextFieldObject>("CItemTextFieldObject");

		qRegisterMetaType<CDbFieldObject>("CDbFieldObject");
		qRegisterMetaType<CItemLineObject>("CItemLineObject");
		qRegisterMetaType<CItemRectObject>("CItemRectObject");
		qRegisterMetaType<CCalculatedFieldObject>("CCalculatedFieldObject");
		qRegisterMetaType<CTotalFieldObject>("CTotalFieldObject");
		
		qRegisterMetaType<CValueTypeString>("CValueTypeString");
		qRegisterMetaType<CValueTypeInteger>("CValueTypeInteger");
		qRegisterMetaType<CValueTypeReal>("CValueTypeReal");
		qRegisterMetaType<CValueTypeDate>("CValueTypeDate");
		qRegisterMetaType<CValueTypeDateTime>("CValueTypeDateTime");
		
		document=0;
		inFonts=inFields=inDatabase=inQuery=inBody=inField=inCDATA=inParams=inGroups=inGroupHeaders=inGroupFooters=false;
		curDocBand=0;
		this->connectionName=connectionName;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool CDocumentParser::startDocument(void) {
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool CDocumentParser::endDocument(void) {
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool CDocumentParser::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts) {
		int i;
		
		if(qName == "document") {
			QString pageWidth, pageHeight, unit;

			for(i=0;i<atts.count();i++) {
				if(atts.localName(i) == "pageWidth") {
					pageWidth=atts.value(i);
				}else if(atts.localName(i) == "pageHeight") {
					pageHeight=atts.value(i);
				}else if(atts.localName(i) == "unit") {
					unit=atts.value(i);
				}
			}
			document=new CDocument(pageWidth, pageHeight, unit, connectionName);
			
			return true;
		}
		
		if(qName == "fonts") {
			inFonts=true;
			return true;
		}
		
		if(qName == "fields" || qName == "dbFields") {
			inFields=true;
			return true;
		}
		
		if(qName == "database") {
			inDatabase=true;
			return true;
		}
		
		if(qName == "groups") {
			inGroups=true;
			return true;
		}
		
		if(qName == "params" && inDatabase) {
			inParams=true;
			return true;
		}
		
		if(qName == "param" && inParams) {
			QString id;
			QVariant value;
			
			for(i=0;i<atts.count();i++) {
				if(atts.localName(i) == "id") {
					id=atts.value(i);
				}
			}
			
			emit queryParam(id, value);
			if(value.isValid()) {
				document->setParamValue(id, value);
			}
			
			return true;
		}
		
		if(qName == "query" && inDatabase) {
			inQuery=true;
			return true;
		}
		
		if(qName == "group" && inGroups) {
			QString id, refer;
			
			for(i=0;i<atts.count();i++) {
				if(atts.localName(i) == "id") {
					id=atts.value(i);
				}else if(atts.localName(i) == "refer") {
					refer=atts.value(i);
				}
			}
			
			CGroups::getInstance()->addGroup(id, refer);
			
			return true;
		}
		
		if(qName == "font" && inFonts) {
			QString id, family;
			int size;
			QString weight, style;
			CFont *font;
			
			for(i=0;i<atts.count();i++) {
				if(atts.localName(i) == "id") {
					id=atts.value(i);
				}else if(atts.localName(i) == "family") {
					family=atts.value(i);
				}else if(atts.localName(i) == "size") {
					size=atts.value(i).toInt();
				}else if(atts.localName(i) == "weight") {
					weight=atts.value(i);
				}else if(atts.localName(i) == "style") {
					style=atts.value(i);
				}
			}
			font=new CFont(family, size);
			if(!weight.isEmpty()) {
				font->setWeight(weight);
			}
			if(!style.isEmpty()) {
				font->setStyle(style);
			}
			
			CFonts::getInstance()->addFont(id, font);
			
			return true;
		}
		
		if((qName == "field" || qName == "dbField") && inFields) {
			CField *field=parseField(qName, atts);
			QString id=field->getAttribute("id");
			
			CFields::getInstance()->addField(id, field);
			
			curField=field;
			
			inField=true;
			
			return true;
		}
		
		if(qName == "expression" && curField != 0 && inField) {
			inFieldExpression=true;
			return true;
		}
		
		if(qName == "connection" && inDatabase) {
			QString driver, host, userName, password, dbName;
			
			for(i=0;i<atts.count();i++) {
				if(atts.localName(i) == "driver") {
					driver=atts.value(i);
				}else if(atts.localName(i) == "host") {
					host=atts.value(i);
				}else if(atts.localName(i) == "userName") {
					userName=atts.value(i);
				}else if(atts.localName(i) == "password") {
					password=atts.value(i);
				}else if(atts.localName(i) == "dbName") {
					dbName=atts.value(i);
				}
			}
			
			document->setDatabaseInfos(driver, host, userName, password, dbName);
			
			return true;
		}
		
		if(qName == "body") {
			inBody=true;
			return true;
		}
		
		if(qName == "pageHeader" && inBody) {
			curDocBand=document->createPageHeader();
			return true;
		}
		
		if(qName == "docHeader" && inBody) {
			curDocBand=document->createDocHeader();
			return true;
		}
		
		if(qName == "docBody" && inBody) {
			curDocBand=document->createDocBody();
			return true;
		}
		
		if(qName == "docFooter" && inBody) {
			curDocBand=document->createDocFooter();
			return true;
		}
		
		if(qName == "pageFooter" && inBody) {
			curDocBand=document->createPageFooter();
			return true;
		}
		
		if(qName == "groupHeaders" && inBody) {
			inGroupHeaders=true;
			return true;
		}
		
		if(qName == "groupFooters" && inBody) {
			inGroupFooters=true;
			return true;
		}
		
		if((qName == "groupHeader" && inGroupHeaders) || (qName == "groupFooter" && inGroupFooters)) {
			QString groupId;
			
			for(i=0;i<atts.count();i++) {
				if(atts.localName(i) == "groupId") {
					groupId=atts.value(i);
				}
			}
			
			curDocBand=new CDocBand();
			document->addGroupBand(CGroups::getInstance()->getGroup(groupId), 
								(qName == "groupHeader" ? CDocument::egbHeader : CDocument::egbFooter), curDocBand);
			return true;
		}
		
		if(qName == "item" && curDocBand != 0) {
			CItem *item=parseItem(atts);
			if(item != 0) {
				curDocBand->insert(item->getAttribute("id"), item);
			}
			return true;
		}
		
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool CDocumentParser::startCDATA(void) {
		inCDATA=true;
		
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool CDocumentParser::endCDATA(void) {
		inCDATA=false;
		
		return true;
	}
//--------------------------------------------------------------------------------------------------------------
	bool CDocumentParser::endElement(const QString& namespaceURI, const QString& localName, const QString& qName) {
		if(qName == "document") {
			return true;
		}
		
		if(qName == "fonts") {
			inFonts=false;
			return true;
		}
		
		if(qName == "fields" || qName == "dbFields") {
			inFields=false;
			if(!CFields::getInstance()->processDepends()) {
				lastError=CFields::getInstance()->getDependsError();
				return false;
			}
			return true;
		}
		
		if(qName == "database") {
			inDatabase=false;
			return true;
		}
		
		if(qName == "groups") {
			inGroups=false;
			return true;
		}
		
		if(qName == "params") {
			inParams=false;
			return true;
		}
		
		if(qName == "query" && inDatabase) {
			inQuery=false;
			return true;
		}
		
		if(qName == "inBody") {
			inBody=false;
			return true;
		}
		
		if(inBody && (qName == "pageHeader" || qName == "docHeader" || qName == "docBody"|| qName == "docFooter" || qName == "pageFooter")) {
			curDocBand=0;
			return true;
		}
		
		if((qName == "groupHeader" && inGroupHeaders) || (qName == "groupFooter" && inGroupFooters)) {
			curDocBand=0;
			return true;
		}
		
		if((qName == "field" || qName == "dbField") && inFields) {
			curField=0;
			inField=false;
			return true;
		}
		
		if(qName == "expression" && curField != 0 && inField) {
			inFieldExpression=false;
			return true;
		}
		
		
		
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool CDocumentParser::characters(const QString& ch) {
		if(inCDATA) {
			if(inQuery) {
				document->setQuery(ch);
				return true;
			}
			
			if(inFieldExpression && curField != 0) {
				CCalculatedFieldObject *cfo=static_cast<CCalculatedFieldObject *>(curField);
				if(cfo != 0) {
					try {
						cfo->setExpression(ch);
					}catch(QString *e) {
						lastError=*e;
						return false;
					}
				}
				return true;
			}
		}
		
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	CItem * CDocumentParser::parseItem(const QXmlAttributes& atts) {
		int i;
		QString className;
		CItem * item=0;
		
		i=atts.index("http://www.w3.org/2001/XMLSchema-instance", "type");
		if(i != -1) {
			className="CItem"+atts.value(i).left(1).toUpper()+atts.value(i).mid(1);
				
			int id=QMetaType::type(className.toUtf8().data());
			
			if(id != 0) {
				item=static_cast<CItem *>(QMetaType::construct(id));
				item->processAttributes(atts);
			}
		}
		
		return item;
	}
	//--------------------------------------------------------------------------------------------------------------
	CField * CDocumentParser::parseField(QString qName, const QXmlAttributes& atts) {
		int i;
		QString className;
		CField * field=0;
		
		if(qName == "dbField") {
			field=new CDbFieldObject();
		}else {
			i=atts.index("http://www.w3.org/2001/XMLSchema-instance", "type");
			if(i != -1) {
				className="C"+atts.value(i).left(1).toUpper()+atts.value(i).mid(1);
					
				int id=QMetaType::type(className.toUtf8().data());
				
				if(id != 0) {
					field=static_cast<CField *>(QMetaType::construct(id));
				}
			}
		}
		
		if(field != 0) {
			field->processAttributes(atts);
		}
		
		return field;
	}
	//--------------------------------------------------------------------------------------------------------------
}
