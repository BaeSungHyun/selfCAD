#pragma once
#include "afxdialogex.h"

class CstructureView;
class CstructureDoc;

// Extrude dialog

class Extrude : public CDialog
{
	DECLARE_DYNAMIC(Extrude)

public:
	Extrude(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Extrude();

	CstructureDoc* pDoc{ nullptr };
	CstructureView* pView{ nullptr };

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXTRUDE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float extrudeX;
	float extrudeY;
	float extrudeZ;
	float translate;
	virtual void OnCancel();
	virtual void OnOK();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedExtrudeEraseButton();
};
