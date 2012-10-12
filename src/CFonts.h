//------------------------------------------------------------------------------
#ifndef __CFONTS_H__
#define __CFONTS_H__
//------------------------------------------------------------------------------
#include <QHash>
#include "CFont.h"
//------------------------------------------------------------------------------
namespace libqt4report {
	class CFonts {
		public:
			CFont * getFont(QString key) { return map->value(key); }
			void addFont(QString key, CFont *font) { map->insert(key, font); }
			static CFonts * getInstance(void);
		private:
			static CFonts *instance;
			QHash<QString, CFont *> *map;
			
			CFonts(void);
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CFONTS_H__
//------------------------------------------------------------------------------