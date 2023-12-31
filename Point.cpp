// Point.cpp : implementation file
//

#include "pch.h"
#include "structure.h"
#include "afxdialogex.h"
#include "Point.h"
#include "structureDoc.h"
#include "structureView.h"

// Point dialog

IMPLEMENT_DYNAMIC(Point, CDialog)

Point::Point(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_POINT_DIALOG, pParent)
	, pointX(0)
	, pointY(0)
	, pointZ(0)
{
}

Point::~Point()
{
}

void Point::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POINT_X, pointZ);
	DDX_Text(pDX, IDC_POINT_Y, pointX);
	DDX_Text(pDX, IDC_POINT_Z, pointY);
}


BEGIN_MESSAGE_MAP(Point, CDialog)
	ON_BN_CLICKED(IDC_POINT_ERASE_BUTTON, &Point::OnBnClickedPointEraseButton)
END_MESSAGE_MAP()


// Point message handlers

// index, 자기 위치 기억하는거 넣자
void Point::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	
	GLprimitive* pPoint{ nullptr };
	pPoint = pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT);
	pPoint->setVertex(pointX, pointY, pointZ);
	pPoint->pushVertex();


	pPoint->drawing();
	pDoc->pLayer->bPoint = TRUE;

	pDoc->UpdateAllViews(NULL);
}


void Point::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
}


void Point::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	pDoc->pPointdlg = NULL;
	delete this;
}


void Point::OnBnClickedPointEraseButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);

	pointX = 0;
	pointY = 0;
	pointZ = 0;
}
