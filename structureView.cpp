
// structureView.cpp : implementation of the CstructureView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "structure.h"
#endif

#include "structureDoc.h"
#include "structureView.h"
#include <glad.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CstructureView

IMPLEMENT_DYNCREATE(CstructureView, CView)

BEGIN_MESSAGE_MAP(CstructureView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FILL_TOOLBAR, &CstructureView::OnFillToolbar)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CstructureView construction/destruction

CstructureView::CstructureView() noexcept
	: m_hRC(0), m_pDC(0)
{
	// TODO: add construction code here
	saveIndex = new unsigned int* [6];
	saveIndex[0] = new unsigned int[saveCapacity[0]]; // POINT
	saveIndex[1] = new unsigned int[saveCapacity[1]]; // LINE - lineEBO
	saveIndex[2] = new unsigned int[saveCapacity[2]]; // POLY - polyEBO
	saveIndex[3] = new unsigned int[saveCapacity[3]]; // POLY - lineEBO
	saveIndex[4] = new unsigned int[saveCapacity[4]]; // LINE - VBO
	saveIndex[5] = new unsigned int[saveCapacity[5]]; // POLY - VBO
}

CstructureView::~CstructureView()
{
	delete axisShader;
}

BOOL CstructureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CstructureView drawing

void CstructureView::OnDraw(CDC* /*pDC*/)
{
	CstructureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// TODO: add draw code for native data here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // color and depth already set

	DimAxis();
	
	glBindVertexArray(axisVAO);
	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);
	
	pDoc->pLayer->draw();

	glFinish();

	if (FALSE == SwapBuffers(m_pDC->GetSafeHdc()))
		return;
}


// CstructureView printing

BOOL CstructureView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CstructureView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CstructureView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CstructureView diagnostics

#ifdef _DEBUG
void CstructureView::AssertValid() const
{
	CView::AssertValid();
}

void CstructureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CstructureDoc* CstructureView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CstructureDoc)));
	return (CstructureDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CstructureView::InitializeOpenGL() {
	CstructureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) {
		MessageBox(_T("m_pDC gone"));
		return FALSE;
	} 

	if (!SetupPixelFormat())
		return FALSE;

	if (0 == (m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc()))) {
		MessageBox(_T("m_hRC gone"));
		return FALSE;
	}

	if (FALSE == ::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) {
		MessageBox(_T("m_hRC gone"));
		return FALSE;
	}

	if (FALSE == SetupGLAD()) {
		MessageBox(_T("glad gone"));
		return FALSE;
	}

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND); //Enable blending.
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

	if (GL_VERSION_3_3 > GL_VERSION) {
		MessageBox(_T("Graphic Driver doens't support OpenGL 3.3 or above"));
		OnDestroy();
	}

	glGenVertexArrays(1, &axisVAO);
	glGenBuffers(1, &axisVBO);
	axisShader = new Shader{ "./glsl/threeaxis.vs", "./glsl/threeaxis.fs" };

	if (pDoc->layerCapacity == 0)
		pDoc->pLayer = new Layer[++pDoc->layerCapacity]; // 2 개 생성. TopView 때문에?

	return TRUE;
}

BOOL CstructureView::SetupGLAD() {
	if (NULL == gladLoadGL()) // don't know how this works exactly
		return FALSE;
	return TRUE;
}

BOOL CstructureView::SetupPixelFormat() {
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1, // version number of data structure
		PFD_DRAW_TO_WINDOW | // buffer can draw to a window or device surface
		PFD_SUPPORT_OPENGL | // buffer supports Opengl
		PFD_DOUBLEBUFFER, // buffer is double-buffered. 
		PFD_TYPE_RGBA, // RGBA type, or Color-index value
		24, // 24 bit color depth
		0, 0, 0, 0, 0, 0, // color bits ignored
		0, // no alpha buffer
		0, // no alpha shift bit
		0, // no accumulation buffer
		0, 0, 0, 0, // accum bits ignored
		32, // 32-bit z buffer
		0, // no stencil buffer
		0, // no auxiliary buffer
		PFD_MAIN_PLANE, // MAIN LAYER
		0,
		0, 0, 0 // layer masks ignored
	};

	int pixelFormat;

	if (0 == (pixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd))) {
		return FALSE;
	}

	if (FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CstructureView::SetupViewport(float cx, float cy) {
	glViewport(0, 0, cx, cy);
	this->cx = cx;
	this->cy = cy;
	return TRUE;
}

BOOL CstructureView::SetupProjection() {
	if (FALSE == SetupModel()) {
		return FALSE;
	}

	if (FALSE == SetupView()) {
		return FALSE;
	}

	if (FALSE == SetupCamera())
		return FALSE;

	// glm::ortho(0.0f, cx, 0.0f, cy, 0.1f, 100.0f);
	projection = glm::perspective(glm::radians(zoom), cx / cy, 0.1f, 1000.0f);
	// retrieve the matrix uniform locations

	return TRUE;
}

BOOL CstructureView::SetupView() {
	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, -100.0f, -500.0f));
	return TRUE;
}

