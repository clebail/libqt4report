#include <iostream>
#include <string>
#include <map>

using std::string;
using std::cout;
using std::endl;
using std::map;

class PrintableObject {
public:
	virtual ~PrintableObject(void) {}
	virtual void draw(int& y) = 0;
};

class Enr : public PrintableObject, public map<string, string> {
public:
	Enr(void) : map<string, string>() {}
	Enr *getNext(void) { return 0; }
	void draw(int &y) {
		map<string, string>::iterator it;
		for(it=begin();it!=end();it++) {
			cout << it->second;
		}
		cout << endl;
	}
};

class Group {
public:
	Group(string refer) { this->refer=refer; next=0; prev=0; }
	string getRefer(void) { return refer; }
	void setNext(Group *g) { next=g; }
	void setPrev(Group *g) { prev=g; }
	Group *getNext(void) { return next; }
	Group *getPrev(void) { return prev; }
private:
	string refer;
	Group *next;
	Group *prev;
};

class Report {
public:
	Report(void) {
		firstGroup=new Group("commande");
		lastGroup=new Group("article");
		firstGroup->setNext(lastGroup);
		lastGroup->setPrev(firstGroup);
	}
	
	void createPages() {
		Enr *curEnr=getPremEnr();
		Enr *prevEnr=0;
		bool firstEnr=true;
		bool fini=false;
		bool chgtGroup=true;
		Group *changedGroup=getFirstGroup();
		int y=0;

		eval(curEnr);
		while(!fini) {
			if(firstEnr) {
				imprime(title, y);
				firstEnr=false;
			}
			
			if(chgtGroup) {
				Group *g=changedGroup;
				do {
					imprime(getGroupHeader(g), y);
					g=g->getNext();
				}while(g != 0);
				chgtGroup=false;
			}
			
			imprime(curEnr, y);
			
			prevEnr=curEnr;
			curEnr=curEnr->getNext();
			
			if(curEnr == 0) {
				chgtGroup=true;
				fini=true;
				changedGroup=getFirstGroup();
			}else {
				chgtGroup=isGroupChanged(prevEnr, curEnr, changedGroup);
				eval(curEnr);
			}
			
			if(chgtGroup) {
				Group *g=getLastGroup();
				Group *changedGroupParent=changedGroup->getPrev();
				while(g != changedGroupParent) {
					imprime(getGroupFooter(g), y);
					g=g->getPrev();
				}
			}
		}
	}
private:
	PrintableObject *title;
	Group *firstGroup;
	Group *lastGroup;
	
	void imprime(PrintableObject *po, int& y) {}
	Enr *getPremEnr(void) { return 0; }
	Group *getFirstGroup(void) { return firstGroup; }
	Group *getLastGroup(void) { return lastGroup; }
	bool isGroupChanged(Enr *enr1, Enr *enr2, Group *changedGroup) { return false; }
	PrintableObject *getGroupHeader(Group *g) { return 0; }
	PrintableObject *getGroupFooter(Group *g) { return 0; }
	void eval(Enr *e) {}
};	

