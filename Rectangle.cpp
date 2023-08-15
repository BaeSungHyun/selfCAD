// Rectangle.cpp : implementation file
//

#include "pch.h"
#include "structure.h"
#include "afxdialogex.h"
#include "Rectangle.h"
#include "structureDoc.h"
#include "structureView.h"

// Rectangle dialog

IMPLEMENT_DYNAMIC(Rectangles, CDialog)

Rectangles::Rectangles(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RECT_DIALOG, pParent)
	, mRadio(-1)
	, rectX(0)
	, rectY(0)
	, rectZ(0)
{

}

Rectangles::~Rectangles()
{
}

void Rectangles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, mRadio);
	DDX_Text(pDX, IDC_RECT_X, rectZ);
	DDX_Text(pDX, IDC_RECT_Y, rectX);
	DDX_Text(pDX, IDC_RECT_Z, rectY);
}


BEGIN_MESSAGE_MAP(Rectangles, CDialog)
	ON_BN_CLICKED(IDC_RECT_ERASE_BUTTON, &Rectangles::OnBnClickedRectEraseButton)
END_MESSAGE_MAP()


// Rectangle message handlers


void Rectangles::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	float tempRectZ{ rectY };

	UpdateData(TRUE);

	GLpoly* pointerPoly = reinterpret_cast<GLpoly*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY));

	// EXCEPTIONS
	if (mRadio == -1) {
		MessageBox(_T("Select direction of rectangle"));
		return;
	}
	if (mRadio == 0 && pointerPoly->getIndiCapacity() == 1) { // vertical
		if (tempRectZ == rectY) {
			MessageBox(_T("Z coordinates must differ for vertical rectangle"));
			return;
		}
	}
	else if (mRadio == 1 && pointerPoly->getIndiCapacity() == 1) { // horizontal
		if (tempRectZ != rectY) {
			MessageBox(_T("Z coordinates must be same for horizontal rectangle"));
			return;
		}
	}

	GLprimitive* pPoly{ nullptr };
	pPoly = pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY);
	pPoly->setVertex(rectX, rectY, rectZ, 0.9f, 0.9f, 0.9f);
	pPoly->setMode(mode); // TRIANGLES, STRIP, FAN
	pPoly->setRadio(mRadio); // direction for RECTANGLE, CIRCLE
	pPoly->pushVertex();

	pPoly->drawing();
	pDoc->pLayer->bPoly = TRUE;

	pDoc->UpdateAllViews(NULL);
}


void Rectangles::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	GLprimitive* pRect{ nullptr };
	pRect = pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY);
	pRect->setMode(mode);
	pRect->setRadio(mRadio);
	pRect->popVertex();
	// popping part in case there is not enough vertices

	pRect->drawing();
	pDoc->UpdateAllViews(NULL);

	reinterpret_cast<GLpoly*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY))->setIndiCapacity(0);

	DestroyWindow();
}


void Rectangles::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	pDoc->pRectdlg = NULL;
	delete this;
}


void Rectangles::OnBnClickedRectEraseButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);

	rectX = 0;
	rectY = 0;
	rectZ = 0;

	mRadio = -1;
}
