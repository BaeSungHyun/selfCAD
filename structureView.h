
// structureView.h : interface of the CstructureView class
//

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "Layer.h"
#include "Point.h"
#include "Line.h"

class CstructureView : public CView
{
protected: // create from serialization only
	CstructureView() noexcept;
	DECLARE_DYNCREATE(CstructureView)

		// Attributes
public:
	CstructureDoc* GetDocument() const;

	// Layer Array
	int layerCapacity{ 0 };
	Layer* pLayer = new Layer[layerCapacity];


	// Point Dialog
	Point* pPointdlg;
	Line* pLinedlg;


	// transformations
	glm::mat4 model{glm::mat4(1.0f)};
	glm::mat4 view{glm::mat4(1.0f)};
	glm::mat4 camera{glm::mat4(1.0f)};
	glm::mat4 projection{glm::mat4(1.0f)};

protected:
	CToolBar m_wndToolbar;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


	// Implementation
public:
	virtual ~CstructureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Initialize
	virtual BOOL SetupPixelFormat();
	virtual BOOL SetupGLAD();

	// local space -> world space -> view space -> clip space -> screen space
	virtual BOOL SetupProjection();
	virtual BOOL SetupView();
	virtual BOOL SetupModel();
	virtual BOOL SetupViewport(float cx, float cy);
	virtual BOOL SetupCamera();

private:
	// Initializing OpenGL
	BOOL InitializeOpenGL();
	CDC* m_pDC;
	HGLRC m_hRC; // rendering context
	float cx, cy;

	// camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraDirection = glm::normalize(-cameraFront);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// rotation by origin on camera
	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX;
	float lastY;

	// rotation by origin on world
	const float sensitivity2{ 0.5f };
	glm::mat4 temp{glm::mat4(1.0f)};
	float xAxis;
	float yAxis;

	// zoom
	float zoom{ 45.0f };


	// axis VAO stored for defalut
	unsigned int axisVAO;
	unsigned int axisVBO;
	Shader* axisShader{ nullptr };

	// Axis
	BOOL first{ FALSE };
	void DimAxis();


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPointToolbar();
	afx_msg void OnLinesToolbar();
	afx_msg void OnLineStripToolbar();
	afx_msg void OnLineLoopToolbar();
};

#ifndef _DEBUG  // debug version in structureView.cpp
inline CstructureDoc* CstructureView::GetDocument() const
   { return reinterpret_cast<CstructureDoc*>(m_pDocument); }
#endif

