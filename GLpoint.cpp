#include "pch.h"
#include "GLpoint.h"

IMPLEMENT_SERIAL(GLpoint, GLprimitive, 1);

GLpoint::GLpoint(const char* verPath, const char* fragPath)
	:GLprimitive(verPath, fragPath)
{
}

GLpoint::GLpoint() 
	:GLprimitive()
{
}

void GLpoint::Serialize(CArchive& ar) {

	GLprimitive::Serialize(ar);

	if (ar.IsStoring()) {
		ar << pointSize;
	}
	else {
		ar >> pointSize;
		drawing();
	}
}

void GLpoint::draw() {
	glBindVertexArray(VAO);
	glPointSize(pointSize);
	glDrawArrays(GL_POINTS, 0, getCapacity());
	glBindVertexArray(0);
}