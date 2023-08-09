#include "pch.h"
#include "GLpoly.h"


GLpoly::GLpoly(const char* verPath, const char* fragPath) {
	this->setShader(verPath, fragPath);
	addCapacity(0);
	setOffset(12);
	getVertex() = new float[offset];
	getpVertices() = new float[offset * capacity];
}

GLpoly::GLpoly() {
	addCapacity(0);
	setOffset(12);
	getVertex() = new float[offset];
	getpVertices() = new float[offset * capacity];
}

GLpoly::~GLpoly() {
	delete[] indices;
	delete[] lineIndices;
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
	glGenBuffers(1, &lineEBO);
	// just using the same shader.use() in dimAxis().
}

void GLpoly::setIndiCapacity(int input) {
	individualCapacity = input;
}

void GLpoly::setMode(int type) {
	mode = static_cast<Mode>(type);
}

void GLpoly::setRadio(int mRadio) {
	this->mRadio = mRadio;
}

void GLpoly::TRIANGLESLinepushVertex() {
	if (lineIndividualCapacity == 2) { // 2, 4
		unsigned int* indexTemp = new unsigned int[lineIndexCapacity + 4];
		for (int j = 0; j < lineIndexCapacity; ++j) {
			indexTemp[j] = lineIndices[j];
		}
		// Before incrementing getCapacity()
		indexTemp[lineIndexCapacity] = getCapacity() - 1;
		indexTemp[lineIndexCapacity + 1] = getCapacity();
		indexTemp[lineIndexCapacity + 2] = getCapacity() - 2;
		indexTemp[lineIndexCapacity + 3] = getCapacity();

		delete[] lineIndices;

		lineIndices = indexTemp;
		lineIndexCapacity += 4;
		lineIndividualCapacity = 0;
		return;
	}
	else if (lineIndividualCapacity < 2) { // 0, 1
		unsigned int* indexTemp = new unsigned int[lineIndexCapacity + 1];
		for (int j = 0; j < lineIndexCapacity; ++j) {
			indexTemp[j] = lineIndices[j];
		}
		indexTemp[lineIndexCapacity] = getCapacity();
		delete[] lineIndices;
		lineIndices = indexTemp;
		++lineIndividualCapacity;
		++lineIndexCapacity;
	}
}

void GLpoly::TRIANGLESpushVertex() {
	unsigned int* indexTemp = new unsigned int[indexCapacity + 1];
	for (int j = 0; j < indexCapacity; ++j) {
		indexTemp[j] = indices[j];
	}
	// before incrementing capacity of vertices. So not getCapacity() - 1
	indexTemp[indexCapacity] = getCapacity();
	delete[] indices;
	indices = indexTemp;
	++indexCapacity;
	++individualCapacity;
}

void GLpoly::RECTANGLESLinepushVertex() {
	switch (mRadio) {
	case 0:
	case 1: { // vertical
		if (lineIndividualCapacity == 1) {
			unsigned int* indexTemp = new unsigned int[lineIndexCapacity + 7];
			for (int j = 0; j < lineIndexCapacity; ++j) {
				indexTemp[j] = lineIndices[j];
			}
			// before incrementing getCapacity() -> so currently 1
			indexTemp[lineIndexCapacity] = getCapacity() + 2;
			indexTemp[lineIndexCapacity + 1] = getCapacity() - 1;
			indexTemp[lineIndexCapacity + 2] = getCapacity() + 1;
			indexTemp[lineIndexCapacity + 3] = getCapacity();
			indexTemp[lineIndexCapacity + 4] = getCapacity() + 1;
			indexTemp[lineIndexCapacity + 5] = getCapacity();
			indexTemp[lineIndexCapacity + 6] = getCapacity() + 2;

			delete[] lineIndices;
			lineIndices = indexTemp;

			lineIndividualCapacity = 0;
			lineIndexCapacity += 7;
			
		}
		else if (lineIndividualCapacity == 0) {
			unsigned int* indexTemp = new unsigned int[lineIndexCapacity + 1];
			for (int j = 0; j < lineIndexCapacity; ++j) {
				indexTemp[j] = lineIndices[j];
			}
			indexTemp[lineIndexCapacity] = getCapacity();

			delete[] lineIndices;
			lineIndices = indexTemp;

			++lineIndividualCapacity;
			++lineIndexCapacity;
		}
		break;
	} 
	}
}

