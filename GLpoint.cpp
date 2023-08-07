#include "pch.h"
#include "GLpoint.h"

GLpoint::GLpoint(const char* verPath, const char* fragPath)
	:GLprimitive(verPath, fragPath)
{
}

GLpoint::GLpoint() {

}

void GLpoint::draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, getCapacity());
	glBindVertexArray(0);
}