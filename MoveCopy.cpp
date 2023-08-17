// MoveCopy.cpp : implementation file
//

#include "pch.h"
#include "structure.h"
#include "afxdialogex.h"
#include "MoveCopy.h"
#include "structureView.h"
#include "structureDoc.h"


// MoveCopy dialog

IMPLEMENT_DYNAMIC(MoveCopy, CDialog)

MoveCopy::MoveCopy(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MOVE_DIALOG, pParent)
	, mRadio(-1)
	, originalX(0)
	, originalY(0)
	, originalZ(0)
	, finalX(0)
	, finalY(0)
	, finalZ(0)
{

}

MoveCopy::~MoveCopy()
{
}

void MoveCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_COPY, mRadio);
	DDX_Text(pDX, IDC_MOVE_ORIGINALX, originalZ);
	DDX_Text(pDX, IDC_MOVE_ORIGINALY, originalX);
	DDX_Text(pDX, IDC_MOVE_ORIGINALZ, originalY);
	DDX_Text(pDX, IDC_MOVE_FINALX, finalZ);
	DDX_Text(pDX, IDC_MOVE_FINALY, finalX);
	DDX_Text(pDX, IDC_MOVE_FINALZ, finalY);
}


BEGIN_MESSAGE_MAP(MoveCopy, CDialog)
	ON_BN_CLICKED(IDC_MOVE_ERASE, &MoveCopy::OnBnClickedMoveErase)
END_MESSAGE_MAP()


// MoveCopy message handlers


void MoveCopy::OnBnClickedMoveErase()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);

	originalX = 0;
	originalY = 0;
	originalZ = 0;

	finalX = 0;
	finalY = 0;
	finalZ = 0;

	mRadio = -1;
}


void MoveCopy::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
}


void MoveCopy::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	float directionX{ finalX - originalX };
	float directionY{ finalY - originalY };
	float directionZ{ finalZ - originalZ };

	if (directionX * directionX + directionY * directionY + directionZ * directionZ == 0) {
		MessageBox(_T("Original Position and Final Position can't be same"));
		return;
	}

	GLpoly* polyPointer{ reinterpret_cast<GLpoly*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY)) };
	GLline* linePointer{ reinterpret_cast<GLline*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->LINE)) };
	GLprimitive* pointPointer{ reinterpret_cast<GLprimitive*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POINT)) };

	switch (mRadio) {
	case 0: { // copy
		if (pView->saveCapacity[0] != 0) {
			pointPointer->drawing();
		}
		if (pView->saveCapacity[1] != 0) {
			linePointer->drawing();
		}
		if (pView->saveCapacity[2] != 0) {
			polyPointer->drawing();
		}
		break;
	}
	case 1: { // move
		if (pView->saveCapacity[0] != 0) {
			for (unsigned int i = 0; i < pView->saveCapacity[0]; ++i) {
				pointPointer->addVX(pView->saveIndex[0][i], directionX);
				pointPointer->addVY(pView->saveIndex[0][i], directionY);
				pointPointer->addVZ(pView->saveIndex[0][i], directionZ);
			}
			pointPointer->drawing();
		}
		if (pView->saveCapacity[1] != 0) {
			for (unsigned int i = 0; i < pView->saveCapacity[4]; ++i) {
				linePointer->addVX(pView->saveIndex[4][i], directionX);
				linePointer->addVY(pView->saveIndex[4][i], directionY);
				linePointer->addVZ(pView->saveIndex[4][i], directionZ);
			}
			linePointer->drawing();
		}
		if (pView->saveCapacity[2] != 0) {
			for (unsigned int i = 0; i < pView->saveCapacity[5]; ++i) {
				polyPointer->addVX(pView->saveIndex[5][i], directionX);
				polyPointer->addVY(pView->saveIndex[5][i], directionY);
				polyPointer->addVZ(pView->saveIndex[5][i], directionZ);
			}
			polyPointer->drawing();
		}
		break;
	}
	}

	pDoc->UpdateAllViews(NULL);
}


void MoveCopy::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	pDoc->pMoveCopydlg = NULL;
	delete this;
}
