#include "pch.h"
#include "GLline.h"

IMPLEMENT_SERIAL(GLline, GLprimitive, 1);

GLline::GLline(const char* verPath, const char* fragPath)
{
	this->setShader(verPath, fragPath);
	addCapacity(0);
	setOffset(9);
	getVertex() = new float[offset];
	getpVertices() = new float[offset * capacity];
	indices = new unsigned int[0];
}

// if this is called, call setShader manually
GLline::GLline() {
	addCapacity(0);
	setOffset(9);
	getVertex() = new float[offset];
	getpVertices() = new float[offset * capacity];
	indices = new unsigned int[0];
}

void GLline::Serialize(CArchive& ar) {
	GLprimitive::Serialize(ar);

	if (ar.IsStoring()) {
		ar << indexCapacity << pointSize << lineWidth;
		for (int i = 0; i < indexCapacity; ++i) {
			ar << indices[i];
		}
	}
	else {
		ar >> indexCapacity >> pointSize >> lineWidth;

		delete[] indices;
		indices = new unsigned int[indexCapacity];

		for (int i = 0; i < indexCapacity; ++i) {
			ar >> indices[i];
		}

		drawing();
	}
}

GLline::~GLline() {
	delete[] indices;
	// delete shader in base class
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

unsigned int*& GLline::getpIndices() {
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

void GLline::LINESpushVertex() {
	unsigned int* indexTemp = new unsigned int[indexCapacity + 1];
	for (int j = 0; j < indexCapacity; ++j) {
		indexTemp[j] = indices[j];
	}
	// before incrementing capacity of vertices. So not getCapacity() - 1
	indexTemp[indexCapacity] = getCapacity();
	delete[] indices;
	indices = indexTemp;
	++indexCapacity;
}

void GLline::STRIPpushVertex() {
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
		indexTemp[indexCapacity] = getCapacity() - 1;
		indexTemp[indexCapacity + 1] = getCapacity();
		delete[] indices;
		indices = indexTemp;
		indexCapacity += 2;
		individualCapacity += 2;
	}
}

void GLline::LOOPpushVertex() {
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
}

void GLline::pushVertex() {
	// index
	switch (mode) {
	case LINES: {
		LINESpushVertex();
		break;
	}
	case STRIP: {
		STRIPpushVertex();
		break;
	}
	case LOOP: {
		LOOPpushVertex();
		break;
	}
	case COPY: {
		break;
	}
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
	if (individualCapacity > 2) {
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

void GLline::rangeDelete(unsigned int min, unsigned int max) {
	unsigned int minVertexIndex, maxVertexIndex;
	// Delete vertices (not indices)
	rangeDeleteVBO(min, max, minVertexIndex, maxVertexIndex);
	// Delete indices
	rangeDeleteLineEBO(min, max, minVertexIndex, maxVertexIndex);
}

// utility function for DELETION
void GLline::rangeDeleteLineEBO(unsigned int min, unsigned int max, unsigned int& minVertexIndex, unsigned int& maxVertexIndex) {
	unsigned int reducedCapacityVBO = maxVertexIndex - minVertexIndex + 1;

	unsigned int* tempIndices = new unsigned int[indexCapacity - max + min - 1]; // no offset
	for (int i = 0; i < min; ++i) {
		tempIndices[i] = indices[i];
	}
	for (int i = max + 1; i < indexCapacity; ++i) {
		// subtract as much as current capacity
		tempIndices[i - max + min - 1] = indices[i] - reducedCapacityVBO;
	}

	delete[] indices;
	indices = tempIndices;

	// Reduce capacity
	indexCapacity -= static_cast<int>(max - min + 1);
}

void GLline::rangeDeleteVBO(unsigned int min, unsigned int max, unsigned int& minVertexIndex, unsigned int& maxVertexIndex) {
	minVertexIndex = indices[min];
	maxVertexIndex = indices[max];

	if (minVertexIndex > indices[min + 1]) {
		minVertexIndex = indices[min + 1];
	}
	if (maxVertexIndex < indices[max - 1]) {
		maxVertexIndex = indices[max - 1];
	}

	// Reduce capacity included
	GLprimitive::rangeDelete(minVertexIndex, maxVertexIndex);
}