BOOL CstructureView::SetupModel() {
	model = glm::mat4(1.0f);
	model *= temp;
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return TRUE;
}

BOOL CstructureView::SetupCamera() {
	camera = glm::mat4(1.0f);
	// camera - remember there is no camera in OpenGL. It is the model that's moving.
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraDirection = glm::normalize(-cameraFront);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	// eye, center, up

	return TRUE;
}

void CstructureView::DimAxis() {
	float vertices[] = {   // colors
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		200.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 200.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 200.0f, 1.0f, 0.0f, 0.0f
	};

	// need this in advance to update uniform variable in GLSL
	if (!first) {
		(*axisShader).use();
	
		glBindVertexArray(axisVAO);
		glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), 0, GL_STATIC_DRAW);
	}
	unsigned int modelLoc = glGetUniformLocation((*axisShader).ID, "model");
	unsigned int viewLoc = glGetUniformLocation((*axisShader).ID, "view");
	// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	(*axisShader).setMat4("projection", projection);
	(*axisShader).setMat4("camera", camera);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	first = TRUE;
}

// CstructureView message handlers


int CstructureView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	InitializeOpenGL();

	return 0;
}


void CstructureView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here	
	if (0 >= cx || 0 >= cy) {
		return;
	}

	if (FALSE == SetupViewport(cx, cy))
		return;

	if (FALSE == SetupProjection())
		return;
}	



void CstructureView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (m_pDC)
		delete m_pDC;

	if (FALSE == ::wglDeleteContext(m_hRC))
		return;

}



BOOL CstructureView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	// return CView::OnEraseBkgnd(pDC);
	return TRUE;
}


void CstructureView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	lastX = static_cast<float>(point.x);
	lastY = static_cast<float>(point.y);

	if (lastX <= 0)
		lastX = 0;
	if (lastY <= 0)
		lastY = 0;

	CView::OnRButtonDown(nFlags, point);
}

