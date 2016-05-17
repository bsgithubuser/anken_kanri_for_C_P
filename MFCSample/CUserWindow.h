#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CUserData.h"
#include "CUserRegistWindow.h"

#include <vector>

// CUserWindow dialog

class CUserWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CUserWindow)
private:
	CUserRegistWindow m_cUserRegistWindow;
public:
	CUserWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserWindow();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_MastaTitle;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtl;
	void InitUserList();
	void UserListUpdate();
	std::vector<CUserData> m_vUserData;
	afx_msg void OnBnClickedBtnDatareg();
	afx_msg void OnBnClickedBtnDataedit();
	afx_msg void OnBnClickedBtnDatadel();
	afx_msg void OnBnClickedBtnRedraw();
	virtual void OnOK();
	virtual void OnCancel();
};
