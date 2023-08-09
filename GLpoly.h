#pragma once
#include "GLprimitive.h"
class GLpoly : public GLprimitive
{
public:
	GLpoly(const char*, const char*);
	GLpoly();

	virtual ~GLpoly();

	void setShader(const char*, const char*);

	void setIndiCapacity(int);

	enum Mode {TRIANGLES, STRIP, FAN}; 
	// TRIANGLES - for triangles
	// STRIP - for rectangles
	// FAN - for circles, etc.
	void setMode(int);
	void setRadio(int);
	
	// TRIANGLES pushVertex utility function
	void TRIANGLESLinepushVertex();
	void TRIANGLESpushVertex();

	// RECTANGELS pushVertex utility function
	void RECTANGLESLinepushVertex();
	void RECTANGLESpushVertex();
	
	// overrides
	void setVertex(float, float, float, float = 1.0f, float = 1.0f, float = 1.0f);
	void pushVertex();
	void popVertex();
	void drawing();
	void draw();

private:
	unsigned int lineVAO;
	unsigned int polyVAO;

	unsigned int EBO; // polyEBO
	unsigned int lineEBO;
	Mode mode;

	int mRadio; // for direction in RECTANGLE, CIRCLE

	int lineIndividualCapacity{ 0 };
	int individualCapacity{ 0 };

	int lineIndexCapacity{ 0 };
	int indexCapacity{ 0 };
	
	unsigned int* lineIndices = new unsigned int[0];
	unsigned int* indices = new unsigned int[0];

	float lineWidth{ 2.0f };
	float pointSize{ 2.0f };

};

