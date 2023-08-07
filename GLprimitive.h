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
	GLprimitive(const char*, const char*);
	GLprimitive(const GLprimitive&);
	GLprimitive();
	virtual ~GLprimitive();

	// shader
	virtual void setShader(const char*, const char*);
	virtual void compileShader(glm::mat4, glm::mat4, glm::mat4, glm::mat4);

	// main vertex[6] - both point[3] and color[3] 
	virtual void setVertex(float, float, float, float = 0.0f, float = 0.0f, float = 0.0f);
	virtual void pushVertex();

	// vertex
	virtual float getX(int) const;
	virtual float getY(int) const;
	virtual float getZ(int) const;

	virtual void setX(float);
	virtual void setY(float);
	virtual void setZ(float);

	virtual float* getVertex();

	// color
	virtual float getCX(int) const;
	virtual float getCY(int) const;
	virtual float getCZ(int) const;

	virtual void setCX(float);
	virtual void setCY(float);
	virtual void setCZ(float);

	// capacity
	virtual void setCapacity(int);
	virtual int getCapacity() const;

	// how to paint
	virtual void drawing();
	virtual void draw();

	void operator=(const GLprimitive&);

	// set object outlinining
	void setOutline(BOOL);

private: // why not change to protected

	float vertex[6]{ 0.0f };
	int capacity{ 0 };
	float* vertices{ new float[6 * capacity] };

	int indexLayer{ 0 };

	Shader* shader{ 0 };

	// whether to use object outlining or not
	BOOL outline{ FALSE };

protected:
	unsigned int VAO;
	unsigned int VBO;
};

