#pragma once
#include "GLprimitive.h"
class GLpoint : public GLprimitive
{
public:
	GLpoint(const char*, const char*);
	GLpoint();
	void draw();
private:
	float pointSize {2.0f};
};

