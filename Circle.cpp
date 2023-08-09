// Circle.cpp : implementation file
//

#include "pch.h"
#include "structure.h"
#include "afxdialogex.h"
#include "Circle.h"
#include "structureDoc.h"
#include "structureView.h"


// Circle dialog

IMPLEMENT_DYNAMIC(Circle, CDialog)

Circle::Circle(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CIRCLE_DIALOG, pParent)
	, mRadio(-1)
	, circleX(0)
	, circleY(0)
	, circleZ(0)
	, circleRadius(5)
{

}

Circle::~Circle()
{
}

void Circle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, mRadio);
	DDX_Text(pDX, IDC_CIRCLE_X, circleZ);
	DDX_Text(pDX, IDC_CIRCLE_Y, circleX);
	DDX_Text(pDX, IDC_CIRCLE_Z, circleY);
	DDX_Text(pDX, IDC_CIRCLE_RADIUS, circleRadius);
}


BEGIN_MESSAGE_MAP(Circle, CDialog)
	ON_BN_CLICKED(IDC_CIRCLE_ERASE_BUTTON, &Circle::OnBnClickedCircleEraseButton)
END_MESSAGE_MAP()


// Circle message handlers


void Circle::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CstructureDoc* pDoc = pView->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	UpdateData(TRUE);

	if (mRadio == -1) {
		MessageBox(_T("Select normal of circle"));
		return;
	}

	if (circleRadius <= 0)
	{
		MessageBox(_T("Radius must be bigger than 0"));
		return;
	}

	GLprimitive* pCircle{ nullptr };
	pCircle = pView->pLayer->getPrimitive(pView->pLayer->POLY);
	pCircle->setVertex(circleX, circleY, circleZ, 0.9f, 0.9f, 0.9f);
	pCircle->setMode(mode);
	pCircle->setRadio(mRadio);
	reinterpret_cast<GLpoly*>(pCircle)->CIRCLEsetRadius(circleRadius);
	pCircle->pushVertex();

	pCircle->drawing();
	pView->pLayer->bPoly = TRUE;

	pDoc->UpdateAllViews(NULL);
}


void Circle::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CstructureDoc* pDoc = pView->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	


	DestroyWindow();
}


void Circle::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	pView->pCircledlg = NULL;
	delete this;
}


void Circle::OnBnClickedCircleEraseButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);

	circleX = 0;
	circleY = 0;
	circleZ = 0;

	mRadio = -1;
}
