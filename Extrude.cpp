// Extrude.cpp : implementation file
//

#include "pch.h"
#include "structure.h"
#include "afxdialogex.h"
#include "Extrude.h"
#include "structureView.h"
#include "structureDoc.h"


// Extrude dialog

IMPLEMENT_DYNAMIC(Extrude, CDialog)

Extrude::Extrude(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_EXTRUDE_DIALOG, pParent)
	, extrudeX(0)
	, extrudeY(0)
	, extrudeZ(0)
	, translate(0)
{
}

Extrude::~Extrude()
{
}

void Extrude::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EXTRUDE_X, extrudeZ);
	DDX_Text(pDX, IDC_EXTRUDE_Y, extrudeX);
	DDX_Text(pDX, IDC_EXTRUDE_Z, extrudeY);
	DDX_Text(pDX, IDC_EXTRUDE_SPACING, translate);
}


BEGIN_MESSAGE_MAP(Extrude, CDialog)
	ON_BN_CLICKED(IDC_EXTRUDE_ERASE_BUTTON, &Extrude::OnBnClickedExtrudeEraseButton)
END_MESSAGE_MAP()


// Extrude message handlers


void Extrude::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
}


void Extrude::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	float norm = sqrt(extrudeX * extrudeX + extrudeY * extrudeY + extrudeZ * extrudeZ);

	if (norm == 0) {
		MessageBox(_T("Directional Vector should be bigger than 0."));
		return;
	}

	if (pView->saveCapacity[0] != 0) {
		MessageBox(_T("Select only line or polygon"));
		return;
	}

	if (pView->saveCapacity[1] == 0 && pView->saveCapacity[2] == 0) {
		MessageBox(_T("Select a line or a polygon"));
		return;
	}

	if (pView->saveCapacity[1] != 0 && pView->saveCapacity[2] != 0) {
		MessageBox(_T("Select a line or a polygon"));
	}

	extrudeX /= norm;
	extrudeY /= norm;
	extrudeZ /= norm;

	// Use the SELECTED VERTICES, DIRECTIONAL VECTOR, and SPACING
	GLpoly* polyPointer{ nullptr };
	polyPointer = reinterpret_cast<GLpoly*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->POLY));
	
	GLline* linePointer{ nullptr };
	linePointer = reinterpret_cast<GLline*>(pDoc->pLayer->getPrimitive(pDoc->pLayer->LINE));

	// check if there is selected POLY or LINE
	// 1. LINE
	if (pView->saveCapacity[1] != 0) {
		// pView->saveIndex[1][0] == min , pView->saveIndex[1][saveCapacity[1] - 1] == max
		// min , max + 1
		for (int i = pView->saveIndex[1][0]; i < pView->saveIndex[1][pView->saveCapacity[1] - 1] + 1; i += 2) { // step : 4. 4 vertices for rectangle
			// nice cycle loop for EXTRUDE POLY
			polyPointer->ExtrudeLinePushVertex();
			polyPointer->ExtrudePushVertex();

			polyPointer->setMode(3); // Just same as FILL, only for GLprimitive::pushVertex();
			float firstX{ linePointer->getX(linePointer->getpIndices()[i]) }, firstY{ linePointer->getY(linePointer->getpIndices()[i]) }, firstZ{ linePointer->getZ(linePointer->getpIndices()[i]) };
			float secondX{ linePointer->getX(linePointer->getpIndices()[i + 1]) }, secondY{ linePointer->getY(linePointer->getpIndices()[i + 1]) }, secondZ{ linePointer->getZ(linePointer->getpIndices()[i + 1]) };
			polyPointer->setVertex(firstX, firstY, firstZ, 0.9f, 0.9f ,0.9f);
			polyPointer->pushVertex();
			polyPointer->setVertex(secondX, secondY, secondZ, 0.9f, 0.9f, 0.9f);
			polyPointer->pushVertex();
			polyPointer->setVertex(secondX + extrudeX * translate, secondY+ extrudeY * translate, secondZ + extrudeZ * translate, 0.9f, 0.9f, 0.9f);
			polyPointer->pushVertex();
			polyPointer->setVertex(firstX + extrudeX * translate, firstY + extrudeY * translate, firstZ + extrudeZ * translate, 0.9f, 0.9f, 0.9f);
			polyPointer->pushVertex();
		}
	}
	// 2. POLY
	if (pView->saveCapacity[2] != 0) {
		// pView->saveIndex[3][0] == min , pView->saveIndex[3][saveCapacity[2] - 1] == max
		// min, max + 1 -> for loop
		for (int i = pView->saveIndex[3][0]; i < pView->saveIndex[3][pView->saveCapacity[3] - 1] + 1; i += 2) {
			// nice cycle loop for EXTRUDE POLY
			polyPointer->ExtrudeLinePushVertex();
			polyPointer->ExtrudePushVertex();

			polyPointer->setMode(3); // Just same as FILL, only for GLprimitive::pushVertex();
			float firstX{ polyPointer->getX(polyPointer->getpLineIndices()[i]) }, firstY{ polyPointer->getY(polyPointer->getpLineIndices()[i]) }, firstZ{ polyPointer->getZ(polyPointer->getpLineIndices()[i]) };
			float secondX{ polyPointer->getX(polyPointer->getpLineIndices()[i + 1]) }, secondY{ polyPointer->getY(polyPointer->getpLineIndices()[i + 1]) }, secondZ{ polyPointer->getZ(polyPointer->getpLineIndices()[i + 1]) };
			polyPointer->setVertex(firstX, firstY, firstZ, 0.9f, 0.9f, 0.9f);
			polyPointer->pushVertex();
			polyPointer->setVertex(secondX, secondY, secondZ, 0.9f, 0.9f, 0.9f);
			polyPointer->pushVertex();
			polyPointer->setVertex(secondX + extrudeX * translate, secondY + extrudeY * translate, secondZ + extrudeZ * translate, 0.9f, 0.9f, 0.9f);
			polyPointer->pushVertex();
			polyPointer->setVertex(firstX + extrudeX * translate, firstY + extrudeY * translate, firstZ + extrudeZ * translate, 0.9f, 0.9f, 0.9f);
			polyPointer->pushVertex();
		}

		// CAP for EXTRUDE POLY

	}

	pDoc->pLayer->bPoly = TRUE;
	polyPointer->drawing();
	pDoc->UpdateAllViews(NULL);
}


void Extrude::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	pDoc->pExtrudedlg = NULL;
	delete this;
}


void Extrude::OnBnClickedExtrudeEraseButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);

	extrudeX = 0;
	extrudeY = 0;
	extrudeZ = 0;
}
