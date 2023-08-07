#include "pch.h"
#include "GLprimitive.h"

GLprimitive::GLprimitive() {

}

GLprimitive::GLprimitive(const char* VerPath, const char* FragPath) {
	setShader(VerPath, FragPath);
}

void GLprimitive::setShader(const char* VerPath, const char* FragPath) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	shader = new Shader{ VerPath, FragPath };
}

void GLprimitive::compileShader(glm::mat4 model, glm::mat4 view, glm::mat4 camera, glm::mat4 projection) {
	(*shader).use();

	(*shader).setMat4("projection", projection);
	(*shader).setMat4("camera", camera);
	(*shader).setMat4("view", view);
	(*shader).setMat4("model", model);
}

GLprimitive::GLprimitive(const GLprimitive& old) {
	vertex[0] = old.vertex[0];
	vertex[1] = old.vertex[1];
	vertex[2] = old.vertex[2];
	vertex[3] = old.vertex[3];
	vertex[4] = old.vertex[4];
	vertex[5] = old.vertex[5];

	capacity = old.capacity;

	for (int i = 0; i < 6 * capacity; ++i)
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

float* GLprimitive::getVertex() {
	return vertex;
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
void GLprimitive::setCapacity(int addCapacity) {
	capacity += addCapacity;
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
	float* temp = new float[6*(capacity + 1)];
	for (int i = 0; i < capacity; ++i) {
		temp[6 * i] = vertices[6 * i];
		temp[6 * i + 1] = vertices[6 * i + 1];
		temp[6 * i + 2] = vertices[6 * i + 2];
		temp[6 * i + 3] = vertices[6 * i + 3];
		temp[6 * i + 4] = vertices[6 * i + 4];
		temp[6 * i + 5] = vertices[6 * i + 5];
	}
	temp[6 * capacity] = vertex[0];
	temp[6 * capacity + 1] = vertex[1];
	temp[6 * capacity + 2] = vertex[2];
	temp[6 * capacity + 3] = vertex[3];
	temp[6 * capacity + 4] = vertex[4];
	temp[6 * capacity + 5] = vertex[5];

	delete[] vertices;
	vertices = temp;
	
	setCapacity(1);
} 


// DRAWING
void GLprimitive::drawing() {
	
	// make sure to push only one vertex to existing VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * 6 * getCapacity(), vertices, GL_STATIC_DRAW);

	CString str;
	str.Format(_T("%f, %f, %f"), vertex[0], vertex[1], vertex[2]);
	MessageBox(NULL, str, NULL, MB_OK);
	
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
	vertex[0] = old.vertex[0];
	vertex[1] = old.vertex[1];
	vertex[2] = old.vertex[2];
	vertex[3] = old.vertex[3];
	vertex[4] = old.vertex[4];
	vertex[5] = old.vertex[5];

	capacity = old.capacity;

	for (int i = 0; i < 6 * capacity; ++i)
		vertices[i] = old.vertices[i];

	shader = old.shader;
}

// OBJECT OUTLINE
void GLprimitive::setOutline(BOOL bit) {
	outline = bit;
}