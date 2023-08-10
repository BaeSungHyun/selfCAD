#pragma once
#include "afxdialogex.h"

class CstructureView;
class CstructureDoc;

// Line dialog

class Line : public CDialog
{
	DECLARE_DYNAMIC(Line)

public:
	Line(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Line();

	int mode{ 0 };

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CstructureDoc* pDoc{ nullptr };
	float lineX;
	float lineY;
	float lineZ;

	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedLineEraseButton();
};
