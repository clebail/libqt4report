//------------------------------------------------------------------------------
#ifndef __CFONT_H__
#define __CFONT_H__
//------------------------------------------------------------------------------
#include <QString>
#include <QFont>
//------------------------------------------------------------------------------
namespace libqt4report {
	class CFont : public QObject {
		Q_OBJECT
		Q_ENUMS(Weight);
		Q_ENUMS(Style);
		public:
			enum Weight { Light=25, Normal=50, DemiBold=63, Bold=75, Black=87 };
			enum Style { StyleNormal=0, StyleItalic, StyleOblique };
			
			CFont(QString family, int size) { this->family=family; this->size=size;  weight=Normal; style=StyleNormal; }
			QString getFamily(void) { return family; }
			int getSize(void) { return size; }
			void setWeight(QString sWeight);
			void setWeight(Weight weight) { this->weight=weight; }
			Weight getWeight(void) { return weight; }
			void setStyle(QString sStyle);
			void setStyle(Style style) { this->style=style; }
			Style getStyle(void) { return style; }
			void toQFont(QFont &font);
			
			static QString toSvgWeight(Weight weight);
			static QString toSvgStyle(Style style);
		private:
			QString family;
			int size;
			Weight weight;
			Style style;
	};
} //namespace
//------------------------------------------------------------------------------
#endif //__CFONT_H__
//------------------------------------------------------------------------------