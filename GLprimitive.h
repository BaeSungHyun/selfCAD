#pragma once
#include <afx.h>
#include <glad.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLprimitive : public CObject
{
public:
	DECLARE_SERIAL(GLprimitive);

	void Serialize(CArchive&);

	GLprimitive(const char*, const char*);
	GLprimitive(const GLprimitive&);
	GLprimitive();
	virtual ~GLprimitive();

	// shader
	virtual void setShader(const char*, const char*);
	virtual void compileShader(glm::mat4, glm::mat4, glm::mat4, glm::mat4);

	// main vertex[6] - both point[3] and color[3] 
	// point color to default 1.0f 1.0f 1.0f
	virtual void setVertex(float, float, float, float = 1.0f, float = 1.0f, float = 1.0f);
	virtual void pushVertex();
	virtual void popVertex();

	// vertex
	virtual float getX(int) const;
	virtual float getY(int) const;
	virtual float getZ(int) const;

	virtual void setX(float);
	virtual void setY(float);
	virtual void setZ(float);

	virtual float*& getVertex();
	virtual float*& getpVertices();

	// color
	virtual float getCX(int) const;
	virtual float getCY(int) const;
	virtual float getCZ(int) const;

	virtual void setVCX(int, float);
	virtual void setVCY(int, float);
	virtual void setVCZ(int, float);

	virtual void setCX(float);
	virtual void setCY(float);
	virtual void setCZ(float);

	// capacity
	virtual void addCapacity(int);
	virtual int getCapacity() const;

	// mode
	virtual void setMode(int);
	virtual void setRadio(int);

	// how to paint
	virtual void drawing();
	virtual void draw();

	void operator=(const GLprimitive&);

	// set object outlinining
	void setOutline(BOOL);

	// offset
	void setOffset(int);

private: // why not change to protected
	int indexLayer;

	// whether to use object outlining or not
	BOOL outline;

	float* vertex{ nullptr }; // contains coordinate and color
	float* vertices{ nullptr };

protected:
	unsigned int VAO;
	unsigned int VBO;
	Shader* shader{ 0 };

	int capacity;
	int offset;
};

