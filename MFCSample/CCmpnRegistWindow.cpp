// CCmpnRegistWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CCmpnRegistWindow.h"
#include "CDBAccess.h"
#include "afxdialogex.h"


// CCmpnRegistWindow dialog

IMPLEMENT_DYNAMIC(CCmpnRegistWindow, CDialogEx)

CCmpnRegistWindow::CCmpnRegistWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CMPN_REG_WINDOW, pParent)
{

}

CCmpnRegistWindow::~CCmpnRegistWindow()
{
}

void CCmpnRegistWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMPNID_EDIT, cCmpnIdEdit);
	DDX_Control(pDX, IDC_CMPNNAME_EDIT, cCmpnNameEdit);
	DDX_Control(pDX, IDC_CMPNKANA_EDIT, cCmpnFuriEdit);
	DDX_Control(pDX, IDC_ERROR, cError);
}


BEGIN_MESSAGE_MAP(CCmpnRegistWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BACK, &CCmpnRegistWindow::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_CLR, &CCmpnRegistWindow::OnBnClickedBtnClr)
	ON_BN_CLICKED(IDC_BTN_REG, &CCmpnRegistWindow::OnBnClickedBtnReg)
END_MESSAGE_MAP()


BOOL CCmpnRegistWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	cError.SetWindowTextW(_T(""));
//	cError.SetTextColor(RGB(0, 0, 255));
	SetTextColor((HDC)cError.GetWindowDC(), RGB(255, 0, 0));


	return(TRUE);

}


//***********************************************************//
// 戻るボタン押下処理
//***********************************************************//
void CCmpnRegistWindow::OnBnClickedBtnBack()
{
	this->EndDialog(100);
	this->DestroyWindow();

}


//***********************************************************//
// クリアボタン押下処理
//***********************************************************//
void CCmpnRegistWindow::OnBnClickedBtnClr()
{
	// 画面クリア
	cCmpnNameEdit.SetWindowTextW(_T(""));
	cCmpnFuriEdit.SetWindowTextW(_T(""));

}


//***********************************************************//
// 登録ボタン押下処理
//***********************************************************//
void CCmpnRegistWindow::OnBnClickedBtnReg()
{
	int nRet;
	int nCmpnID;

	CDBAccess &clDBA = CDBAccess::getInstance();

	
//	std::string strCmpnId;
//	std::string strCmpnName;
	CString  strCmpnId;
	CString  strCmpnName;
	CString  strCmpnFuri;

	cCmpnIdEdit.GetWindowTextW(strCmpnId);
	cCmpnNameEdit.GetWindowTextW(strCmpnName);
	cCmpnFuriEdit.GetWindowTextW(strCmpnFuri);

	// 会社名入力チェック
	if (strCmpnName.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("会社名は必須入力です。"));
		return;

	}
	// フリガナ入力チェック
	if (strCmpnFuri.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("フリガナは必須入力です。"));
		return;

	}

	// 確認メッセージ
	nRet = MessageBoxW(_T("入力した内容にてマスタ登録を行います。よろしいですか？"), _T("確認"), MB_OKCANCEL + MB_DEFBUTTON2);
	if (nRet == IDCANCEL)
	{
		return;
	}

	nRet = clDBA.DBSetCmpnData(strCmpnId, strCmpnName, strCmpnFuri, bUpdFlg);

	this->EndDialog(100);
	this->DestroyWindow();

}
