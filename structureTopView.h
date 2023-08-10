#pragma once
#include "structureView.h"
class CstructureTopView : public CstructureView
{
protected:
	CstructureTopView() noexcept;
	DECLARE_DYNCREATE(CstructureTopView);
	virtual ~CstructureTopView();

// overrides
public:
	virtual void OnDraw(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

