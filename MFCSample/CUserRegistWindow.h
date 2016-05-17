#pragma once
#pragma once
#include <string>
#include "CUserWindow.h"

// CUserRegistWindow dialog

class CUserRegistWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CUserRegistWindow)

public:
	CUserRegistWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserRegistWindow();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_REG_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CEdit cUserIdEdit;
	CEdit cUserNameEdit;
	CEdit cPasswordEdit;
	CEdit cPasswordEdit2;
	CComboBox cAdminEdit;
	CStatic cError;
	int nSkillID;
	int nUpdFlg;
	std::string strUserName;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnClr();
	afx_msg void OnBnClickedBtnReg();
	afx_msg void OnEnMaxtextPassEdit();
	CString aaaa;
};
