#pragma once
#include "afxwin.h"


// CLOGIN dialog

class CLOGIN : public CDialogEx
{
	DECLARE_DYNAMIC(CLOGIN)

public:
	CLOGIN(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLOGIN();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnLogin();
	CEdit cUserEdit;
	CEdit cPasswordEdit;
	afx_msg void OnClear();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CLOGIN::ParentApp(CWinApp * pParentApp);

public:
	CWinApp *m_pParentApp;
	virtual BOOL OnInitDialog();
	CButton m_LonginBtn;
	CButton m_ClearBtn;
};
