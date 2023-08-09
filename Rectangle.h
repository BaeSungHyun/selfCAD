#pragma once
#include "afxdialogex.h"

class CstructureView;
class CstructureDoc;

// Rectangle dialog

class Rectangles : public CDialog
{
	DECLARE_DYNAMIC(Rectangles)

public:
	Rectangles(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Rectangles();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int mode{0};
	CstructureView* pView{ nullptr };
	int mRadio;
	float rectX;
	float rectY;
	float rectZ;
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedRectEraseButton();
};
