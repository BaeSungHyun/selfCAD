#include "pch.h"
#include "Layer.h"

Layer::Layer() {
}

// virtual
Layer::~Layer() {
	delete[] VAO;
}

unsigned int* Layer::getPVAO() {
	return VAO;
}

void Layer::addVAO() {
	unsigned int* temp = new unsigned int[capacity + 1];
	for (int i = 0; i < capacity; ++i) {
		temp[i] = VAO[i];
	}
	delete[] VAO;
	VAO = temp;
	++capacity;
}

void Layer::deleteVAO(int index) {
	if (capacity < 1 && index >= capacity) // capacity = 0 or below, which doesn't make sense to delete it
		return;								// also doesn't make sense if index is bigger than capacity
	unsigned int* temp = new unsigned int[capacity - 1];
	for (int i = 0; i < capacity-1; ++i) {
		if (i >= index)
			temp[i] = VAO[i + 1];
		else
			temp[i] = VAO[i];
	}
	delete[] VAO;
	VAO = temp;
	--capacity;
}