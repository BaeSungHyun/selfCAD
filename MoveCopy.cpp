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
			for (unsigned int i = 0; i < pView->saveCapacity[0]; ++i) {
				pointPointer->setVertex(pointPointer->getX(pView->saveIndex[0][i]) + directionX,
					pointPointer->getY(pView->saveIndex[0][i]) + directionY,
					pointPointer->getZ(pView->saveIndex[0][i]) + directionZ);
				pointPointer->pushVertex();
			}
			pointPointer->drawing();
		}
		if (pView->saveCapacity[1] != 0) {
			unsigned int start1{ 0 }, start2{ 0 };
			while (!done) {
				for (unsigned int i = start1; i < pView->saveCapacity[1]; ++i) {
					unsigned int* temp = new unsigned int[linePointer->getIndexCapacity() + 1];
					for (int j = 0; j < linePointer->getIndexCapacity(); ++j) {
						temp[j] = linePointer->getpIndices()[j];
					}
					temp[linePointer->getIndexCapacity()] // inserting position
						= linePointer->getCapacity() - linePointer->getpIndices()[pView->saveIndex[1][start1]]
						+ linePointer->getpIndices()[pView->saveIndex[1][i]]; // selected position

					delete[] linePointer->getpIndices();
					linePointer->getpIndices() = temp;

					linePointer->addIndexCapacity(1);

					if (i < pView->saveCapacity[1] - 1 && pView->saveIndex[1][i + 1] - pView->saveIndex[1][i] != 1) {
						start1 = i + 1;
						break;
					}
					else if (i == pView->saveCapacity[1] - 1) {
						start1 = pView->saveCapacity[1];
					}
				}
				for (unsigned int i = start2; i < pView->saveCapacity[4]; ++i) {
					linePointer->setVertex(linePointer->getX(pView->saveIndex[4][i]) + directionX,
						linePointer->getY(pView->saveIndex[4][i]) + directionY, linePointer->getZ(pView->saveIndex[4][i]) + directionZ, 0.0f, 0.0f, 0.0f);
					linePointer->setMode(3);
					linePointer->pushVertex();

					if (i < pView->saveCapacity[4] - 1 && pView->saveIndex[4][i + 1] - pView->saveIndex[4][i] != 1) {
						start2 = i + 1;
						break;
					}
					else if (i == pView->saveCapacity[4] - 1) {
						start2 = pView->saveCapacity[4];
					}
				}

				if (start1 == pView->saveCapacity[1] && start2 == pView->saveCapacity[4]) {
					done = TRUE;
				}
			}
			linePointer->drawing();
			done = FALSE;
		}
		if (pView->saveCapacity[2] != 0) {
			unsigned int start1{ 0 }, start2{ 0 }, start3{ 0 };
			while (!done) {
				for (unsigned int i = start1; i < pView->saveCapacity[2]; ++i) {
					unsigned int* temp = new unsigned int[polyPointer->getIndexCapacity() + 1];
					for (unsigned int j = 0; j < polyPointer->getIndexCapacity(); ++j) {
						temp[j] = polyPointer->getpIndices()[j];
					}
					temp[polyPointer->getIndexCapacity()]
						= polyPointer->getCapacity() - polyPointer->getpIndices()[pView->saveIndex[2][start1]]
						+ polyPointer->getpIndices()[pView->saveIndex[2][i]];

					delete[] polyPointer->getpIndices();
					polyPointer->getpIndices() = temp;

					polyPointer->addPolyIndexCapacity(1);

					if (i < pView->saveCapacity[2] - 1 && pView->saveIndex[2][i + 1] - pView->saveIndex[2][i] != 1) {
						start1 = i + 1;
						break;
					}
					else if (i == pView->saveCapacity[2] - 1) {
						start1 = pView->saveCapacity[2];
					}
				}
				for (unsigned int i = start2; i < pView->saveCapacity[3]; ++i) {
					unsigned int* temp = new unsigned int[polyPointer->getLineIndexCapacity() + 1];
					for (unsigned int j = 0; j < polyPointer->getLineIndexCapacity(); ++j) {
						temp[j] = polyPointer->getpLineIndices()[j];
					}
					temp[polyPointer->getLineIndexCapacity()]
						= polyPointer->getCapacity() - polyPointer->getpLineIndices()[pView->saveIndex[3][start2]]
						+ polyPointer->getpLineIndices()[pView->saveIndex[3][i]];

					delete[] polyPointer->getpLineIndices();
					polyPointer->getpLineIndices() = temp;

					polyPointer->addLineIndexCapacity(1);

					if (i < pView->saveCapacity[3] - 1 && pView->saveIndex[3][i + 1] - pView->saveIndex[3][i] != 1) {
						start2 = i + 1;
						break;
					}
					else if (i == pView->saveCapacity[3] - 1) {
						start2 = pView->saveCapacity[3];
					}
				}
				for (unsigned int i = start3; i < pView->saveCapacity[5]; ++i) {
					polyPointer->setVertex(polyPointer->getX(pView->saveIndex[5][i]) + directionX,
						polyPointer->getY(pView->saveIndex[5][i]) + directionY, polyPointer->getZ(pView->saveIndex[5][i]) + directionZ, 0.9f, 0.9f, 0.9f);
					polyPointer->setMode(3);
					polyPointer->pushVertex();

					if (i < pView->saveCapacity[5] - 1 && pView->saveIndex[5][i + 1] - pView->saveIndex[5][i] != 1) {
						start3 = i + 1;
						break;
					}
					else if (i == pView->saveCapacity[5] - 1) {
						start3 = pView->saveCapacity[5];
					}
				}

				if (start1 == pView->saveCapacity[2] && start2 == pView->saveCapacity[3] && start3 == pView->saveCapacity[5]) {
					done = TRUE;
				}
			}
			polyPointer->drawing();
			done = FALSE;
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

	done = FALSE;

	pDoc->UpdateAllViews(NULL);
}


void MoveCopy::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	pDoc->pMoveCopydlg = NULL;
	delete this;
}
