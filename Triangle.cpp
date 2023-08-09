// Triangle.cpp : implementation file
//

#include "pch.h"
#include "structure.h"
#include "afxdialogex.h"
#include "Triangle.h"
#include "structureDoc.h"
#include "structureView.h"


// Triangle dialog

IMPLEMENT_DYNAMIC(Triangle, CDialog)

Triangle::Triangle(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TRIANGLE_DIALOG, pParent)
	, triX(0)
	, triY(0)
	, triZ(0)
{

}

Triangle::~Triangle()
{
}

void Triangle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POLY_X, triZ);
	DDX_Text(pDX, IDC_POLY_Y, triX);
	DDX_Text(pDX, IDC_POLY_Z, triY);
}


BEGIN_MESSAGE_MAP(Triangle, CDialog)
	ON_BN_CLICKED(IDC_TRIANGLE_ERASE_BUTTON, &Triangle::OnBnClickedTriangleEraseButton)
END_MESSAGE_MAP()


// Triangle message handlers


void Triangle::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CstructureDoc* pDoc = pView->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	UpdateData(TRUE);

	GLprimitive* pPoly{ nullptr };
	pPoly = pView->pLayer->getPrimitive(pView->pLayer->POLY);
	pPoly->setVertex(triX, triY, triZ, 0.9f, 0.9f, 0.9f); // set color of polygon to dark grey
	pPoly->setMode(mode);
	pPoly->pushVertex();

	pPoly->drawing();
	pView->pLayer->bPoly = TRUE;

	pDoc->UpdateAllViews(NULL);
}


void Triangle::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	pView->pTridlg = NULL;
	delete this;
}


void Triangle::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CstructureDoc* pDoc = pView->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GLprimitive* pTri{ nullptr };
	pTri = pView->pLayer->getPrimitive(pView->pLayer->POLY);
	pTri->popVertex();

	// if mode == 1, 2

	pTri->drawing();
	pDoc->UpdateAllViews(NULL);

	reinterpret_cast<GLpoly*>(pView->pLayer->getPrimitive(pView->pLayer->POLY))->setIndiCapacity(0);

	DestroyWindow();
}


void Triangle::OnBnClickedTriangleEraseButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);

	triX = 0;
	triY = 0;
	triZ = 0;
}
