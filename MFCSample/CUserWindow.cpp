// CUserWindow.cpp : implementation file
//

#include "stdafx.h"
#include <atlbase.h>
#include <atlconv.h>
#include "MFCSample.h"
#include "CUserWindow.h"
#include "CUserRegistWindow.h"
#include "afxdialogex.h"
#include "CUserData.h"
#include "CDBAccess.h"

#include <vector>

// CUserWindow dialog

IMPLEMENT_DYNAMIC(CUserWindow, CDialogEx)

CUserWindow::CUserWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USERWINDOW, pParent)
{

}

CUserWindow::~CUserWindow()
{
}

void CUserWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_MASTA, m_MastaTitle);
	DDX_Control(pDX, IDC_LIST2, m_ListCtl);
}


BEGIN_MESSAGE_MAP(CUserWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DATAREG, &CUserWindow::OnBnClickedBtnDatareg)
	ON_BN_CLICKED(IDC_BTN_DATAEDIT, &CUserWindow::OnBnClickedBtnDataedit)
	ON_BN_CLICKED(IDC_BTN_DATADEL, &CUserWindow::OnBnClickedBtnDatadel)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &CUserWindow::OnBnClickedBtnRedraw)
END_MESSAGE_MAP()

// CUserWindow message handlers


BOOL CUserWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("ＭＳ Ｐゴシック"));
	m_MastaTitle.SetFont(&SetFont);

	/* 担当者リストの初期設定*/
	InitUserList();

	/* 担当者情報の参照 */
	UserListUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CUserWindow::InitUserList()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("担当者ID"), _T("担当者名"),
								_T("パスワード") , _T("管理者権限") };
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;

	m_ListCtl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // サブアイテム番号
		lvc.pszText = caption[i];   // 見出しテキスト
		if (i == 2)
		{
			lvc.cx = 0;				 // 横幅
		}
		else
		{
			lvc.cx = 100;					 // 横幅
		}

		if (m_ListCtl.InsertColumn(i, &lvc) == -1) { err = 1; break; }
	}

}

void CUserWindow::UserListUpdate()
{
	CUserData tempUserData;
	LVITEM lvi;
	int nIndex = 0;
	CString strTemp;
	std::wstring strwTemp;

	m_vUserData.clear();

	CDBAccess &clDBA = CDBAccess::getInstance();

	/* 担当者情報の参照*/
	clDBA.DBGetUserList(m_vUserData);

	for (std::vector<CUserData>::iterator it = m_vUserData.begin();
		it != m_vUserData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		strTemp.Format(_T("%d"), it->nUserID);
		lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp));
		m_ListCtl.InsertItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 1;
		strwTemp = clDBA.ToWideStr(it->strUserName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 2;
		clDBA.WStrConv(strwTemp, it->strPassword);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 3;
		clDBA.WStrConv(strwTemp, it->strAdmin);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);
	}

}


//***********************************************************//
// 新規登録ボタン押下処理
//***********************************************************//
void CUserWindow::OnBnClickedBtnDatareg()
{
	int nSequence;
	int nRet;
	CString strTemp;
	std::wstring strSequence;
	CDBAccess &clDBA = CDBAccess::getInstance();
	LPCTSTR lpcSequence;
	std::wstring strwTemp;

	// スキルIDをシーケンスから取得
	nSequence = 0;
	nRet = clDBA.DBGetSeqquence("users_id_seq", nSequence);
	strSequence = nSequence;

	strTemp.Format(_T("%d"), nSequence);
	//	lpcSequence = static_cast<LPCTSTR>(strSequence.c_str());

	// 登録画面を生成
	m_cUserRegistWindow.Create(IDD_USER_REG_WINDOW, this);

	// スキルIDを設定
	m_cUserRegistWindow.cUserIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cUserRegistWindow.nUpdFlg = 0;

	// 登録画面を表示
	m_cUserRegistWindow.ShowWindow(SW_SHOW);
	//	m_cSkillRegistWindow.DoModal();

}


//***********************************************************//
// 編集ボタン押下処理
//***********************************************************//
void CUserWindow::OnBnClickedBtnDataedit()
{
	int iSelected;
	CString strTemp;


	// 担当者一覧の選択行を取得
	iSelected = m_ListCtl.GetNextItem(-1, LVNI_SELECTED);

	// 未選択時は何もしない
	if (iSelected < 0)
	{
		return;
	}

	// 担当者登録画面の生成
	m_cUserRegistWindow.Create(IDD_USER_REG_WINDOW, this);

	// 担当者一覧からIDを取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	m_cUserRegistWindow.cUserIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// 担当者一覧からスキル名を取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 1);
	m_cUserRegistWindow.cUserNameEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// 担当者一覧からパスワードを取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 2);
	m_cUserRegistWindow.cPasswordEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// 担当者一覧からパスワードを取得(確認用にセット)
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 2);
	m_cUserRegistWindow.cPasswordEdit2.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// 管理者設定確認
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 3);
	if (strTemp.IsEmpty() == FALSE)
	{
		m_cUserRegistWindow.cAdminEdit.SetCurSel(StrToInt(strTemp));
	}

	m_cUserRegistWindow.nUpdFlg = 1;

	// スキル登録画面の表示
	m_cUserRegistWindow.ShowWindow(SW_SHOW);
}


//***********************************************************//
// 削除ボタン押下処理
//***********************************************************//
void CUserWindow::OnBnClickedBtnDatadel()
{
	int nRet;
	int iSelected;
	CString strTemp;
	CDBAccess &clDBA = CDBAccess::getInstance();

	// スキル一覧の選択行を取得
	iSelected = m_ListCtl.GetNextItem(-1, LVNI_SELECTED);

	// 未選択時は何もしない
	if (iSelected <= 0)
	{
		return;
	}

	// 確認メッセージ
	nRet = MessageBoxW(_T("選択した行を削除します。よろしいですか？"), _T("確認"), MB_OKCANCEL + MB_DEFBUTTON2);
	if (nRet == IDCANCEL)
	{
		OnBnClickedBtnRedraw();
		return;
	}

	// 担当者一覧からIDを取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	nRet = clDBA.DBSetUserData(strTemp, NULL, NULL, NULL, 2);


}


//***********************************************************//
// 再表示ボタン押下処理
//***********************************************************//
void CUserWindow::OnBnClickedBtnRedraw()
{
	m_ListCtl.DeleteAllItems();

	UserListUpdate();
}


void CUserWindow::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnOK();
}


void CUserWindow::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnCancel();
}
