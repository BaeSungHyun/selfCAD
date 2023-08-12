#pragma once
#include "GLprimitive.h"
class GLpoint : public GLprimitive
{
public:
	DECLARE_SERIAL(GLpoint);

	void Serialize(CArchive& ar);

	GLpoint(const char*, const char*);
	GLpoint();
	void draw();
private:
	float pointSize {3.0f};
};

