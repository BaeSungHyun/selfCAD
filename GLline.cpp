#include "pch.h"
#include "GLline.h"

GLline::GLline(const char* verPath, const char* fragPath)
{
	this->setShader(verPath, fragPath);
	setOffset(9);
	getVertex() = new float[offset];
}

// if this is called, call setShader manually
GLline::GLline() {
	setOffset(9);
	getVertex() = new float[offset];
}

GLline::~GLline() {
	delete[] indices;
}

void GLline::setVertex(float x, float y, float z, float CX, float CY, float CZ) {
	GLprimitive::setVertex(x, y, z); // just use default point color because it's not needed
	getVertex()[6] = CX; 
	getVertex()[7] = CY;
	getVertex()[8] = CZ;
}

void GLline::setShader(const char* verPath, const char* fragPath) {
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	shader = new Shader{ verPath, fragPath };
	// just using the same shader.use() in dimAxis();
}

void GLline::addIndexCapacity(int input) {
	indexCapacity += input;
}

void GLline::setIndiCapacity(int input) {
	individualCapacity = input;
}

unsigned int* GLline::getpIndices() {
	return indices;
}

int GLline::getIndexCapacity() const {
	return indexCapacity;
}

int GLline::getIndividualCapacity() const {
	return individualCapacity;
}

void GLline::setMode(int type) {
	mode = static_cast<Mode>(type);
}

void GLline::pushVertex() {
	// index
	switch (mode) {
	case LINES: {
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
	case STRIP: {
		if (individualCapacity < 2) { // 0 or 1
			unsigned int* indexTemp = new unsigned int[indexCapacity + 1];
			for (int j = 0; j < indexCapacity; ++j) {
				indexTemp[j] = indices[j];
			}
			indexTemp[indexCapacity] = getCapacity();
			delete[] indices;
			indices = indexTemp;
			++individualCapacity;
			++indexCapacity;
		}
		else {
			unsigned int* indexTemp = new unsigned int[indexCapacity + 2];
			for (int j = 0; j < indexCapacity; ++j) {
				indexTemp[j] = indices[j];
			}
			indexTemp[indexCapacity] = getCapacity()-1;
			indexTemp[indexCapacity + 1] = getCapacity();
			delete[] indices;
			indices = indexTemp;
			indexCapacity += 2;
			individualCapacity += 2;
		}
		break;
	}
	case LOOP:
		if (individualCapacity < 2) {
			unsigned int* indexTemp = new unsigned int[indexCapacity + 1];
			for (int j = 0; j < indexCapacity; ++j) {
				indexTemp[j] = indices[j];
			}
			indexTemp[indexCapacity] = getCapacity();
			delete[] indices;
			indices = indexTemp;
			++individualCapacity;
			++indexCapacity;
		}
		else {
			unsigned int* indexTemp = new unsigned int[indexCapacity + 2];
			for (int j = 0; j < indexCapacity; ++j) {
				indexTemp[j] = indices[j];
			}
			indexTemp[indexCapacity] = getCapacity() - 1;
			indexTemp[indexCapacity + 1] = getCapacity();
			delete[] indices;
			indices = indexTemp;
			indexCapacity += 2;
			individualCapacity += 2;
		}
		break;
	}

	// vertex
	GLprimitive::pushVertex(); // increments capacity of vertex array
}

void GLline::popVertex() {
	if (indexCapacity % 2 != 0) {
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
	else
		return;
}

void GLline::loopComplete() {
	if (individualCapacity >= 2) {
		unsigned int* indexTemp = new unsigned int[indexCapacity + 2];
		for (int j = 0; j < indexCapacity; ++j) {
			indexTemp[j] = indices[j];
		}
		indexTemp[indexCapacity] = getCapacity() - 1;
		indexTemp[indexCapacity + 1] = getCapacity()-1 - individualCapacity/2;
		delete[] indices;
		indices = indexTemp;
		indexCapacity += 2;
	}
}

void GLline::drawing() {
	if (indexCapacity % 2 != 0) {
		// Point
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * getCapacity(), getpVertices(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// point color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Line
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * getCapacity(), getpVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCapacity, indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// line color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void GLline::draw() {
	if (indexCapacity % 2 != 0) {
		glBindVertexArray(VAO);
		glPointSize(pointSize);
		glDrawArrays(GL_POINTS, 0, getCapacity()); // glDrawElements 로 바꾸자.
		glBindVertexArray(0);
	}

	glBindVertexArray(lineVAO);
	glLineWidth(lineWidth);
	glDrawElements(GL_LINES, indexCapacity, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