void GLpoly::RECTANGLESpushVertex() {
	switch (mRadio) {
	case 0:
	case 1: { // vertical
		if (individualCapacity % 6 == 1) {
			unsigned int* indexTemp = new unsigned int[indexCapacity + 5];
			for (int j = 0; j < indexCapacity; ++j) {
				indexTemp[j] = indices[j];
			}
			// getCapcity() = at the start 1
			indexTemp[indexCapacity] = getCapacity() + 1;
			indexTemp[indexCapacity + 1] = getCapacity() + 2;
			indexTemp[indexCapacity + 2] = getCapacity();
			indexTemp[indexCapacity + 3] = getCapacity() + 1;
			indexTemp[indexCapacity + 4] = getCapacity() + 2;

			delete[] indices;
			indices = indexTemp;

			individualCapacity += 5;
			indexCapacity += 5;
		}
		else if (individualCapacity % 6 == 0) {
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
		break;
	}
	}
}

// indexCapacity += 2*radius*3  * 3
// lineIndexCapacity += 2*radius*3 * 2

void GLpoly::CIRCLESLinepushVertex() {
	int increment{ 2 * static_cast<int>(radius) * 3 * 2 };
	unsigned int* indexTemp = new unsigned int[lineIndexCapacity + increment];
	for (int j = 0; j < lineIndexCapacity; ++j) {
		indexTemp[j] = lineIndices[j];
	}
	// getCapacity() -> first. Remember it is before GLprimitive::pushVertex()
	int capacity{ getCapacity() };
	int first{ getCapacity() };
	for (int j = 0; j < increment/2; ++j) {
		indexTemp[lineIndexCapacity + 2*j] = capacity + 1;
		if (j == increment / 2 - 1) {
			indexTemp[lineIndexCapacity + 2 * j + 1] = first + 1;
			break;
		}
		indexTemp[lineIndexCapacity + 2*j + 1] = capacity + 2;
		++capacity;
	}
	delete[] lineIndices;
	lineIndices = indexTemp;

	lineIndexCapacity += increment;
}

void GLpoly::CIRCLESpushVertex() {
	int increment{ 2 * static_cast<int>(radius) * 3 * 3 };
	unsigned int* indexTemp = new unsigned int[indexCapacity + increment];
	for (int j = 0; j < indexCapacity; ++j) {
		indexTemp[j] = indices[j];
	}
	// getCapacity() -> first. Remember it is before GLprimitive::pushVertex()
	int capacity{ getCapacity() };
	int first{ getCapacity() };
	for (int j = 0; j < increment / 3; ++j) {
		indexTemp[indexCapacity + 3*j] = first;
		indexTemp[indexCapacity + 3*j + 1] = capacity + 1;
		if (j == increment / 3 - 1) {
			indexTemp[indexCapacity + 3 * j + 2] = first + 1;
			break;
		}
		indexTemp[indexCapacity + 3*j + 2] = capacity + 2;
		++capacity;
	}
	delete[] indices;
	indices = indexTemp;

	indexCapacity += increment;
}

void GLpoly::CIRCLEsetRadius(float radius) {
	this->radius = radius;
}


void GLpoly::pushVertex() {
	// index
	switch (mode) {
	case TRIANGLES: {
		TRIANGLESpushVertex();
		TRIANGLESLinepushVertex();

		// vertex
		GLprimitive::pushVertex();
		break;
	}
	case STRIP: {
		RECTANGLESpushVertex();
		RECTANGLESLinepushVertex();

		GLprimitive::pushVertex();

		if (mRadio == 0) {

			// vertex
			if (individualCapacity % 6 == 0) { // getCapacity() - 2 : first corner / getCapacity() - 1 : second corner
				// first corner's x and y coordinate. second corner's z coordinate
				setX(getX(getCapacity() - 2));
				setY(getY(getCapacity() - 1));
				setZ(getZ(getCapacity() - 2));

				GLprimitive::pushVertex();

				// getCapacity() changes -> first corner : getCapacity() - 3 / second corner : getCapacity() - 2
				// first conrner's z coordinate, second corner's x and y coordinates
				setX(getX(getCapacity() - 2));
				setY(getY(getCapacity() - 3));
				setZ(getZ(getCapacity() - 2));

				GLprimitive::pushVertex();
			}
		}
		else if (mRadio == 1) {
			// vertex
			if (individualCapacity % 6 == 0) { // getCapacity() - 2 : first corner / getCapacity() - 1 : second corner
				// first corner's x and y coordinate. second corner's z coordinate
				setX(getX(getCapacity() - 1)); // second
				setY(getY(getCapacity() - 2)); // first
				setZ(getZ(getCapacity() - 2)); // first

				GLprimitive::pushVertex();

				// getCapacity() changes -> first corner : getCapacity() - 3 / second corner : getCapacity() - 2
				// first conrner's z coordinate, second corner's x and y coordinates
				setX(getX(getCapacity() - 3)); // first
				setY(getY(getCapacity() - 3)); // first
				setZ(getZ(getCapacity() - 2)); // second

				GLprimitive::pushVertex();
			}
		}
		break;
	}
	case FAN: {
		CIRCLESLinepushVertex();
		CIRCLESpushVertex();
		GLprimitive::pushVertex();
		if (mRadio == 0) { // x-axis in dialog but in real z-axis
			// x = radius * cos(2*pi / 2*radius*3 * theta);
			// for (theta = 0; theta < 2*radius*3; ++theta)
			for (int theta = 0; theta < 2 * radius * 3; ++theta) {
				this->setVertex(radius * glm::cos(2 * 3.14159265358979323846264338327950288 / (2 * radius * 3) * theta), 
					radius * glm::sin(2* 3.14159265358979323846264338327950288 / (2*radius * 3) * theta), getZ(getCapacity() - 1));
				GLprimitive::pushVertex();
			}
		}
		else if (mRadio == 1) { // y-axis in dialog but in real x-axis
			for (int theta = 0; theta < 2 * radius * 3; ++theta) {
				this->setVertex(getX(getCapacity() - 1), radius * glm::cos(2 * 3.14159265358979323846264338327950288 / (2 * radius * 3) * theta)
					, radius * glm::sin(2 * 3.14159265358979323846264338327950288 / (2 * radius * 3) * theta));
				GLprimitive::pushVertex();
			}
		}
		else if (mRadio == 2) { // z-axis in dialog but in real y-axis
			for (int theta = 0; theta < 2 * radius * 3; ++theta) {
				this->setVertex(radius * glm::cos(2 * 3.14159265358979323846264338327950288 / (2 * radius * 3) * theta),
					getY(getCapacity() - 1), radius * glm::sin(2 * 3.14159265358979323846264338327950288 / (2 * radius * 3) * theta));
				GLprimitive::pushVertex();
			}
		}
		break;
	}
	}
}

void GLpoly::popVertex() {
	switch (mode) {
	case TRIANGLES: {
		while (individualCapacity % 3 != 0) {
			// poly index
			unsigned int* indexTemp = new unsigned int[indexCapacity - 1];
			for (int j = 0; j < indexCapacity - 1; ++j) {
				indexTemp[j] = indices[j];
			}
			delete[] indices;
			indices = indexTemp;
			--indexCapacity;
			--individualCapacity;

			// line index
			unsigned int* lineIndexTemp = new unsigned int[lineIndexCapacity - 1];
			for (int j = 0; j < lineIndexCapacity - 1; ++j) {
				lineIndexTemp[j] = lineIndices[j];
			}
			delete[] lineIndices;
			lineIndices = lineIndexTemp;
			--lineIndexCapacity;
			--lineIndividualCapacity;

			// vertex
			GLprimitive::popVertex();
		}
		break;
	}
	case STRIP: {
		while (individualCapacity % 6 != 0) {
			// poly index
			unsigned int* indexTemp = new unsigned int[indexCapacity - 1];
			for (int j = 0; j < indexCapacity - 1; ++j) {
				indexTemp[j] = indices[j];
			}
			delete[] indices;
			indices = indexTemp;
			--indexCapacity;
			--individualCapacity;

			// line index
			unsigned int* lineIndexTemp = new unsigned int[lineIndexCapacity - 1];
			for (int j = 0; j < lineIndexCapacity - 1; ++j) {
				lineIndexTemp[j] = lineIndices[j];
			}
			delete[] lineIndices;
			lineIndices = lineIndexTemp;
			--lineIndexCapacity;
			--lineIndividualCapacity;

			// vertex
			GLprimitive::popVertex();
		}
		break;
	}
	case FAN: {

		break;
	}
	}

}

void GLpoly::drawing() {
	switch (mode) {
	case TRIANGLES: {
		if (individualCapacity % 3 != 0) {
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

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	}
	case STRIP: {
		if (individualCapacity % 6 != 0) {
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

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		break;
	}
	case FAN: {

		break;
	}
	}

	// LINE
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * getCapacity(), getpVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * lineIndexCapacity, lineIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// line color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//*********************************************************8
	

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

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	CString str;
	str.Format(_T("%d, %d, %d"), indexCapacity, lineIndexCapacity, getCapacity());
	MessageBox(NULL, str, NULL, MB_OK);
}

// glPolygonMode 를 effective하게 하는 방법도 있으면 좋겠다.
void GLpoly::draw() {
	// this is for mode TRIANGLES.. is it all the same for other modes?
	switch (mode) {
	case TRIANGLES: {
		if (individualCapacity % 3 != 0) {
			// POINT
			glBindVertexArray(VAO);
			glPointSize(pointSize);
			glDrawArrays(GL_POINTS, 0, getCapacity());
			glBindVertexArray(0);
		}
		break;
	}
	case STRIP: {
		if (individualCapacity % 6 != 0) {
			// POINT 
			glBindVertexArray(VAO);
			glPointSize(pointSize);
			glDrawArrays(GL_POINTS, 0, getCapacity());
			glBindVertexArray(0);
		}
		break;
	}
	case FAN: {
		break;
	}
	}
	
	// LINE
	glBindVertexArray(lineVAO);
	glLineWidth(lineWidth);
	glDrawElements(GL_LINES, lineIndexCapacity, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// POLY
	glBindVertexArray(polyVAO);
	glDrawElements(GL_TRIANGLES, indexCapacity, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
