#include "pch.h"
#include "GLprimitive.h"

GLprimitive::GLprimitive() {
	setOffset(6);
	vertex = new float[offset];
}

GLprimitive::GLprimitive(const char* VerPath, const char* FragPath) {
	setShader(VerPath, FragPath);
	setOffset(6);
	vertex = new float[offset];
}

void GLprimitive::setShader(const char* VerPath, const char* FragPath) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	shader = new Shader{ VerPath, FragPath };
}

// not actually needed because we're using the same shader as axisdim()
void GLprimitive::compileShader(glm::mat4 model, glm::mat4 view, glm::mat4 camera, glm::mat4 projection) {
	(*shader).use();
	(*shader).setMat4("projection", projection);
	(*shader).setMat4("camera", camera);
	(*shader).setMat4("view", view);
	(*shader).setMat4("model", model);
}

GLprimitive::GLprimitive(const GLprimitive& old) {
	for (int k = 0; k < offset; ++k) {
		vertex[k] = old.vertex[k];
	}

	capacity = old.capacity;

	for (int i = 0; i < offset * capacity; ++i)
		vertices[i] = old.vertices[i];

	shader = old.shader;
}

GLprimitive::~GLprimitive() {
	delete[] vertices;
	delete shader;
}

// VERTEX
// check before if index is above capcity
float GLprimitive::getX(int index) const {
	return vertices[6 * (index) + 0]; 
}
// check before if index is above capcity
float GLprimitive::getY(int index) const {
	return vertices[6 * (index) + 1];
}
// check before if index is above capcity
float GLprimitive::getZ(int index) const {
	return vertices[6 * (index) + 2];
}
void GLprimitive::setX(float newX) {
	vertex[0] = newX;
}
void GLprimitive::setY(float newY) {
	vertex[1] = newY;
}
void GLprimitive::setZ(float newZ) {
	vertex[2] = newZ;
}

float*& GLprimitive::getVertex() {
	return vertex;
}

float* GLprimitive::getpVertices() {
	return vertices;
}

// COLOR
float GLprimitive::getCX(int index) const {
	return vertices[6 * (index)+3];
}
float GLprimitive::getCY(int index) const {
	return vertices[6 * (index)+4];
}
float GLprimitive::getCZ(int index) const {
	return vertices[6 * (index)+5];
}
void GLprimitive::setCX(float newCX) {
	vertex[3] = newCX;
}
void GLprimitive::setCY(float newCY) {
	vertex[4] = newCY;
}
void GLprimitive::setCZ(float newCZ) {
	vertex[5] = newCZ;
}

// CAPACITY
int GLprimitive::getCapacity() const {
	return capacity;
}
void GLprimitive::addCapacity(int addCapacity) {
	capacity += addCapacity;
}

// MODE
void GLprimitive::setMode(int mode) {
	return;
}

// MAIN VERTEX - both point[3] and color[3]
void GLprimitive::setVertex(float x, float y, float z, float cx, float cy, float cz) {
	setX(x);
	setY(y);
	setZ(z);
	setCX(cx);
	setCY(cy);
	setCZ(cz);
}
void GLprimitive::pushVertex() {
	float* temp = new float[offset * (capacity + 1)];
	for (int i = 0; i < capacity; ++i) {
		for (int k = 0; k < offset; ++k) {
			temp[offset * i +k] = vertices[offset * i + k];
		}
	}
	for (int k = 0; k < offset; ++k) {
		temp[offset * capacity + k] = vertex[k];
	}
	delete[] vertices;
	vertices = temp;
	
	addCapacity(1);
} 

void GLprimitive::popVertex() {
	float* temp = new float[offset * (capacity)-1];
	for (int i = 0; i < capacity - 1; ++i) {
		for (int k = 0; k < offset; ++k) {
			temp[offset * i + k] = vertices[offset * i + k];
		}
	}
	delete[] vertices;
	vertices = temp;

	addCapacity(-1);
}

// DRAWING

void GLprimitive::drawing() {
	
	// make sure to push only one vertex to existing VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * 6 * getCapacity(), vertices, GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void GLprimitive::draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, getCapacity());
	glBindVertexArray(0);
}

// OVERLOAD = 
void GLprimitive::operator=(const GLprimitive & old){
	for (int k = 0; k < offset; ++k) {
		vertex[k] = old.vertex[k];
	}

	capacity = old.capacity;

	for (int i = 0; i < offset * capacity; ++i)
		vertices[i] = old.vertices[i];

	shader = old.shader;
}

// OBJECT OUTLINE
void GLprimitive::setOutline(BOOL bit) {
	outline = bit;
}

// OFFSET
void GLprimitive::setOffset(int off) {
	offset = off;
}