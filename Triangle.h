#pragma once
#include "afxdialogex.h"

class CstructureView;
class CstructureDoc;

// Triangle dialog

class Triangle : public CDialog
{
	DECLARE_DYNAMIC(Triangle)

public:
	Triangle(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Triangle();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRIANGLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CstructureView* pView{ nullptr };
	float triX;
	float triY;
	float triZ;
	int mode;
	virtual void OnOK();
	virtual void PostNcDestroy();
	virtual void OnCancel();
	afx_msg void OnBnClickedTriangleEraseButton();
};
