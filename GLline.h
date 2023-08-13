#pragma once
#include "GLprimitive.h"
class GLline : public GLprimitive
{
public:
	DECLARE_SERIAL(GLline, GLprimitive);

	GLline(const char*, const char*);
	GLline();

	void Serialize(CArchive& ar);

	virtual ~GLline();

	void setShader(const char*, const char*);

	void addIndexCapacity(int);
	int getIndexCapacity() const;

	void setIndiCapacity(int);
	int getIndividualCapacity() const;

	unsigned int*& getpIndices();

	// pushVertex() utility functions
	void LINESpushVertex();
	void STRIPpushVertex();
	void LOOPpushVertex();

	enum Mode { LINES, STRIP, LOOP };

	void loopComplete();
	void setMode(int);
	// DEFAULT IS ALSO INHERITED?
	void setVertex(float, float, float, float = 1.0f, float = 1.0f, float = 1.0f);
	void pushVertex(); // override
	void popVertex(); // override
	void drawing(); // override
	void draw(); // override

private:
	unsigned int EBO;
	unsigned int lineVAO;
	Mode mode;

	int individualCapacity{ 0 };

	int indexCapacity{0};
	unsigned int* indices{ nullptr };

	float pointSize{ 2.0f };
	float lineWidth{ 1.5f };
};

