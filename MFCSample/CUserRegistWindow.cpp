// CUserRegistWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CUserWindow.h"
#include "CUserRegistWindow.h"
#include "CDBAccess.h"
#include "afxdialogex.h"


// CUserRegistWindow dialog

IMPLEMENT_DYNAMIC(CUserRegistWindow, CDialogEx)

CUserRegistWindow::CUserRegistWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USER_REG_WINDOW, pParent)
	, aaaa(_T(""))
{

}

CUserRegistWindow::~CUserRegistWindow()
{
}

BOOL CUserRegistWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//	m_combo1 = (CComboBox*)GetDlgItem(IDC_COMBO1);  // 標準

	// コンボボックスの初期値
	cAdminEdit.InsertString(0, _T("0:管理者"));
	cAdminEdit.InsertString(1, _T("1:一般"));
	cAdminEdit.InsertString(2, _T("9:開発者"));

	cAdminEdit.SetTopIndex(0);

	cError.SetWindowTextW(_T(""));


	return(TRUE);

}

void CUserRegistWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERID_EDIT, cUserIdEdit);
	DDX_Control(pDX, IDC_USERNAME_EDIT, cUserNameEdit);
	DDX_Control(pDX, IDC_PASS_EDIT, cPasswordEdit);
	DDX_Control(pDX, IDC_PASSCHK_EDIT, cPasswordEdit2);
	DDX_Control(pDX, IDC_COMBO1, cAdminEdit);
	DDX_Control(pDX, IDC_ERROR, cError);
}


BEGIN_MESSAGE_MAP(CUserRegistWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BACK, &CUserRegistWindow::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_CLR, &CUserRegistWindow::OnBnClickedBtnClr)
	ON_BN_CLICKED(IDC_BTN_REG, &CUserRegistWindow::OnBnClickedBtnReg)
END_MESSAGE_MAP()


// CUserRegistWindow message handlers


//***********************************************************//
// 戻るボタン押下処理
//***********************************************************//
void CUserRegistWindow::OnBnClickedBtnBack()
{
	this->EndDialog(100);
	this->DestroyWindow();

	//	m_cUserWindow.UserListUpdate();

}


//***********************************************************//
// クリアボタン押下処理
//***********************************************************//
void CUserRegistWindow::OnBnClickedBtnClr()
{

	// 画面クリア
	cUserNameEdit.SetWindowTextW(_T(""));
	cPasswordEdit.SetWindowTextW(_T(""));
	cPasswordEdit2.SetWindowTextW(_T(""));
	cAdminEdit.SetTopIndex(1);
	cError.SetWindowTextW(_T(""));


}


//***********************************************************//
// 登録ボタン押下処理
//***********************************************************//
void CUserRegistWindow::OnBnClickedBtnReg()
{
	int nRet;
	int nIndex;

	CDBAccess &clDBA = CDBAccess::getInstance();


	//	std::string strUserId;
	//	std::string strUserName;
	CString  strUserId;
	CString  strUserName;
	CString  strUserPass;
	CString  strUserPassChk;
	CString  strUserAdmin;

	cUserIdEdit.GetWindowTextW(strUserId);
	cUserNameEdit.GetWindowTextW(strUserName);
	cPasswordEdit.GetWindowTextW(strUserPass);
	cPasswordEdit2.GetWindowTextW(strUserPassChk);

	cAdminEdit.GetWindowTextW(strUserAdmin);

	// 担当者名入力チェック
	if (strUserName.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("担当者名は必須入力です。"));
		return;

	}
	// パスワード入力チェック
	if (strUserPass.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("パスワードは必須入力です。"));
		return;

	}
	// 確認パスワード入力チェック
	if (strUserPassChk.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("パスワード(確認)は必須入力です。"));
		return;

	}
	// パスワード相違チェック
	if (strUserPass != strUserPassChk)
	{
		cError.SetWindowTextW(_T("パスワードが確認用と一致しません。"));
		return;
	}

	//	// 管理者設定確認
	nIndex = cAdminEdit.GetCurSel();
	switch (nIndex)
	{
	case 0:
		strUserAdmin = _T("0");
		break;
	case 1:
		strUserAdmin = _T("1");
		break;
	case 2:
		strUserAdmin = _T("9");
		break;
	default:
		cError.SetWindowTextW(_T("管理者は必須入力です。"));
		return;
	}

	// 確認メッセージ
	nRet = MessageBoxW(_T("入力した内容にてマスタ登録を行います。よろしいですか？"), _T("確認"), MB_OKCANCEL + MB_DEFBUTTON2);
	if (nRet == IDCANCEL)
	{
		return;
	}



	nRet = clDBA.DBSetUserData(strUserId, strUserName, strUserPass, strUserAdmin, nUpdFlg);

	this->EndDialog(100);
	this->DestroyWindow();

}


void CUserRegistWindow::OnEnMaxtextPassEdit()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
