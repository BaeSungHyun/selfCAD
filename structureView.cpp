
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

#include "shader.h"


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
END_MESSAGE_MAP()

// CstructureView construction/destruction

CstructureView::CstructureView() noexcept
	: m_hRC(0), m_pDC(0)
{
	// TODO: add construction code here
}

CstructureView::~CstructureView()
{
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// TODO: add draw code for native data here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // color and depth already set
	
	DimAxis();
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 6);

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

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);

	if (GL_VERSION_3_3 > GL_VERSION) {
		MessageBox(_T("Graphic Driver doens't support OpenGL 3.3 or above"));
		OnDestroy();
	}

	return TRUE;
}

BOOL CstructureView::SetupGLAD() {
	if (NULL == gladLoadGL()) // don't know how this works exactly
		return FALSE;
	const GLubyte* version = glGetString(GL_VERSION);
	
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

BOOL CstructureView::SetupViewport(int cx, int cy) {
	glViewport(0, 0, cx, cy);
	this->cx = cx;
	this->cy = cy;
	return TRUE;
}

BOOL CstructureView::SetupProjection() {
	model = glm::mat4(1.0f); // initialize to identity matrix
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, -10.0f, -50.0f));

	// glm::ortho(0.0f, cx, 0.0f, cy, 0.1f, 100.0f);
	projection = glm::perspective(glm::radians(45.0f), cx / cy, 0.1f, 100.0f);
	// retrieve the matrix uniform locations

	return TRUE;
}

void CstructureView::DimAxis() {
	Shader axisShader("./glsl/threeaxis.vs", "./glsl/threeaxis.fs");

	float vertices[] = {   // colors
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 50.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	// need this in advance to update uniform variable in GLSL
	
	axisShader.use();

	unsigned int modelLoc = glGetUniformLocation(axisShader.ID, "model");
	unsigned int viewLoc = glGetUniformLocation(axisShader.ID, "view");
	// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	axisShader.setMat4("projection", projection);


	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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