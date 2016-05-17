#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CCompanyData.h"
#include "CCmpnRegistWindow.h"

#include <vector>

// CCompanyWindow dialog

class CCompanyWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CCompanyWindow)

private:
	CCmpnRegistWindow m_cCmpnRegistWindow;
public:
	CCompanyWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCompanyWindow();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMPNWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_MastaTitle;
	virtual BOOL OnInitDialog();
	CListCtrl m_ListCtl;
	void InitCompanyList();
	void CompanyListUpdate();
	std::vector<CCompanyData> m_vCompanyData;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDatareg();
	afx_msg void OnBnClickedBtnDataedit();
	afx_msg void OnBnClickedBtnDatadel();
	afx_msg void OnBnClickedBtnRedraw();
	virtual void OnOK();
	virtual void OnCancel();
};
