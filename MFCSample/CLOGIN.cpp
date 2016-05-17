// CLOGIN.cpp : implementation file
//
/**
* @file CLOGIN.cpp
* @brief ログイン画面
* @date 2016/04/25
*/
#include "stdafx.h"
#include "MFCSample.h"
#include "CLOGIN.h"
#include "afxdialogex.h"
#include "CDBAccess.h"

// CLOGIN dialog

IMPLEMENT_DYNAMIC(CLOGIN, CDialogEx)

CLOGIN::CLOGIN(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

CLOGIN::~CLOGIN()
{
}

void CLOGIN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, cUserEdit);
	DDX_Control(pDX, IDC_EDIT2, cPasswordEdit);
	DDX_Control(pDX, IDC_BUTTON1, m_LonginBtn);
	DDX_Control(pDX, IDC_BUTTON2, m_ClearBtn);
}


BEGIN_MESSAGE_MAP(CLOGIN, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLOGIN::OnLogin)
	ON_BN_CLICKED(IDC_BUTTON2, &CLOGIN::OnClear)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLOGIN message handlers

/**
* @fn
* ログイン画面作成処理
* @brief ログイン画面作成処理
* @return なし
* @detail ログイン画面作成処理
*/
BOOL CLOGIN::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}

/**
* @fn
* ログインボタンクリック処理
* @brief ログインボタンクリック処理
* @return なし
* @detail ログインボタンクリック処理
*/
void CLOGIN::OnLogin()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	std::wstring strwUserName;
	std::wstring strwPassword;
	std::string strDBUserName;
	std::string strDBPassword;
	CString strUserName;
	CString strPassword;
	long lChkFlg;
	std::wstring strErrorMsg;

	/* DBアクセスインスタンスを参照 */
	CDBAccess &cDBA = CDBAccess::getInstance();

	/* 文字列参照 */
	cUserEdit.GetWindowTextW(strUserName);
	cPasswordEdit.GetWindowTextW(strPassword);
	strwUserName = strUserName;
	strwPassword = strPassword;

	/* 文字変換 */
	cDBA.StrConv(strDBUserName, strwUserName);
	cDBA.StrConv(strDBPassword, strwPassword);

	/* アクセス */
	lChkFlg = cDBA.DBLoginChk(strDBUserName, strDBPassword);

	switch (lChkFlg)
	{
	case 0:
		/* メイン画面の表示 */
		//this->ShowWindow(SW_HIDE);
		//cMainWindow.DoModal();
		((CMFCSampleApp*)m_pParentApp)->SetLoginSts(true);
		this->EndDialog(100);

		break;

	case 1:
		MessageBox(_T("DB接続異常"));

		strErrorMsg = cDBA.OutErrorMsg();
		MessageBox(strErrorMsg.c_str());

		break;

	case 2:
		MessageBox(_T("失敗しました。再入力してください"));
		break;

	default:
		break;
	}

}

/**
* @fn
* クリアボタンクリック処理
* @brief クリアボタンクリック処理
* @return なし
* @detail クリアボタンクリック処理
*/
void CLOGIN::OnClear()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	cUserEdit.SetWindowTextW(_T(""));
	cPasswordEdit.SetWindowTextW(_T(""));
}

/**
* @fn
* クローズボタンクリック処理
* @brief クローズボタンクリック処理
* @return なし
* @detail クローズボタンクリック処理
*/
void CLOGIN::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CDialogEx::OnClose();
}

/**
* @fn
* メッセージディスパッチ前処理
* @brief メッセージディスパッチ前処理
* @return なし
* @detail メッセージディスパッチ前処理
*/
BOOL CLOGIN::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

/**
* @fn
* アプリケーションポインタ設定処理
* @brief アプリケーションポインタ設定処理
* @return なし
* @detail アプリケーションポインタ設定処理
*/
void CLOGIN::ParentApp(CWinApp * pParentApp)
{
	m_pParentApp = pParentApp;
}

/**
* @fn
* ログイン画面の初期化
* @brief ログイン画面の初期化
* @return なし
* @detail ログイン画面の初期化
*/
BOOL CLOGIN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
