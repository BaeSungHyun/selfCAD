#include "pch.h"
#include "Layer.h"

IMPLEMENT_SERIAL(Layer, CObject, 1);

Layer::Layer() {
	point = new GLpoint{ "./glsl/threeaxis.vs", "./glsl/threeaxis.fs" };
	line = new GLline{ "./glsl/threeaxis.vs", "./glsl/threeaxis.fs" };
	poly = new GLpoly{ "./glsl/threeaxis.vs", "./glsl/threeaxis.fs" };
	primitives[0] = point;
	primitives[1] = line;
	primitives[2] = poly;
}

// virtual
Layer::~Layer() {
	delete point;
	delete line;
	delete poly;
}

void Layer::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
	
	if (ar.IsStoring()) {
		ar << bPoint << bLine << bPoly;
	}
	else {
		ar >> bPoint >> bLine >> bPoly;
	}
}

GLprimitive* Layer::getPrimitive(TYPE type) {
	// need GLprimitive(const GLprimitive&); constructor for this.
	return (primitives[type]);
}


void Layer::drawing(TYPE type) {
		primitives[type]->drawing();
}

void Layer::draw() {
	// mode : gl_filled ÀÌ·±°Å ³Ö¾îµµ ±¦ÂúÀ»µí!
	if (bPoint)
		primitives[POINT]->draw();
	if (bLine)
		primitives[LINE]->draw();
	if (bPoly)
		primitives[POLY]->draw();
}