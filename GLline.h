#pragma once
#include "GLprimitive.h"
class GLline : public GLprimitive
{
public:
	GLline(const char*, const char*);
	GLline();

	virtual ~GLline();

	void setShader(const char*, const char*);

	void addIndexCapacity(int);
	int getIndexCapacity() const;

	void setIndiCapacity(int);
	int getIndividualCapacity() const;

	unsigned int* getpIndices();


	enum Mode { LINES, STRIP, LOOP };

	void loopComplete();
	void setMode(int);
	void pushVertex(); // override
	void drawing(); // override
	void draw(); // override

private:
	unsigned int EBO;
	Mode mode;

	int individualCapacity{ 0 };

	int indexCapacity{0};
	unsigned int* indices = new unsigned int[0];

	float lineWidth{1.5f};
};

