#pragma once
#include <string>

// CSkillRegistWindow dialog

class CSkillRegistWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CSkillRegistWindow)

public:
	CSkillRegistWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkillRegistWindow();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SKILL_REG_WINDOW};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CEdit cSkillIdEdit;
	CEdit cSkillNameEdit;
	CStatic cError;
	int nSkillID;
	boolean bUpdFlg;
	std::string strSkillName;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnClr();
	afx_msg void OnBnClickedBtnReg();
};
