#pragma once
#include "CProjectData.h"
#include "CUserData.h"
#include "CCmpData.h"
#include "afxwin.h"
#include "afxdtctl.h"

// CAnkenDetailWindow dialog

class CAnkenDetailWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CAnkenDetailWindow)

public:
	CAnkenDetailWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnkenDetailWindow();
	CProjectData m_ProjectData;
	std::vector<CUserData> m_vUserData;
	std::vector<CSkillData> m_vSkillData;
	std::vector<CCmpData> m_vCmpData;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANKENDETAILWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetProjectData(CProjectData& cProjectData, std::vector<CUserData>& vUserData);
	CEdit m_AnkenNameEdit;
	CComboBox m_UserCombo;
	CDateTimeCtrl m_GenDayEdit;
	CDateTimeCtrl m_FromDayEdit;
	CDateTimeCtrl m_ToDayEdit;
	int m_ExtentionRadio;
	BOOL m_LongTermChk;
	BOOL m_SameDayChk;
	BOOL m_AnyTimeChk;
	CEdit m_SkillNameEdit;
	CEdit m_OrverEdit;
	CEdit m_OtherEdit;
	void SetUserCombo();
	CTime GetTimeDay(std::string& strGetTime);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	int SetUpdateData(CProjectData& cProjectData);
	CComboBox m_CmpCombo;
	void SetCmpCombo();
	afx_msg void OnBnClickedButton3();
	virtual void OnOK();
	virtual void OnCancel();
};
