//------------------------------------------------------------------------------
#include <QtDebug>
#include <log4cpp/Category.hh>
#include "CFonts.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	CFonts *CFonts::instance=0;
	static log4cpp::Category& logger = log4cpp::Category::getInstance("CFonts");
	//------------------------------------------------------------------------------
	void CFonts::cleanup(void) {
		QHashIterator<QString, CFont *> i(*map);
		while (i.hasNext()) {
			i.next();
			delete i.value();
		}
		delete this;
		
		instance=0;
	}
	//------------------------------------------------------------------------------
	void CFonts::serialize(QDataStream &out) {
		QHashIterator<QString, CFont *> i(*map);
		
		logger.debug("Serialize fonts");
		
		out << map->size();
		while (i.hasNext()) {
			i.next();
			
			logger.debug((QString("Serialize font id ")+i.key()).toStdString());
			
			out << i.key();
			i.value()->serialize(out);
		}
	}
	//------------------------------------------------------------------------------
	void CFonts::fromCache(QDataStream &in, qint32 size) {
		
		logger.debug("Read fonts from cache");
		
		for(int i=0;i<size;i++) {
			QString id;
			CFont *font;
			
			in >> id;
			
			logger.debug((QString("Read font ")+id+" from cache").toStdString());
			
			font=CFont::fromCache(in);
			map->insert(id, font);
		}
	}
	//------------------------------------------------------------------------------
	CFonts * CFonts::getInstance(void) {
		if(instance == 0) {
			instance=new CFonts();
		}
		return instance;
	}
	//------------------------------------------------------------------------------
	CFonts::CFonts(void) {
		map=new QHash<QString, CFont *>();
	}
	//------------------------------------------------------------------------------
}//namespace
//------------------------------------------------------------------------------