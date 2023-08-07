#pragma once
#include <afx.h>
#include "GLpoint.h"
#include "GLline.h"

class Layer : public CObject
{
public:
	Layer();
	virtual ~Layer();

	static enum TYPE {POINT, LINE, POLY};

	BOOL bPoint{ FALSE };
	BOOL bLine{ FALSE };
	BOOL bPoly{ FALSE };

	GLprimitive* getPrimitive(TYPE);

	void drawing(TYPE);
	void draw();
private:
	GLprimitive* point = new GLpoint{ "./glsl/threeaxis.vs", "./glsl/threeaxis.fs" }; // point, line, triangle... etc.;
	// TEMPORARY
	GLprimitive* line = new GLline{ "./glsl/threeaxis.vs", "./glsl/threeaxis.fs" };
	GLprimitive* poly = new GLpoint{ "./glsl/threeaxis.vs", "./glsl/threeaxis.fs" };
	GLprimitive* primitives[3]{ point, line, poly }; // array of pointer 
};

