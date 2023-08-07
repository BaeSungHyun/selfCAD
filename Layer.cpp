#include "pch.h"
#include "Layer.h"

Layer::Layer() {
	
}

// virtual
Layer::~Layer() {
	delete point;
	delete line;
	delete poly;
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