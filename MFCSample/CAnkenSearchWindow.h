#pragma once
#include "CProjectSearchData.h"
#include "CSkillData.h"
#include <vector>


// CAnkenSearchWindow dialog

class CAnkenSearchWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CAnkenSearchWindow)

public:
	CAnkenSearchWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnkenSearchWindow();
	CProjectSearchData* m_pProjectSearchData;
	std::vector<CSkillData> m_vSkillData;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANKENSEARCHWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetSearchData(CProjectSearchData* cProjectSearch);
	virtual BOOL OnInitDialog();
	CDateTimeCtrl m_FromDayEdit;
	CDateTimeCtrl m_ToDayEdit;
	int m_ExtentionRadio;
	BOOL m_LongTermChk;
	BOOL m_SameDayChk;
	BOOL m_AnyTimeChk;
	CEdit m_SkillNameEdit;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	BOOL m_ValidDay;
	afx_msg void OnBnClickedCheck4();
};
