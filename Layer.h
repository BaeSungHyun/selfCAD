#pragma once
#include <afx.h>
class Layer : public CObject
{
public:
	Layer();
	virtual ~Layer();

	unsigned int* getPVAO(); // to get a pointer to member data VAO
	void addVAO(); // add one element to VAO array
	void deleteVAO(int); // delete one element from VAO array
private:
	unsigned int* VAO = new unsigned int[capacity];
	int capacity{ 0 };
};