void CstructureView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// rotate - origin is camera
	float xPos = static_cast<float>(point.x);
	float yPos = static_cast<float>(point.y);
	// Euclid Rotation - pitch yaw roll
	if (nFlags & MK_CONTROL && nFlags & MK_RBUTTON) {
		float xOffset = xPos - lastX;
		float yOffset = lastY - yPos;
		lastX = xPos;
		lastY = yPos;

		const float sensitivity = 0.1f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset; // around y-axis
		pitch += yOffset; // around x-axis

		// check out of bounds
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 target;
		target.x = -glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)); 
		target.y = -glm::sin(glm::radians(pitch)); // turn the other way
		target.z = -glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)); 

		cameraFront = glm::normalize(target);
	
		SetupCamera();
		Invalidate();
	}
	// move camera position
	else if (nFlags & MK_SHIFT && nFlags & MK_RBUTTON) {
		float xVec = xPos - lastX;
		float yVec = lastY - yPos;
		lastX = xPos;
		lastY = yPos;

		const float cameraSpeed = 0.1f;
		xVec *= cameraSpeed;
		yVec *= cameraSpeed;

		glm::vec3 cameraMove = glm::vec3(xVec, yVec, 0);

		cameraPos -= cameraMove;
		SetupCamera();
		Invalidate();
	}
	// Arcball rotation 
	else if (nFlags & MK_RBUTTON) {
		// find rotational axis
		xAxis = -(lastY - yPos);
		yAxis = (xPos - lastX);
		lastX = xPos;
		lastY = yPos;

		temp = glm::rotate(temp, glm::radians(sensitivity2), glm::vec3(xAxis, yAxis, 0.0f));

		SetupModel();
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CstructureView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	zoom -= static_cast<float>(zDelta / 120);
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 80.0f)
		zoom = 80.0f;


	projection = glm::perspective(glm::radians(zoom), cx / cy, 0.1f, 1000.0f);

	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CstructureView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CstructureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	float mouseX = point.x;
	float mouseY = cy - point.y;

	glm::vec4 nearV;
	glm::vec4 farV;

	BOOL found{ FALSE };
	rayCoordinates(mouseX, mouseY, nearV, farV);

	// POLY
	GLline* linePointer = reinterpret_cast<GLline*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->LINE));
	GLpoly* polyPointer = reinterpret_cast<GLpoly*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY));
	GLprimitive* pointPointer = reinterpret_cast<GLprimitive*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT));
	
	float minDistance;
	int minIndex{ -1 };
	minDistance = sqrt((nearV.x / nearV.w - farV.x / farV.w) * (nearV.x / nearV.w - farV.x / farV.w) +
		(nearV.y / nearV.w - farV.y / farV.w) * (nearV.y / nearV.w - farV.y / farV.w) +
		(nearV.z / nearV.w - farV.z / farV.w) * (nearV.z / nearV.w - farV.z / farV.w));
	float minX, minY, minZ;
	for (unsigned int i = 0; i < polyPointer->getIndexCapacity() / 3; ++i) { // multiple of 3
		if (rayPoly(nearV, farV, polyPointer->getX(polyPointer->getpIndices()[3 * i]), polyPointer->getY(polyPointer->getpIndices()[3 * i]), polyPointer->getZ(polyPointer->getpIndices()[3 * i]),
			polyPointer->getX(polyPointer->getpIndices()[3 * i + 1]), polyPointer->getY(polyPointer->getpIndices()[3 * i + 1]), polyPointer->getZ(polyPointer->getpIndices()[3 * i + 1]),
			polyPointer->getX(polyPointer->getpIndices()[3 * i + 2]), polyPointer->getY(polyPointer->getpIndices()[3 * i + 2]), polyPointer->getZ(polyPointer->getpIndices()[3 * i + 2]), minX, minY, minZ)) {
			if (minDistance > sqrt((nearV.x / nearV.w - minX) * (nearV.x / nearV.w - minX) + (nearV.y / nearV.w - minY) * (nearV.y / nearV.w - minY) + (nearV.z / nearV.w - minZ) * (nearV.z / nearV.w - minZ))) {
				minDistance = sqrt((nearV.x / nearV.w - minX) * (nearV.x / nearV.w - minX) + (nearV.y / nearV.w - minY) * (nearV.y / nearV.w - minY) + (nearV.z / nearV.w - minZ) * (nearV.z / nearV.w - minZ));
				minIndex = i;
			}
		}
	}
	if (minIndex != -1) { // means found the minimum index 
		// if SHIFT is pressed
		if ((nFlags & MK_SHIFT) && (saveCapacity[2] != 0 || saveCapacity[1] != 0 || saveCapacity[0] != 0)) {
			unsigned int min{ static_cast<unsigned int>(minIndex) }, max{ static_cast<unsigned int>(minIndex) };
			POLYIndexIdentifier(polyPointer->getpIndices(), min, max, polyPointer->getIndexCapacity() / 3);
			saveCapacity[2] += max - min + 1;
			unsigned int* temp = new unsigned int[saveCapacity[2]];
			for (unsigned int j = 0; j < saveCapacity[2] - max + min - 1; ++j) {
				temp[j] = saveIndex[2][j];
			}
			for (unsigned int j = 0; j < max - min + 1; ++j) {
				temp[j + saveCapacity[2] - max + min - 1] = j + min;
				polyPointer->setVCZ(polyPointer->getpIndices()[j + min], 0.0f);
			}
			delete[] saveIndex[2];
			saveIndex[2] = temp;

			pushPOLYVBOIndex(polyPointer->getpIndices(), min, max);

			POLYLineIndexIdentifier(polyPointer->getpIndices(), polyPointer->getpLineIndices(), min, max);

			saveCapacity[3] += max - min + 1;
			unsigned int* tempLine = new unsigned int[saveCapacity[3]];
			for (unsigned int j = 0; j < saveCapacity[3] - max + min - 1; ++j) {
				tempLine[j] = saveIndex[3][j];
			}
			for (unsigned int j = 0; j < max - min + 1; ++j) {
				tempLine[j + saveCapacity[3] - max + min - 1] = j + min;
				// Do not change the color. 
			}
				

			delete[] saveIndex[3];
			saveIndex[3] = tempLine;

			polyPointer->drawing();
			found = TRUE;
		}

		else if (saveCapacity[0] != 0 || saveCapacity[1] != 0 || saveCapacity[2] != 0) {
			// POLY
			for (unsigned int j = 0; j < saveCapacity[2]; ++j) {
				polyPointer->setVCZ(polyPointer->getpIndices()[saveIndex[2][j]], 0.9f);
			}

			delete[] saveIndex[2];
			delete[] saveIndex[3];
			delete[] saveIndex[5];
			saveCapacity[2] = 0;
			saveCapacity[3] = 0;
			saveCapacity[5] = 0;

			// LINE
			for (unsigned int j = 0; j < saveCapacity[1]; ++j) {
				linePointer->setVCX(linePointer->getpIndices()[saveIndex[1][j]], 0.0f);
				linePointer->setVCY(linePointer->getpIndices()[saveIndex[1][j]], 0.0f);
			}

			delete[] saveIndex[1];
			delete[] saveIndex[4];
			saveCapacity[1] = 0;
			saveCapacity[4] = 0;
			saveIndex[1] = new unsigned int[0];
			saveIndex[4] = new unsigned int[0];

			// POINT
			for (unsigned int j = 0; j < saveCapacity[0]; ++j) {
				pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->setVCZ(saveIndex[0][j], 1.0f);
			}

			saveCapacity[0] = 0;
			delete[] saveIndex[0];
			saveIndex[0] = new unsigned int[0];

			// POLY SPECIFIC
			unsigned int min{ static_cast<unsigned int>(minIndex) }, max{ static_cast<unsigned int>(minIndex) };
			POLYIndexIdentifier(polyPointer->getpIndices(), min, max, polyPointer->getIndexCapacity() / 2);
			saveCapacity[2] = max - min + 1;
			saveIndex[2] = new unsigned int[max - min + 1];
			for (unsigned int k = 0; k < max - min + 1; ++k) {
				saveIndex[2][k] = k + min;
				polyPointer->setVCZ(polyPointer->getpIndices()[k + min], 0.0f);
			}

			saveIndex[5] = new unsigned int[0];
			pushPOLYVBOIndex(polyPointer->getpIndices(), min, max);

			POLYLineIndexIdentifier(polyPointer->getpIndices(), polyPointer->getpLineIndices(), min, max);

			saveCapacity[3] = max - min + 1;
			saveIndex[3] = new unsigned int[max - min + 1];
			for (unsigned int k = 0; k < max - min + 1; ++k) {
				saveIndex[3][k] = k + min;
				// Do not change color
			}

			polyPointer->drawing();
			linePointer->drawing();
			pointPointer->drawing();

			found = TRUE;
		}
			
		else if (saveCapacity[2] == 0) {
			unsigned int min{ static_cast<unsigned int>(minIndex) }, max{ static_cast<unsigned int>(minIndex) }; // for going both direction (increasing / decreasing)
			POLYIndexIdentifier(polyPointer->getpIndices(), min, max, polyPointer->getIndexCapacity() / 3);
			saveCapacity[2] = max - min + 1;
			unsigned int* temp = new unsigned int[max - min + 1];
			for (unsigned int k = 0; k < max - min + 1; ++k) {
				temp[k] = k + min;
				polyPointer->setVCZ(polyPointer->getpIndices()[k + min], 0.0f);
			}
			delete[] saveIndex[2];
			saveIndex[2] = temp;
				
			pushPOLYVBOIndex(polyPointer->getpIndices(), min, max);

			POLYLineIndexIdentifier(polyPointer->getpIndices(), polyPointer->getpLineIndices(), min, max);

			saveCapacity[3] = max - min + 1;
			unsigned int* tempLine = new unsigned int[max - min + 1];
			for (unsigned int k = 0; k < max - min + 1; ++k) {
				tempLine[k] = k + min;
				// Do not change color
			}
			delete[] saveIndex[3];
			saveIndex[3] = tempLine;

			polyPointer->drawing();
			found = TRUE;
		}
	}

	// LINE
	// LINE vertecis always go by pairs. In even numbers. getCapacity() : even number.
	// saveIndex[1] : index of EBO of Line. indices[ saveIndex[1][x] ] gives index of vertex in vertices array.
	//                Using the value inside indices, 'Extrude' but be careful to avoid adding same vertex multiple times.
	for (unsigned int i = 0; i < linePointer->getIndexCapacity() / 2; ++i) {
		if (rayLine(nearV, farV, linePointer->getX(linePointer->getpIndices()[2*i]), linePointer->getY(linePointer->getpIndices()[2 * i]),
			linePointer->getZ(linePointer->getpIndices()[2*i]), linePointer->getX(linePointer->getpIndices()[2 * i + 1]), linePointer->getY(linePointer->getpIndices()[2 * i + 1]),
			linePointer->getZ(linePointer->getpIndices()[2 * i + 1]))) {
			// if SHIFT is pressed, select multiple vertices of LINE
			if ((nFlags & MK_SHIFT) && (saveCapacity[2] != 0 || saveCapacity[1] != 0 || saveCapacity[0] != 0)) {
				unsigned int min{ i }, max{ i };
				LINEIndexIdentifier(linePointer->getpIndices(), min, max, linePointer->getIndexCapacity() / 2);
				saveCapacity[1] += max - min + 1;
				unsigned int* temp = new unsigned int[saveCapacity[1]];
				for (unsigned int j = 0; j < saveCapacity[1] - max + min - 1; ++j) {
					temp[j] = saveIndex[1][j];
				}
				for (unsigned int j = 0; j < max - min + 1; ++j) {
					temp[j + saveCapacity[1] - max + min - 1] = j + min;
					linePointer->setVCX(linePointer->getpIndices()[j + min], 1.0f);
					linePointer->setVCY(linePointer->getpIndices()[j + min], 1.0f);
				}

				delete[] saveIndex[1];
				saveIndex[1] = temp;

				// remember VBO
				pushLINEVBOIndex(linePointer->getpIndices(), min, max);

				linePointer->drawing();
				found = TRUE;
				break;
			}
			else if (saveCapacity[0] != 0 || saveCapacity[1] != 0 || saveCapacity[2] != 0) {
				// POLY
				for (unsigned int j = 0; j < saveCapacity[2]; ++j) {
					polyPointer->setVCZ(polyPointer->getpIndices()[saveIndex[2][j]], 0.9f);
				}
				delete[] saveIndex[3];
				saveCapacity[3] = 0;
				saveIndex[3] = new unsigned int[0];

				delete[] saveIndex[2];
				saveCapacity[2] = 0;
				saveIndex[2] = new unsigned int[0];

				delete[] saveIndex[5];
				saveCapacity[5] = 0;
				saveIndex[5] = new unsigned int[0];

				// LINE
				for (unsigned int j = 0; j < saveCapacity[1]; ++j) {
					linePointer->setVCX(linePointer->getpIndices()[saveIndex[1][j]], 0.0f);
					linePointer->setVCY(linePointer->getpIndices()[saveIndex[1][j]], 0.0f);
				}

				delete[] saveIndex[1];
				delete[] saveIndex[4];
				saveCapacity[1] = 0;
				saveCapacity[4] = 0;

				// POINT
				for (unsigned int j = 0; j < saveCapacity[0]; ++j) {
					pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->setVCZ(saveIndex[0][j], 1.0f);
				}

				saveCapacity[0] = 0;
				delete[] saveIndex[0];
				saveIndex[0] = new unsigned int[0];

				// LINE SPECIFIC
				unsigned int min{ i }, max{ i };
				LINEIndexIdentifier(linePointer->getpIndices(), min, max, linePointer->getIndexCapacity() / 2);
				saveCapacity[1] = max - min + 1;
				saveIndex[1] = new unsigned int[max - min + 1];
				for (unsigned int k = 0; k < max - min + 1; ++k) {
					saveIndex[1][k] = k + min;
					linePointer->setVCX(linePointer->getpIndices()[k + min], 1.0f);
					linePointer->setVCY(linePointer->getpIndices()[k + min], 1.0f);
				}

				saveIndex[4] = new unsigned int[0]; // PLEASE MAKE EMPTY HEAP ARRAY
				pushLINEVBOIndex(linePointer->getpIndices(), min, max);

				polyPointer->drawing();
				linePointer->drawing();
				pointPointer->drawing();
				found = TRUE;
				break;
			}
			else if (saveCapacity[1] == 0) { // saveCapacity[1] for saving capacity of LINE vertices
				// first element equals second element in previous pair or second element equals first element in nextpair
				unsigned int min{ i }, max{ i };
				LINEIndexIdentifier(linePointer->getpIndices(), min, max, linePointer->getIndexCapacity() / 2);
				saveCapacity[1] = max - min + 1;
				unsigned int* tempOrder = new unsigned int[max - min + 1];
				for (unsigned int k = 0; k < max - min + 1; ++k) {
					tempOrder[k] = k + min;
					linePointer->setVCX(linePointer->getpIndices()[k + min], 1.0f);
					linePointer->setVCY(linePointer->getpIndices()[k + min], 1.0f);
				}
				delete[] saveIndex[1];
				saveIndex[1] = tempOrder;

				pushLINEVBOIndex(linePointer->getpIndices(), min, max);

				linePointer->drawing();
				found = TRUE;
				break;
			}
		}
	}	

	// POINT
	for (unsigned int i = 0; i < pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->getCapacity(); ++i) {
		if (rayPoint(nearV, farV, pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->getX(i),
			pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->getY(i), pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->getZ(i))) {
			// if SHIFT is pressed, select multiple POINTS
			if ((nFlags & MK_SHIFT) && (saveCapacity[2] != 0 || saveCapacity[1] != 0 || saveCapacity[0] != 0)) {
				// add one point to saveIndex[0] (POINT)
				unsigned int* temp = new unsigned int[++saveCapacity[0]];
				for (unsigned int j = 0; j < saveCapacity[0] - 1; ++j) {
					temp[j] = saveIndex[0][j];
				}
				temp[saveCapacity[0] - 1] = i; // same index as GLpoint in Layer
				delete[] saveIndex[0];
				saveIndex[0] = temp;

				pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->setVCZ(i, 0.0f);
				pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->drawing();
				found = TRUE;
				break;
			}
			// delete all and add one
			else if (saveCapacity[0] != 0 || saveCapacity[1] != 0 || saveCapacity[2] != 0) {
				// POLY
				for (unsigned int j = 0; j < saveCapacity[2]; ++j) {
					polyPointer->setVCZ(polyPointer->getpIndices()[saveIndex[2][j]], 0.9f);
				}

				delete[] saveIndex[2];
				saveCapacity[2] = 0;
				saveIndex[2] = new unsigned int[0];

				delete[] saveIndex[3];
				saveCapacity[3] = 0;
				saveIndex[3] = new unsigned int[0];

				delete[] saveIndex[5];
				saveCapacity[5] = 0;
				saveIndex[5] = new unsigned int[0];

				// LINE
				for (unsigned int j = 0; j < saveCapacity[1]; ++j) {
					linePointer->setVCX(linePointer->getpIndices()[saveIndex[1][j]], 0.0f);
					linePointer->setVCY(linePointer->getpIndices()[saveIndex[1][j]], 0.0f);
				}

				delete[] saveIndex[1];
				delete[] saveIndex[4];
				saveCapacity[1] = 0;
				saveCapacity[4] = 0;
				saveIndex[1] = new unsigned int[0];
				saveIndex[4] = new unsigned int[0];

				// POINT
				for (unsigned int j = 0; j < saveCapacity[0]; ++j) {
					pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->setVCZ(saveIndex[0][j], 1.0f);
				}

				saveCapacity[0] = 1;
				delete[] saveIndex[0];


				// POINT SPECIFIC
				saveIndex[0] = new unsigned int[1];
				saveIndex[0][0] = i;

				pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->setVCZ(i, 0.0f);

				pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->drawing();
				polyPointer->drawing();
				linePointer->drawing();

				found = TRUE;
				break;
			}
			// just add one
			else if (saveCapacity[0] == 0) {
				// add one point to saveIndex[0] (POINT)
				unsigned int* temp = new unsigned int[++saveCapacity[0]];
				temp[0] = i; // same index as GLpoint in Layer

				delete[] saveIndex[0];
				saveIndex[0] = temp;

				pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->setVCZ(i, 0.0f);
				pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->drawing();
				found = TRUE;				
				break;
			}
		}
	}

	// POINT, LINE, POLY
	// when SHIFT is not pressed and if not found. Turn off the yellow color.  
	if (!found && (saveCapacity[2] != 0 || saveCapacity[1] != 0 || saveCapacity[0] != 0)) {

		for (unsigned int i = 0; i < saveCapacity[2]; ++i) {
			polyPointer->setVCZ(polyPointer->getpIndices()[saveIndex[2][i]], 0.9f);
		}
		polyPointer->drawing();

		saveCapacity[2] = 0;
		saveCapacity[3] = 0;
		saveCapacity[5] = 0;
		delete[] saveIndex[3];
		delete[] saveIndex[2];
		delete[] saveIndex[5];
		saveIndex[2] = new unsigned int[0];
		saveIndex[3] = new unsigned int[0];
		saveIndex[5] = new unsigned int[0];

		for (unsigned int i = 0; i < saveCapacity[1]; ++i) {
			linePointer->setVCX(linePointer->getpIndices()[saveIndex[1][i]], 0.0f);
			linePointer->setVCY(linePointer->getpIndices()[saveIndex[1][i]], 0.0f);
		}
		linePointer->drawing();

		saveCapacity[1] = 0;
		saveCapacity[4] = 0;
		delete[] saveIndex[1];
		delete[] saveIndex[4];
		saveIndex[1] = new unsigned int[0];
		saveIndex[4] = new unsigned int[0];

		for (unsigned int i = 0; i < saveCapacity[0]; ++i) {
			pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->setVCZ(saveIndex[0][i], 1.0f);
		}
		pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)->drawing();

		saveCapacity[0] = 0;
		unsigned int* temp = new unsigned int[0];
		delete[] saveIndex[0];
		saveIndex[0] = temp;
	}

	Invalidate();
}

