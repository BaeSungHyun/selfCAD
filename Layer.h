#pragma once
#include <afx.h>
#include "GLpoint.h"
#include "GLline.h"
#include "GLpoly.h"

class Layer : public CObject
{
public:
	DECLARE_SERIAL(Layer, CObject);

	void Serialize(CArchive& ar);

	Layer();
	virtual ~Layer();
	
	enum TYPE {POINT, LINE, POLY};

	BOOL bPoint{ FALSE };
	BOOL bLine{ FALSE };
	BOOL bPoly{ FALSE };

	GLprimitive* getPrimitive(TYPE);

	void drawing(TYPE);
	void draw();
private:
	GLprimitive* point{ nullptr }; // point, line, triangle... etc.;
	// TEMPORARY
	GLprimitive* line{ nullptr };
	GLprimitive* poly{ nullptr };
	GLprimitive* primitives[3]{ nullptr }; // array of pointer 
};

