
// structureDoc.h : interface of the CstructureDoc class
//


#pragma once
#include "Layer.h"
#include "Point.h"
#include "Line.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Circle.h"


class CstructureDoc : public CDocument
{
protected: // create from serialization only
	CstructureDoc() noexcept;
	DECLARE_DYNCREATE(CstructureDoc)

// Attributes
public:
	CstructureView* pView{ nullptr };

	// Layer Array
	int layerCapacity{ 0 };
	Layer* pLayer = new Layer[layerCapacity];

	// Dialog
	Point* pPointdlg;
	Line* pLinedlg;
	Triangle* pTridlg;
	Rectangles* pRectdlg;
	Circle* pCircledlg;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CstructureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPointToolbar();
	afx_msg void OnLinesToolbar();
	afx_msg void OnLineStripToolbar();
	afx_msg void OnLineLoopToolbar();
	afx_msg void OnPolyTriToolbar();
	afx_msg void OnPolyRecToolbar();
	afx_msg void OnPolyCircleToolbar();

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
