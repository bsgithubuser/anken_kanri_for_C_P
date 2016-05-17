#pragma once
#include "afxcmn.h"

#include "CUserWindow.h"
#include "CSkillWindow.h"
#include "CCmpWindow.h"
#include "CAnkenListWindow.h"
#include "CAnkenReadWindow.h"
#include "CAnkenRegistWindow.h"
#include "afxwin.h"

// CMainWindow dialog

class CMainWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CMainWindow)

public:
	CMainWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainWindow();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINWINDOW };
#endif

private:
	CUserWindow m_cUserWindow;
	CSkillWindow m_cSkillWindow;
	CCompanyWindow m_cCmpWindow;
	CAnkenListWindow m_cAnkenListWindow;
	CAnkenRegistWindow m_cAnkenRegistWindow;
	CAnkenReadWindow m_cAnkenReadWindow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	CTabCtrl m_TabCtl;

private:
	bool m_bUserWindowCreate = false;
	bool m_bUserWindowShow = false;


public:
	virtual BOOL OnInitDialog();
	void HideChildWindow();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	CButton m_AnkenListBtn;
	CButton m_UserBtn;
	CButton m_SkillBtn;
	CButton m_CmpBtn;
	CButton m_AnkenReadBtn;
	virtual void OnOK();
	virtual void OnCancel();
};
