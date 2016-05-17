#pragma once
#include "afxwin.h"
#include "CUserData.h"
#include "CProjectData.h"
#include "CCmpData.h"
#include "CSkillData.h"

#include <vector>

// CAnkenRegistWindow dialog

class CAnkenRegistWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CAnkenRegistWindow)

public:
	CAnkenRegistWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnkenRegistWindow();
	std::vector<CUserData> m_vUserData;
	std::vector<CCmpData> m_vCmpData;
	std::vector<CSkillData> m_vSkillData;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANKENREGISTWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CStatic m_AnkenRLabel;
	virtual BOOL OnInitDialog();
	CEdit m_AnkenNameEdit;
	CComboBox m_UserCombo;
	CDateTimeCtrl m_GenDayEdit;
	CEdit m_CmpNameEdit;
	CDateTimeCtrl m_FromDayEdit;
	CDateTimeCtrl m_ToDayEdit;
	int m_ExtentionRadio;
	BOOL m_LongTermChk;
	BOOL m_SameDayChk;
	BOOL m_AnyTimeChk;
	CEdit m_SkillNameEdit;
	CEdit m_OrverEdit;
	CEdit m_OtherEdit;
	afx_msg void OnBnClickedButton3();
	void InitUserCombo();
	int SetRegistData(CProjectData& cProjectData);
	void InitCmpCombo();
	CComboBox m_CmpCombo;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	virtual void OnOK();
	virtual void OnCancel();
};
