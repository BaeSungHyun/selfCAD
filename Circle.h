#pragma once
#include "afxdialogex.h"

class CstructureView;
class CstructureDoc;

// Circle dialog

class Circle : public CDialog
{
	DECLARE_DYNAMIC(Circle)

public:
	Circle(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Circle();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CstructureDoc* pDoc{ nullptr };
	int mode;
	int mRadio;
	float circleX;
	float circleY;
	float circleZ;
	float circleRadius;
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedCircleEraseButton();
};
