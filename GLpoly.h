#pragma once
#include "GLprimitive.h"
class GLpoly : public GLprimitive
{
public:
	GLpoly(const char*, const char*);
	GLpoly();

	virtual ~GLpoly();

	void setShader(const char*, const char*);

	enum Mode {TRIANGLES, STRIP, FAN}; 
	// TRIANGLES - for triangles
	// STRIP - for rectangles
	// FAN - for circles, etc.
	void setMode(int);
	void setVertex(float, float, float, float = 1.0f, float = 1.0f, float = 1.0f);
	void pushVertex();
	void popVertex();
	void drawing();
	void draw();

private:
	unsigned int lineVAO;
	unsigned int polyVAO;
	unsigned int EBO;
	Mode mode;


	int individualCapacity{ 0 };


	int indexCapacity{ 0 };
	
	unsigned int* indices = new unsigned int[0];

	float lineWidth{ 1.5f };
	float pointSize{ 2.0f };

};

