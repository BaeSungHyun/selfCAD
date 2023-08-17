
// structureDoc.cpp : implementation of the CstructureDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "structure.h"
#endif

#include "structureDoc.h"
#include "structureView.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CstructureDoc

IMPLEMENT_DYNCREATE(CstructureDoc, CDocument)

BEGIN_MESSAGE_MAP(CstructureDoc, CDocument)
	ON_COMMAND(ID_POINT_TOOLBAR, &CstructureDoc::OnPointToolbar)
	ON_COMMAND(ID_LINES_TOOLBAR, &CstructureDoc::OnLinesToolbar)
	ON_COMMAND(ID_LINE_STRIP_TOOLBAR, &CstructureDoc::OnLineStripToolbar)
	ON_COMMAND(ID_LINE_LOOP_TOOLBAR, &CstructureDoc::OnLineLoopToolbar)
	ON_COMMAND(ID_POLY_TRIANGLES, &CstructureDoc::OnPolyTriToolbar)
	ON_COMMAND(ID_POLY_RECTANGLES, &CstructureDoc::OnPolyRecToolbar)
	ON_COMMAND(ID_POLY_CIRCLE, &CstructureDoc::OnPolyCircleToolbar)
	ON_COMMAND(ID_EXTRUDE_TOOLBAR, &CstructureDoc::OnExtrudeToolbar)
END_MESSAGE_MAP()


// CstructureDoc construction/destruction

CstructureDoc::CstructureDoc() noexcept
{
	// TODO: add one-time construction code here
	pPointdlg = NULL;
	pLinedlg = NULL;
	pTridlg = NULL;
	pRectdlg = NULL;
	pCircledlg = NULL;
	pExtrudedlg = NULL; 
	layerCapacity = 0;
	pLayer = new Layer[ layerCapacity ];
}

CstructureDoc::~CstructureDoc()
{
	delete pPointdlg;
	delete[] pLayer;
}

BOOL CstructureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CstructureDoc serialization

void CstructureDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << layerCapacity;
		pLayer->Serialize(ar);
		pLayer->getPrimitive(pLayer->POINT)->Serialize(ar);
		pLayer->getPrimitive(pLayer->LINE)->Serialize(ar);
		pLayer->getPrimitive(pLayer->POLY)->Serialize(ar);
	}
	else
	{
		// TODO: add loading code here
		ar >> layerCapacity;
		pLayer->Serialize(ar);
		pLayer->getPrimitive(pLayer->POINT)->Serialize(ar);
		pLayer->getPrimitive(pLayer->LINE)->Serialize(ar);
		pLayer->getPrimitive(pLayer->POLY)->Serialize(ar);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CstructureDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CstructureDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CstructureDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CstructureDoc diagnostics

#ifdef _DEBUG
void CstructureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CstructureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CstructureDoc commands
void CstructureDoc::OnPointToolbar() {
	if (pPointdlg != NULL) {
		pPointdlg->SetFocus();
	}
	else {
		// remember AfxGetMainWnd() function only returns pointer when called from the main thread
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CstructureView* pView = (CstructureView*)pMainFrame->GetActiveView();
		pPointdlg = new Point(pView);
		pPointdlg->pDoc = this;
		pPointdlg->pointX = 0;
		pPointdlg->pointY = 0;
		pPointdlg->pointZ = 0;

		pPointdlg->Create(IDD_POINT_DIALOG);
		pPointdlg->ShowWindow(SW_SHOW);
	}
}

void CstructureDoc::OnLinesToolbar() {
	if (pLinedlg != NULL) {
		pLinedlg->SetFocus();
	}
	else {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CstructureView* pView = (CstructureView*)pMainFrame->GetActiveView();
		pLinedlg = new Line(pView);
		pLinedlg->pDoc = this;
		pLinedlg->lineX = 0;
		pLinedlg->lineY = 0;
		pLinedlg->lineZ = 0;

		pLinedlg->Create(IDD_LINE_DIALOG);
		pLinedlg->mode = 0;
		pLinedlg->ShowWindow(SW_SHOW);
	}
}

void CstructureDoc::OnLineStripToolbar() {
	if (pLinedlg != NULL) {
		pLinedlg->SetFocus();
	}
	else {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CstructureView* pView = (CstructureView*)pMainFrame->GetActiveView();
		pLinedlg = new Line(pView);
		pLinedlg->pDoc = this;
		pLinedlg->lineX = 0;
		pLinedlg->lineY = 0;
		pLinedlg->lineZ = 0;

		pLinedlg->Create(IDD_LINE_DIALOG);
		pLinedlg->mode = 1;
		pLinedlg->ShowWindow(SW_SHOW);
	}
}

void CstructureDoc::OnLineLoopToolbar() {
	if (pLinedlg != NULL) {
		pLinedlg->SetFocus();
	}
	else {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CstructureView* pView = (CstructureView*)pMainFrame->GetActiveView();
		pLinedlg = new Line(pView);
		pLinedlg->pDoc = this;
		pLinedlg->lineX = 0;
		pLinedlg->lineY = 0;
		pLinedlg->lineZ = 0;

		pLinedlg->Create(IDD_LINE_DIALOG);
		pLinedlg->mode = 2;
		pLinedlg->ShowWindow(SW_SHOW);
	}
}

void CstructureDoc::OnPolyTriToolbar() {
	if (pTridlg != NULL) {
		pTridlg->SetFocus();
	}
	else {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CstructureView* pView = (CstructureView*)pMainFrame->GetActiveView();
		pTridlg = new Triangle(pView);
		pTridlg->pDoc = this;
		pTridlg->triX = 0;
		pTridlg->triY = 0;
		pTridlg->triZ = 0;

		pTridlg->Create(IDD_TRIANGLE_DIALOG);
		pTridlg->mode = 0;
		pTridlg->ShowWindow(SW_SHOW);
	}
}

// BEFORE MAKING DIALOGUES FOR BELOW TWE FUNCTIONS
void CstructureDoc::OnPolyRecToolbar() {
	if (pRectdlg != NULL) {
		pRectdlg->SetFocus();
	}
	else {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CstructureView* pView = (CstructureView*)pMainFrame->GetActiveView();
		pRectdlg = new Rectangles(pView);
		pRectdlg->pDoc = this;
		pRectdlg->rectX = 0;
		pRectdlg->rectY = 0;
		pRectdlg->rectZ = 0;

		pRectdlg->Create(IDD_RECT_DIALOG);
		pRectdlg->mode = 1;
		pRectdlg->ShowWindow(SW_SHOW);
	}
}

void CstructureDoc::OnPolyCircleToolbar() {
	if (pCircledlg != NULL) {
		pCircledlg->SetFocus();
	}
	else {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CstructureView* pView = (CstructureView*)pMainFrame->GetActiveView();
		pCircledlg = new Circle(pView);
		pCircledlg->pDoc = this;
		pCircledlg->circleX = 0;
		pCircledlg->circleY = 0;
		pCircledlg->circleZ = 0;

		pCircledlg->Create(IDD_CIRCLE_DIALOG);
		pCircledlg->mode = 2;
		pCircledlg->ShowWindow(SW_SHOW);
	}
}

// MAKE SURE THAT ONLY ONE POLY LINE OR POLY SURFACE IS CHOSEN
void CstructureDoc::OnExtrudeToolbar() {
	if (pExtrudedlg != NULL) {
		pExtrudedlg->SetFocus();
	}
	else {
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CstructureView* pView = (CstructureView*)pMainFrame->GetActiveView();
		pExtrudedlg = new Extrude(pView);
		pExtrudedlg->pView = pView;
		pExtrudedlg->pDoc = this;
		pExtrudedlg->extrudeX = 0;
		pExtrudedlg->extrudeY = 0;
		pExtrudedlg->extrudeZ = 0;

		pExtrudedlg->Create(IDD_EXTRUDE_DIALOG);
		pExtrudedlg->ShowWindow(SW_SHOW);
	}
}