#pragma once
#include <string>

// CCmpnRegistWindow dialog

class CCmpnRegistWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CCmpnRegistWindow)

public:
	CCmpnRegistWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmpnRegistWindow();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMPN_REG_WINDOW};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CEdit cCmpnIdEdit;
	CEdit cCmpnNameEdit;
	CEdit cCmpnFuriEdit;
	CStatic cError;
	int nCmpnID;
	boolean bUpdFlg;
	std::string strCmpnName;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnClr();
	afx_msg void OnBnClickedBtnReg();
};
