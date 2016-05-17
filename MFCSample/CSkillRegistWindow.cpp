// CSkillRegistWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CSkillRegistWindow.h"
#include "CDBAccess.h"
#include "afxdialogex.h"


// CSkillRegistWindow dialog

IMPLEMENT_DYNAMIC(CSkillRegistWindow, CDialogEx)

CSkillRegistWindow::CSkillRegistWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SKILL_REG_WINDOW, pParent)
{
//	cSkillIdEdite.SetWindowTextW(strUserName);
//	cSkillNamEdit.SetWindowTextW(strPassword);

}

CSkillRegistWindow::~CSkillRegistWindow()
{
}

void CSkillRegistWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKILLID_EDIT, cSkillIdEdit);
	DDX_Control(pDX, IDC_SKILLNAME_EDIT, cSkillNameEdit);
	DDX_Control(pDX, IDC_ERROR, cError);
}


BEGIN_MESSAGE_MAP(CSkillRegistWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BACK, &CSkillRegistWindow::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_CLR, &CSkillRegistWindow::OnBnClickedBtnClr)
	ON_BN_CLICKED(IDC_BTN_REG, &CSkillRegistWindow::OnBnClickedBtnReg)
END_MESSAGE_MAP()


BOOL CSkillRegistWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	cError.SetWindowTextW(_T(""));

	return(TRUE);

}


//***********************************************************//
// 戻るボタン押下処理
//***********************************************************//
void CSkillRegistWindow::OnBnClickedBtnBack()
{
	this->EndDialog(100);
	this->DestroyWindow();

}


//***********************************************************//
// クリアボタン押下処理
//***********************************************************//
void CSkillRegistWindow::OnBnClickedBtnClr()
{
	// 画面クリア
	cSkillNameEdit.SetWindowTextW(_T(""));

}


//***********************************************************//
// 登録ボタン押下処理
//***********************************************************//
void CSkillRegistWindow::OnBnClickedBtnReg()
{
	int nRet;
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int nSkillID;

	CDBAccess &clDBA = CDBAccess::getInstance();

	
//	std::string strSkillId;
//	std::string strSkillName;
	CString  strSkillId;
	CString  strSkillName;

	cSkillIdEdit.GetWindowTextW(strSkillId);
	cSkillNameEdit.GetWindowTextW(strSkillName);

	// スキル名入力チェック
	if (strSkillName.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("スキル名は必須入力です。"));
		return;

	}

	// 確認メッセージ
	nRet = MessageBoxW(_T("入力した内容にてマスタ登録を行います。よろしいですか？"), _T("確認"), MB_OKCANCEL + MB_DEFBUTTON2);
	if (nRet == IDCANCEL)
	{
		return;
	}

	nRet = clDBA.DBSetSkillData(strSkillId, strSkillName, bUpdFlg);

	this->EndDialog(100);
	this->DestroyWindow();

}