BOOL CstructureView::fill() {
	CstructureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;

	GLpoly* pointerPoly = reinterpret_cast<GLpoly*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY));
	GLline* pointerLine = reinterpret_cast<GLline*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->LINE));

	// Both line indices
	int min = saveIndex[1][0];
	int max = saveIndex[1][saveCapacity[1] - 1];

	// To make sure user selected LINE, and not POLY and POINT
	if (saveCapacity[1] != 0 && saveCapacity[0] == 0 && saveCapacity[2] == 0) {
		if (pointerLine->getpIndices()[min] != pointerLine->getpIndices()[max]) // if min and max doesn't equal
			return FALSE;
		// meaning it's looped poly line
		else if (pointerLine->getpIndices()[min] == pointerLine->getpIndices()[max]) {
			// EBO. k (lines) in LOOP LINE EBO -> 3 (vertices) * (k-2) (lines / vertices)
			// POLY
			int increment{ 3 * ((max - min + 1) / 2 - 2)  }; // k (lines) : (max - min + 1) / 2

			unsigned int* tempIndices = new unsigned int[pointerPoly->getIndexCapacity() + increment];
			for (int i = 0; i < pointerPoly->getIndexCapacity(); ++i) {
				tempIndices[i] = pointerPoly->getpIndices()[i];
			}
			int first{ pointerPoly->getCapacity() }; // before pushVertex() so before incrementing capacity
			for (int i = 0; i < increment / 3; ++i) {
				tempIndices[pointerPoly->getIndexCapacity() + 3 * i] = first;
				tempIndices[pointerPoly->getIndexCapacity() + 3 * i + 1] = pointerPoly->getCapacity() + i + 1;
				tempIndices[pointerPoly->getIndexCapacity() + 3 * i + 2] = pointerPoly->getCapacity() + i + 2;
			}
			delete[] pointerPoly->getpIndices();
			pointerPoly->getpIndices() = tempIndices;
			pointerPoly->addPolyIndexCapacity(increment);

			// LINE : just add as much as the enclosed LOOP LINE 
			increment = max - min + 1; // k (lines) : (max - min + 1) / 2 , 2 vertices per line
			
			unsigned int* tempLineIndices = new unsigned int[pointerPoly->getLineIndexCapacity() + increment];
			// Move existing vertice
			for (int i = 0; i < pointerPoly->getLineIndexCapacity(); ++i) {
				tempLineIndices[i] = pointerPoly->getpLineIndices()[i];
			}
			first = pointerPoly->getCapacity(); // before pushVertex() so before incrementing capacity
			for (int i = 0; i < increment / 2; ++i) {
				tempLineIndices[pointerPoly->getLineIndexCapacity() + 2 * i] = first + i;
				if (i == increment / 2 - 1) { // to make a enclosed loop around filled POLY
					tempLineIndices[pointerPoly->getLineIndexCapacity() + 2 * i + 1] = first;
					break;
				}
				tempLineIndices[pointerPoly->getLineIndexCapacity() + 2 * i + 1] = first + i + 1;
			}
			delete[] pointerPoly->getpLineIndices();
			pointerPoly->getpLineIndices() = tempLineIndices;
			pointerPoly->addLineIndexCapacity(increment);
			

			// VBO
			for (int i = min; i < max + 1; i += 2) { // LINE LOOP : i += 2
				pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY)->setVertex(
					pointerLine->getX(pointerLine->getpIndices()[i]),
					pointerLine->getY(pointerLine->getpIndices()[i]),
					pointerLine->getZ(pointerLine->getpIndices()[i]),
					0.9f, 0.9f, 0.9f);
				pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY)->setMode(3); // FILL
				pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY)->pushVertex();
			}
		}
	}
	else {
		return FALSE;
	}

	pointerPoly->drawing();
	pDoc->UpdateAllViews(NULL);
	pDoc->pLayer->bPoly = TRUE;

	return TRUE;
}

