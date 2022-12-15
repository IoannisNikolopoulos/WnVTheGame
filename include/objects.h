#include "base.h"

/*
   Terrain classes
   Based on Object
*/
class Tree: public Object {
	public:
		Tree(Point _position): Object(_position) {};
		void print();
};

class Water: public Object {
	public:
		Water(Point _position): Object(_position) {};
		void print();
};

class MedKit: public Object {
	public:
		MedKit(Point _position): Object(_position) {};
		void print();
};
