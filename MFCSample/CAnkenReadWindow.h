#pragma once


// CAnkenReadWindow dialog

class CAnkenReadWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CAnkenReadWindow)

public:
	CAnkenReadWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnkenReadWindow();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANKENREADWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
