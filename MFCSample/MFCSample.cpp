// MFCSample.cpp : アプリケーションのクラス動作を定義します。
//
/**
* @file MFCSample.cpp
* @brief 案件管理アプリケーションクラス
* @date 2016/04/25
*/

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"

#include "MFCSample.h"
#include "CLOGIN.h"
#include "CMainWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSampleApp

BEGIN_MESSAGE_MAP(CMFCSampleApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMFCSampleApp::OnAppAbout)
END_MESSAGE_MAP()


// CMFCSampleApp コンストラクション

CMFCSampleApp::CMFCSampleApp()
{
	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// アプリケーションが共通言語ランタイム サポート (/clr) を使用して作成されている場合:
	//     1) この追加設定は、再起動マネージャー サポートが正常に機能するために必要です。
	//     2) 作成するには、プロジェクトに System.Windows.Forms への参照を追加する必要があります。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("MFCSample.AppID.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

// 唯一の CMFCSampleApp オブジェクトです。

CMFCSampleApp theApp;


// CMFCSampleApp 初期化

/**
* @fn
* 案件管理アプリケーションの初期化
* @brief 案件管理アプリケーションの初期化
* @return なし
* @detail 案件管理アプリケーションの初期化
*/
BOOL CMFCSampleApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です	
	// AfxInitRichEdit2();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));


	// メイン ウィンドウを作成するとき、このコードは新しいフレーム ウィンドウ オブジェクトを作成し、
	// それをアプリケーションのメイン ウィンドウにセットします
	CLOGIN cLoginWindow((CWnd*)this);
	CMainWindow cMainWindow((CWnd*)this);

	/* ロケール設定 */
	setlocale(LC_CTYPE, "");

	/* 親ポインタ設定 */
	cLoginWindow.ParentApp((CWinApp*)this);

	/* ダイアログ表示 */
	cLoginWindow.DoModal();

	if(m_bLoginSts == true)
	{
		cMainWindow.DoModal();
	}

	return TRUE;
}

/**
* @fn
* 案件管理アプリケーションの終了処理
* @brief 案件管理アプリケーションの終了処理
* @return なし
* @detail 案件管理アプリケーションの終了処理
*/
int CMFCSampleApp::ExitInstance()
{
	//TODO: 追加したリソースがある場合にはそれらも処理してください
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CMFCSampleApp メッセージ ハンドラー


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CMFCSampleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMFCSampleApp メッセージ ハンドラー


void CMFCSampleApp::SetLoginSts(bool bLoginSts)
{
	m_bLoginSts = bLoginSts;
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
