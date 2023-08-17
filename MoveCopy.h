#pragma once
#include "afxdialogex.h"

class CstructureView;
class CstructureDoc;

// MoveCopy dialog

class MoveCopy : public CDialog
{
	DECLARE_DYNAMIC(MoveCopy)

public:
	MoveCopy(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MoveCopy();

	CstructureDoc* pDoc{ nullptr };
	CstructureView* pView{ nullptr };

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOVE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int mRadio;
	float originalX;
	float originalY;
	float originalZ;

	float finalX;
	float finalY;
	float finalZ;

	afx_msg void OnBnClickedMoveErase();
	virtual void OnCancel();
	virtual void OnOK();
	virtual void PostNcDestroy();
};
