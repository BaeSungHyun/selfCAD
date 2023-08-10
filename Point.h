#pragma once
#include "afxdialogex.h"


class CstructureView;
class CstructureDoc;

// Point dialog

class Point : public CDialog
{
	DECLARE_DYNAMIC(Point)

public:
	Point(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Point();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POINT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CstructureDoc* pDoc{ nullptr };
	float pointX;
	float pointY;
	float pointZ;
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedPointEraseButton();
};
