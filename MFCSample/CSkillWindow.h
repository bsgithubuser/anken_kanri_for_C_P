#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CSkillData.h"
#include "CSkillRegistWindow.h"

#include <vector>

// CSkillWindow dialog

class CSkillWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CSkillWindow)

private:
	CSkillRegistWindow m_cSkillRegistWindow;
public:
	CSkillWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkillWindow();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SKILLWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_MastaTitle;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtl;
	void InitSkillList();
	void SkillListUpdate();
	std::vector<CSkillData> m_vSkillData;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDatareg();
	afx_msg void OnBnClickedBtnDataedit();
	afx_msg void OnBnClickedBtnDatadel();
	afx_msg void OnBnClickedBtnRedraw();
	virtual void OnOK();
	virtual void OnCancel();
};
