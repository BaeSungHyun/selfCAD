#include "pch.h"
#include "framework.h"
#include "structure.h"

#include "structureDoc.h"
#include "structureView.h"

// in OpenGL coordinates on screen
void CstructureView::rayCoordinates(float mouseX, float mouseY, glm::vec4& nearest, glm::vec4& farthest) {
	glm::vec4 posNear{ mouseX * 2 / cx - 1.0f, mouseY * 2 / cy - 1.0f, -1.0f, 1.0f};
	glm::vec4 posFar{mouseX * 2 / cx - 1.0f, mouseY * 2 / cy - 1.0f, 1.0f, 1.0f};


	nearest = glm::inverse(model) * glm::inverse(view) * glm::inverse(camera) * glm::inverse(projection) * posNear;
	farthest = glm::inverse(model) * glm::inverse(view) * glm::inverse(camera) * glm::inverse(projection) * posFar;
	
//	CString str;
//	str.Format(_T("%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f"),cx, cy, mouseX * 2 / cx - 1.0f, mouseY * 2 / cy - 1.0f,  nearest.x, nearest.y, nearest.z, nearest.w, farthest.x, farthest.y, farthest.z, nearest.w);
//	MessageBox(str);
}

// Used differetial. But vector and cross product might have been faster.
BOOL CstructureView::rayPoint(const glm::vec4& nearest, const glm::vec4& farthest, const float x3, const float y3, const float z3) {
	// Needs " / (vec4).w " for real coordinate
	float x1 = nearest.x / nearest.w;
	float y1 = nearest.y / nearest.w;
	float z1 = nearest.z / nearest.w;
	
	float x2 = farthest.x / farthest.w;
	float y2 = farthest.y / farthest.w;
	float z2 = farthest.z / farthest.w;

	double distanceX = (x3 - x1 + ((x1-x2) * ((x1 - x2) * (x1 - x3) + (y1 - y2) * (y1 - y3) + (z1 - z2) * (z1 - z3))) / ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2)));
	double distanceY = (y3 - y1 + ((y1-y2) * ((x1 - x2) * (x1 - x3) + (y1 - y2) * (y1 - y3) + (z1 - z2) * (z1 - z3))) / ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2)));
	double distanceZ = (z3 - z1 + ((z1-z2) * ((x1 - x2) * (x1 - x3) + (y1 - y2) * (y1 - y3) + (z1 - z2) * (z1 - z3))) / ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2)));
	
