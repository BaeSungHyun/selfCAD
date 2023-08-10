#include "pch.h"
#include "structureTopView.h"
#include "structure.h"
#include "structureDoc.h"


IMPLEMENT_DYNCREATE(CstructureTopView, CstructureView);

BEGIN_MESSAGE_MAP(CstructureTopView, CstructureView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CstructureTopView::CstructureTopView() noexcept
:CstructureView::CstructureView(){
}

CstructureTopView::~CstructureTopView() {

}

void CstructureTopView::OnDraw(CDC* pDC) {
	CstructureView::OnDraw(NULL);
}

int CstructureTopView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CstructureView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CstructureTopView::OnDestroy()
{
	CstructureView::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CstructureTopView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}




void CstructureTopView::OnSize(UINT nType, int cx, int cy)
{
	CstructureView::OnSize(nType, cx, cy);
}
