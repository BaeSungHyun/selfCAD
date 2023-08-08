#include "pch.h"
#include "GLpoly.h"

GLpoly::GLpoly(const char* verPath, const char* fragPath) {
	this->setShader(verPath, fragPath);
	setOffset(12);
	getVertex() = new float[offset];
}

GLpoly::GLpoly() {
	setOffset(12);
	getVertex() = new float[offset];
}

GLpoly::~GLpoly() {
	delete[] indices;
}

void GLpoly::setVertex(float x, float y, float z, float CX, float CY, float CZ) {
	GLprimitive::setVertex(x, y, z); // setting point coordinate and color
	getVertex()[6] = 0.0f;
	getVertex()[7] = 0.0f;
	getVertex()[8] = 0.0f;

	getVertex()[9] = CX;
	getVertex()[10] = CY;
	getVertex()[11] = CZ;

}

void GLpoly::setShader(const char* verPath, const char* fragPath) {
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lineVAO);
	glGenVertexArrays(1, &polyVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// just using the same shader.use() in dimAxis().
}

void GLpoly::setMode(int type) {
	mode = static_cast<Mode>(type);
}

void GLpoly::pushVertex() {
	// index
	switch (mode) {
	case TRIANGLES: {
		unsigned int* indexTemp = new unsigned int[indexCapacity + 1];
		for (int j = 0; j < indexCapacity; ++j) {
			indexTemp[j] = indices[j];
		}
		// before incrementing capacity of vertices. So not getCapacity() - 1
		indexTemp[indexCapacity] = getCapacity();
		delete[] indices;
		indices = indexTemp;
		++indexCapacity;
		break;
	}
	case STRIP:
		break;
	case FAN:
		break;
	}

	// vertex
	GLprimitive::pushVertex();
}

void GLpoly::popVertex() {
	while (indexCapacity % 3 != 0) {
		// index
		unsigned int* indexTemp = new unsigned int[indexCapacity - 1];
		for (int j = 0; j < indexCapacity - 1; ++j) {
			indexTemp[j] = indices[j];
		}
		delete[] indices;
		indices = indexTemp;
		--indexCapacity;
		--individualCapacity;

		// vertex
		GLprimitive::popVertex();
	}
}

void GLpoly::drawing() {
	if (indexCapacity % 3 != 0) {
		// POINT
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * getCapacity(), getpVertices(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// point color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// LINE
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * getCapacity(), getpVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCapacity, indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// line color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// POLY
	glBindVertexArray(polyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * getCapacity(), getpVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCapacity, indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// poly color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// glPolygonMode 를 effective하게 하는 방법도 있으면 좋겠다.
void GLpoly::draw() {
	// this is for mode TRIANGLES.. is it all the same for other modes?
	if (indexCapacity % 3 != 0) {
		// POINT
		glBindVertexArray(VAO);
		glPointSize(pointSize);
		glDrawArrays(GL_POINTS, 0, getCapacity());
		glBindVertexArray(0);
	}
	// LINE
	glBindVertexArray(lineVAO);
	glLineWidth(lineWidth);
	glDrawElements(GL_TRIANGLES, indexCapacity, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// POLY
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(polyVAO);
	glDrawElements(GL_TRIANGLES, indexCapacity, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/*
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, indexCapacity, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
*/