//	CString str;
//	str.Format(_T("%f %f %f"), distanceX, distanceY, distanceZ);
//	MessageBox(str);

	if (sqrt(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ) < 1) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CstructureView::rayLine(const glm::vec4& nearest, const glm::vec4& farthest, const float x3, const float y3, const float z3, const float x4, const float y4, const float z4) {
	float x1 = nearest.x / nearest.w;
	float y1 = nearest.y / nearest.w;
	float z1 = nearest.z / nearest.w;

	float x2 = farthest.x / farthest.w;
	float y2 = farthest.y / farthest.w;
	float z2 = farthest.z / farthest.w;

	double fraction = sqrt(((x1 - x2) * (y3 - y4) - (x3 - x4) * (y1 - y2)) * ((x1 - x2) * (y3 - y4) - (x3 - x4) * (y1 - y2)) + ((x1 - x2) * (z3 - z4) - (x3 - x4) * (z1 - z2)) * ((x1 - x2) * (z3 - z4) - (x3 - x4) * (z1 - z2)) + ((y1 - y2) * (z3 - z4) - (y3 - y4) * (z1 - z2)) * ((y1 - y2) * (z3 - z4) - (y3 - y4) * (z1 - z2)));
	double distance1 = ((z1 - z3) * ((x1 - x2) * (y3 - y4) - (x3 - x4) * (y1 - y2))) / fraction;
	double distance2 = (- (y1 - y3) * ((x1 - x2) * (z3 - z4) - (x3 - x4) * (z1 - z2))) / fraction;
	double distance3 = ((x1 - x3) * ((y1 - y2) * (z3 - z4) - (y3 - y4) * (z1 - z2))) / fraction;
	
	double angle = ((x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3)) * ((x1 - x4) * (y2 - y4) - (x2 - x4) * (y1 - y4)) + ((x1 - x3) * (z2 - z3) - (x2 - x3) * (z1 - z3)) * ((x1 - x4) * (z2 - z4) - (x2 - x4) * (z1 - z4)) + ((y1 - y3) * (z2 - z3) - (y2 - y3) * (z1 - z3)) * ((y1 - y4) * (z2 - z4) - (y2 - y4) * (z1 - z4));

	if (angle <= 0 && abs(distance1 + distance2  + distance3) < 0.5) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CstructureView::rayPoly(const glm::vec4& nearest, const glm::vec4& farthest, const float x3, const float y3, const float z3,
	const float x4, const float y4, const float z4, const float x5, const float y5, const float z5) {
	float x1 = nearest.x / nearest.w;
	float y1 = nearest.y / nearest.w;
	float z1 = nearest.z / nearest.w;

	float x2 = farthest.x / farthest.w;
	float y2 = farthest.y / farthest.w;
	float z2 = farthest.z / farthest.w;

	double t = ((z4 - z5) * ((x2 - x5) * (y3 - y5) - (x3 - x5) * (y2 - y5))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2))) - ((y4 - y5) * ((x2 - x5) * (z3 - z5) - (x3 - x5) * (z2 - z5))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2))) + ((x4 - x5) * ((y2 - y5) * (z3 - z5) - (y3 - y5) * (z2 - z5))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2)));
	double l = ((z2 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2))) - ((y2 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2))) + ((x2 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2)));
	double m = ((z1 - z2) * ((x2 - x5) * (y3 - y5) - (x3 - x5) * (y2 - y5))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2))) - ((y1 - y2) * ((x2 - x5) * (z3 - z5) - (x3 - x5) * (z2 - z5))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2))) + ((x1 - x2) * ((y2 - y5) * (z3 - z5) - (y3 - y5) * (z2 - z5))) / ((z3 - z5) * ((x1 - x2) * (y4 - y5) - (x4 - x5) * (y1 - y2)) - (y3 - y5) * ((x1 - x2) * (z4 - z5) - (x4 - x5) * (z1 - z2)) + (x3 - x5) * ((y1 - y2) * (z4 - z5) - (y4 - y5) * (z1 - z2)));

	if (t >= 0.0 && t <= 1.0 && l >= 0.0 && l <= 1.0 && m >= 0.0 && m <= 1.0 && l + m >= 0.0 && l + m <= 1.0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// return actual range of min, max in VBO (line)
void CstructureView::pushLINEVBOIndex(unsigned int* lineEBO, unsigned int min, unsigned int max) {
	unsigned int minVBO{ lineEBO[min] }, maxVBO{ lineEBO[max] };

	for (int i = min + 1; i < min + 3; ++i) {
		if (minVBO > lineEBO[i]) {
			minVBO = lineEBO[i];
		}
	}
	for (int i = max - 1; i > max - 3; --i) {
		if (maxVBO < lineEBO[i]) {
			maxVBO = lineEBO[i];
		}
	}

	// for existing saveIndex[4] elements
	unsigned int* temp = new unsigned int[saveCapacity[4] + maxVBO - minVBO + 1];
	for (int i = 0; i < saveCapacity[4]; ++i) {
		temp[i] = saveIndex[4][i];
	}

	for (int i = minVBO; i < maxVBO + 1; ++i)
		temp[i - minVBO + saveCapacity[4]] = i;

	delete[] saveIndex[4];
	saveIndex[4] = temp;

	saveCapacity[4] += maxVBO - minVBO + 1;
}

// return actual range of min, max in 'indices' (lineEBO)
void CstructureView::LINEIndexIdentifier(const unsigned int* saveIndex, unsigned int& min, unsigned int& max, const int LINEIndexCapacity) {
	// compare current elment's first with previous element's second
	while (min >= 1 && saveIndex[2 * min] == saveIndex[2 * min - 1]) { // at least 2 == 1
		--min;
	}
	min *= 2;
	/// compare current element's second with next element's first
	while (max < LINEIndexCapacity && saveIndex[2 * max + 1] == saveIndex[2 * max + 2]) {
		++max;
	}
	max = 2 * max + 1;
}

// return actual range of min, max in VBO (poly)
void CstructureView::pushPOLYVBOIndex(unsigned int* polyEBO, unsigned int min, unsigned int max) {
	unsigned int minVBO{ polyEBO[min] }, maxVBO{ polyEBO[max] };

	CString str;
	str.Format(_T("%d, %d, %d, %d"), min, max, minVBO, maxVBO);
	MessageBox(str);

	for (int i = min + 1; i < min + 3; ++i) {
		if (minVBO > polyEBO[i]) {
			minVBO = polyEBO[i];
		}
	}
	for (int i = max - 1; i > max - 3; --i) {
		if (maxVBO < polyEBO[i]) {
			maxVBO = polyEBO[i];
		}
	}

	// for existing saveIndex[5] elements
	unsigned int* temp = new unsigned int[saveCapacity[5] + maxVBO - minVBO + 1];
	for (int i = 0; i < saveCapacity[5]; ++i) {
		temp[i] = saveIndex[5][i];
	}

	for (int i = minVBO; i < maxVBO + 1; ++i) {
		temp[i - minVBO + saveCapacity[5]] = i;
	}

	delete[] saveIndex[5];
	saveIndex[5] = temp;

	saveCapacity[5] += maxVBO - minVBO + 1;
}

// return actual range of min, max in 'indices' (polyEBO) 
void CstructureView::POLYIndexIdentifier(const unsigned int* saveIndex, unsigned int& min, unsigned int& max, const int POLYIndexCapacity) {
	// compare current element's all three coordinates with previoous element's all three coordinates
	while (min > 0 && (saveIndex[3 * min] == saveIndex[3 * min - 3] || saveIndex[3 * min + 1] == saveIndex[3 * min - 2] || saveIndex[3 * min + 2] == saveIndex[3 * min - 1]
		|| saveIndex[3 * min] == saveIndex[3 * min - 1] || saveIndex[3 * min] == saveIndex[3 * min -2] || saveIndex[3 * min + 1] == saveIndex[3 * min -3]
		|| saveIndex[3 * min + 1] == saveIndex[3 * min - 1] || saveIndex[3 * min + 2] == saveIndex[3 * min - 3] || saveIndex[3 * min + 2] == saveIndex[3 * max - 2])) {
		--min;
	}
	min *= 3;
	while (max < POLYIndexCapacity-1 && (saveIndex[3 * max] == saveIndex[3 * max + 3] || saveIndex[3 * max + 1] == saveIndex[3 * max + 4] || saveIndex[3 * max + 2] == saveIndex[3 * max + 5]
		|| saveIndex[3 * max] == saveIndex[3 * max + 4] || saveIndex[3 * max] == saveIndex[3 * max + 5] || saveIndex[3 * max + 1] == saveIndex[3 * max + 3] 
		|| saveIndex[3 * max + 1] == saveIndex[3 * max + 5] || saveIndex[3 * max + 2] == saveIndex[3 * max + 3] || saveIndex[3 * max + 2] == saveIndex[3 * max + 4])) {
		++max;
	}
	max = 3 * max + 2;
}

// return min, max of lineEBO of POLY by going over last 3 in saveIndex[3]
// using min, max of polyEBO
void CstructureView::POLYLineIndexIdentifier(unsigned int* polyEBO, unsigned int* lineEBO, unsigned int& min, unsigned int& max) {
	// Use saveIndex[2] to get min and max 
	unsigned int maxValue{ polyEBO[max] }, minValue{ polyEBO[min] };

	for (int i = max; i > max - 3; --i) { // max , max - 1, max - 2 
		if (polyEBO[i] > maxValue) {
			maxValue = polyEBO[i];
		}
	}
	for (int i = min; i < min + 3; ++i) {
		if (polyEBO[i] < minValue) {
			minValue = polyEBO[i];
		}
	}

	min = 2 * minValue;
	max = 2 * maxValue + 1;
}