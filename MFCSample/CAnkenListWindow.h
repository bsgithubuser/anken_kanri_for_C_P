#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CUserData.h"
#include "CProjectData.h"
#include "CProjectSearchData.h"
#include "afxdtctl.h"

#include <vector>

// CAnkenList dialog

#define SORT_UP      1
#define SORT_DOWN    0

class CAnkenListWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CAnkenListWindow)

public:
	CAnkenListWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnkenListWindow();

public:
	std::vector<CUserData> m_vUserData;
	std::vector<CProjectData> m_vProjectData;
	CProjectSearchData m_clProjectSearch;
	bool m_WaitCursel = false;
	int m_iSubItem = SORT_DOWN;
	int m_iItemSort;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANKENLIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_AnkenLabel;
	CListCtrl m_AnkenListCtrl;
	void InitAnkenListCtrl();
	CComboBox m_UserCombo;
	void SetUserCombo();
	afx_msg void OnSerachProject();
	void SetProjectList(std::vector<CProjectData>& vProjectData);
	int SetSearchData(CProjectSearchData& SearchData);
	CDateTimeCtrl m_ToDateEdit;
	CDateTimeCtrl m_FromDateEdit;
	CEdit m_CmpEdit;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	CEdit m_AnkenNameSearch;
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	static int CALLBACK CompProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	virtual void OnOK();
	virtual void OnCancel();
};