void CstructureView::OnFillToolbar() {
	if (FALSE == fill()) {
		MessageBox(_T("Please choose enclosed loop line"));
	}
}

void CstructureView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	// CView::OnKeyDown(nChar, nRepCnt, nFlags);

	CstructureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GLpoly* polyPointer = reinterpret_cast<GLpoly*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY));
	GLline* linePointer = reinterpret_cast<GLline*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->LINE));
	GLprimitive* pointPointer = reinterpret_cast<GLprimitive*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT));

	switch (nChar) {
	case VK_DELETE: {
		// Delete selected vertices and polyEBO, lineEBO.
		
		// POINT exists  index : 0
		if (saveCapacity[0] != 0) {
			for (unsigned int i = 0; i < saveCapacity[0] - 1; ++i) {
				if (saveIndex[0][i + 1] - saveIndex[0][i] != 1) {
					MessageBox(_T("Select only one point or Select multiple points by order of creation."));
					return;
				}
			}

			pointPointer->rangeDelete(saveIndex[0][0], saveIndex[0][saveCapacity[0] - 1]);

			delete[] saveIndex[0];
			saveCapacity[0] = 0;
			saveIndex[0] = new unsigned int[0];

			pointPointer->drawing();
		}

		// LINE exists  index : 1
		if (saveCapacity[1] != 0) {
			for (unsigned int i = 0; i < saveCapacity[4] - 1; ++i) {
				if (saveIndex[4][i + 1] - saveIndex[4][i] != 1) {
					MessageBox(_T("Select only one line or Select multiple lines by order of creation."));
					return;
				}
			}

			linePointer->rangeDelete(saveIndex[1][0], saveIndex[1][saveCapacity[1] - 1]);

			delete[] saveIndex[1];
			delete[] saveIndex[4];

			saveCapacity[1] = 0;
			saveCapacity[4] = 0;

			saveIndex[1] = new unsigned int[0];
			saveIndex[4] = new unsigned int[0];
			
			linePointer->drawing();
		}

		// POLY exists  index : 2 (poly) , 3 (line)
		if (saveCapacity[2] != 0) {
			for (unsigned int i = 0; i < saveCapacity[5] - 1; ++i) {
				if (saveIndex[5][i + 1] - saveIndex[5][i] != 1) {
					MessageBox(_T("Select only one poly or Select multiple poly by order of creation."));
					return;
				}
			}

			polyPointer->rangeDelete(saveIndex[2][0], saveIndex[2][saveCapacity[2] - 1], saveIndex[3][0], saveIndex[3][saveCapacity[3] - 1]);

			delete[] saveIndex[2];
			delete[] saveIndex[3];
			delete[] saveIndex[5];

			saveCapacity[2] = 0;
			saveCapacity[3] = 0;
			saveCapacity[5] = 0;

			saveIndex[2] = new unsigned int[0];
			saveIndex[3] = new unsigned int[0];
			saveIndex[5] = new unsigned int[0];

			polyPointer->drawing();
		}
		break;
	}
	}

	Invalidate();
}

// POLY에 saveIndex[2]를 이용해서 polyEBO에 접근. 그러면 vertex가 나오겠지. 거기서 min, max 값을 뽑을 수 있을까?
// 애초에 polyEBO 에 접근 할 때, polyEBO 안에 있는 값이 중복되면 안된다. 그걸 이용해서 EXTRUDE rectangle에 
// 보내면 풀 수 있음.