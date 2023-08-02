
// structureView.h : interface of the CstructureView class
//

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CstructureView : public CView
{
protected: // create from serialization only
	CstructureView() noexcept;
	DECLARE_DYNCREATE(CstructureView)

// Attributes
public:
	CstructureDoc* GetDocument() const;

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
	virtual BOOL SetupViewport(int cx, int cy);

private:
	// Initializing OpenGL
	BOOL InitializeOpenGL();
	CDC* m_pDC;
	HGLRC m_hRC; // rendering context
	float cx, cy;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	unsigned int VAO;


	void DimAxis();


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in structureView.cpp
inline CstructureDoc* CstructureView::GetDocument() const
   { return reinterpret_cast<CstructureDoc*>(m_pDocument); }
#endif

