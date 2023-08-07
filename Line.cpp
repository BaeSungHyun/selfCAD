// Line.cpp : implementation file
//

#include "pch.h"
#include "structure.h"
#include "afxdialogex.h"
#include "Line.h"
#include "structureDoc.h"
#include "structureView.h"


// Line dialog

IMPLEMENT_DYNAMIC(Line, CDialog)

Line::Line(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_LINE_DIALOG, pParent)
	, lineX(0)
	, lineY(0)
	, lineZ(0)
{

}

Line::~Line()
{
}

void Line::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LINE_X, lineX);
	DDX_Text(pDX, IDC_LINE_Y, lineY);
	DDX_Text(pDX, IDC_LINE_Z, lineZ);
}


BEGIN_MESSAGE_MAP(Line, CDialog)
	ON_BN_CLICKED(IDC_LINE_ERASE_BUTTON, &Line::OnBnClickedLineEraseButton)
END_MESSAGE_MAP()


// Line message handlers


void Line::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CstructureDoc* pDoc = pView->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	UpdateData(TRUE);

	GLprimitive* pLine{ nullptr };
	pLine = pView->pLayer->getPrimitive(pView->pLayer->LINE);
	pLine->setVertex(lineX, lineY, lineZ);
	pLine->setMode(mode); 
	pLine->pushVertex();

	pLine->drawing();
	pView->pLayer->bLine = TRUE;

	pDoc->UpdateAllViews(NULL);
}


void Line::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CstructureDoc* pDoc = pView->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GLprimitive* pLine{ nullptr };
	pLine = pView->pLayer->getPrimitive(pView->pLayer->LINE);
	if (mode == 2) {
		GLprimitive* pLine{ nullptr };
		pLine = pView->pLayer->getPrimitive(pView->pLayer->LINE);
		reinterpret_cast<GLline*>(pLine)->loopComplete();
	}
	reinterpret_cast<GLline*>(pView->pLayer->getPrimitive(pView->pLayer->LINE))->setIndiCapacity(0);
	
	pLine->drawing();
	pDoc->UpdateAllViews(NULL);
	
	DestroyWindow();
}


void Line::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	pView->pLinedlg = NULL;
	delete this;
}


void Line::OnBnClickedLineEraseButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);

	lineX = 0;
	lineY = 0;
	lineZ = 0;
}
