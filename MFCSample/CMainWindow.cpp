// CMainWindow.cpp : implementation file
//
/**
* @file CMainWindow.cpp
* @brief 案件管理メイン画面
* @date 2016/04/25
*/

#include "stdafx.h"
#include "MFCSample.h"
#include "CMainWindow.h"
#include "afxdialogex.h"
#include "CDBAccess.h"

//#include "CLOGIN.h"

// CMainWindow dialog

IMPLEMENT_DYNAMIC(CMainWindow, CDialogEx)

CMainWindow::CMainWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINWINDOW, pParent)
{
	//Create(IDD_MAINWINDOW, pParent);
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtl);
	DDX_Control(pDX, IDC_BUTTON4, m_AnkenListBtn);
	DDX_Control(pDX, IDC_BUTTON1, m_UserBtn);
	DDX_Control(pDX, IDC_BUTTON2, m_SkillBtn);
	DDX_Control(pDX, IDC_BUTTON3, m_CmpBtn);
	DDX_Control(pDX, IDC_BUTTON6, m_AnkenReadBtn);
}


BEGIN_MESSAGE_MAP(CMainWindow, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMainWindow::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainWindow::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainWindow::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainWindow::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMainWindow::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMainWindow::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMainWindow message handlers

/**
* @fn
* クローズボタンクリック処理
* @brief クローズボタンクリック処理
* @return なし
* @detail クローズボタンクリック処理
*/
void CMainWindow::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	//CWnd* cParentWindow = this->GetParent();
	//((CLOGIN*)cParentWindow)->OnClose();

	CDialogEx::OnClose();
}

/**
* @fn
* 担当者管理ボタンクリック処理
* @brief 担当者管理ボタンクリック処理
* @return なし
* @detail 担当者管理ボタンクリック処理
*/
void CMainWindow::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	HideChildWindow();
	m_cUserWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* 案件管理メイン画面の初期化
* @brief 案件管理メイン画面の初期化
* @return なし
* @detail 案件管理メイン画面の初期化
*/
BOOL CMainWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	// 子画面作成
	CRect SetRect;
	m_TabCtl.GetWindowRect(SetRect);
	this->ScreenToClient(SetRect);
	SetRect.OffsetRect(0, -25);
	SetRect.bottom = SetRect.bottom + 25;

	/* 画面設定 */
	m_cUserWindow.Create(IDD_USERWINDOW, this);
	m_cUserWindow.MoveWindow(SetRect, 0);
	m_cUserWindow.ShowWindow(SW_HIDE);

	m_cSkillWindow.Create(IDD_SKILLWINDOW, this);
	m_cSkillWindow.MoveWindow(SetRect, 0);
	m_cSkillWindow.ShowWindow(SW_HIDE);

	m_cCmpWindow.Create(IDD_CMPNWINDOW, this);
	m_cCmpWindow.MoveWindow(SetRect, 0);
	m_cCmpWindow.ShowWindow(SW_HIDE);

	m_cAnkenListWindow.Create(IDD_ANKENLIST, this);
	m_cAnkenListWindow.MoveWindow(SetRect, 0);
	m_cAnkenListWindow.ShowWindow(SW_SHOW);

	m_cAnkenRegistWindow.Create(IDD_ANKENREGISTWINDOW, this);
	m_cAnkenRegistWindow.MoveWindow(SetRect, 0);
	m_cAnkenRegistWindow.ShowWindow(SW_HIDE);

	m_cAnkenReadWindow.Create(IDD_ANKENREADWINDOW, this);
	m_cAnkenReadWindow.MoveWindow(SetRect, 0);
	m_cAnkenReadWindow.ShowWindow(SW_HIDE);

	CDBAccess &cDBA = CDBAccess::getInstance();

	/* ログインは管理者以外か */
	if (cDBA.m_strAdmin != "0")
	{
		m_UserBtn.EnableWindow(FALSE);
		m_SkillBtn.EnableWindow(FALSE);
		m_CmpBtn.EnableWindow(FALSE);
	}

	/* 未作成のため非活性 */
	m_AnkenReadBtn.EnableWindow(FALSE);

	m_AnkenListBtn.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/**
* @fn
* 子画面非表示処理
* @brief 子画面非表示処理
* @return なし
* @detail 子画面非表示処理
*/
void CMainWindow::HideChildWindow()
{
	m_cUserWindow.ShowWindow(SW_HIDE);
	m_cSkillWindow.ShowWindow(SW_HIDE);
	m_cCmpWindow.ShowWindow(SW_HIDE);
	m_cAnkenListWindow.ShowWindow(SW_HIDE);
	m_cAnkenRegistWindow.ShowWindow(SW_HIDE);
	m_cAnkenReadWindow.ShowWindow(SW_HIDE);
}

/**
* @fn
* スキル管理ボタンクリック処理
* @brief スキル管理ボタンクリック処理
* @return なし
* @detail スキル管理ボタンクリック処理
*/
void CMainWindow::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	HideChildWindow();
	m_cSkillWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* 会社情報管理ボタンクリック処理
* @brief 会社情報管理ボタンクリック処理
* @return なし
* @detail 会社情報管理ボタンクリック処理
*/
void CMainWindow::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	HideChildWindow();
	m_cCmpWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* 案件情報一覧ボタンクリック処理
* @brief 案件情報一覧ボタンクリック処理
* @return なし
* @detail 案件情報一覧ボタンクリック処理
*/
void CMainWindow::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	HideChildWindow();
	m_cAnkenListWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* 案件情報登録ボタンクリック処理
* @brief 案件情報登録ボタンクリック処理
* @return なし
* @detail 案件情報登録ボタンクリック処理
*/
void CMainWindow::OnBnClickedButton5()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	HideChildWindow();
	m_cAnkenRegistWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* 案件情報取込ボタンクリック処理
* @brief 案件情報取込ボタンクリック処理
* @return なし
* @detail 案件情報取込ボタンクリック処理
*/
void CMainWindow::OnBnClickedButton6()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	HideChildWindow();
	m_cAnkenReadWindow.ShowWindow(SW_SHOW);
}


void CMainWindow::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnOK();
}


void CMainWindow::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDialogEx::OnCancel();
}
