//------------------------------------------------------------------------------
#ifndef __CFONT_H__
#define __CFONT_H__
//------------------------------------------------------------------------------
#include <QString>
#include <QFont>
//------------------------------------------------------------------------------
namespace libqt4report {
	class CFont {
		public:
			CFont(QString family, int size) { this->family=family; this->size=size;  weight="normal"; style="normal"; }
			QString getFamily(void) { return family; }
			int getSize(void) { return size; }
			void setWeight(QString weight) { this->weight=weight; }
			QString getWeight(void) { return weight; }
			void setStyle(QString style) { this->style=style; }
			QString getStyle(void) { return style; }
			void toQFont(QFont &font);
		private:
			QString family;
			int size;
			QString weight;
			QString style;
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CFONT_H__
//------------------------------------------------------------------------------