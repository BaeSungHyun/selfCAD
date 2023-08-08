#include "pch.h"
#include "GLpoint.h"

GLpoint::GLpoint(const char* verPath, const char* fragPath)
	:GLprimitive(verPath, fragPath)
{
}

GLpoint::GLpoint() 
	:GLprimitive()
{
}

void GLpoint::draw() {
	glBindVertexArray(VAO);
	glPointSize(pointSize);
	glDrawArrays(GL_POINTS, 0, getCapacity());
	glBindVertexArray(0);
}