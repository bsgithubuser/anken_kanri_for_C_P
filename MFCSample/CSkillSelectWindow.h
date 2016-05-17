#pragma once
#include "CSkillData.h"
#include <vector>
#include "afxcmn.h"

// CSkillSelectWindow dialog

class CSkillSelectWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CSkillSelectWindow)

public:
	CSkillSelectWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkillSelectWindow();
	std::vector<CSkillData>* m_pvSkillData;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SKILLSELECTWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void SetSkillData(std::vector<CSkillData>* pvSkillData);

	virtual BOOL OnInitDialog();
	CListCtrl m_SkillSelectList;
	void InitSkillSelectList();
	void SetSkillList();
	afx_msg void OnBnClickedButton1();
};
