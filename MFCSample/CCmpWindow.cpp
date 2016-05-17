// CCompanyWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CCmpWindow.h"
#include "afxdialogex.h"
#include "CCompanyData.h"
#include "CDBAccess.h"

#include <vector>

// CCompanyWindow dialog

IMPLEMENT_DYNAMIC(CCompanyWindow, CDialogEx)

CCompanyWindow::CCompanyWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CMPNWINDOW, pParent)
{

}

CCompanyWindow::~CCompanyWindow()
{
}

void CCompanyWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMPN_MASTA, m_MastaTitle);
	DDX_Control(pDX, IDC_LIST2, m_ListCtl);
}


BEGIN_MESSAGE_MAP(CCompanyWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCompanyWindow::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CCompanyWindow::OnNMRClickList2)
	ON_BN_CLICKED(IDC_BTN_DATAREG, &CCompanyWindow::OnBnClickedBtnDatareg)
	ON_BN_CLICKED(IDC_BTN_DATAEDIT, &CCompanyWindow::OnBnClickedBtnDataedit)
	ON_BN_CLICKED(IDC_BTN_DATADEL, &CCompanyWindow::OnBnClickedBtnDatadel)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &CCompanyWindow::OnBnClickedBtnRedraw)
END_MESSAGE_MAP()

// CCompanyWindow message handlers


BOOL CCompanyWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("ＭＳ Ｐゴシック"));
	m_MastaTitle.SetFont(&SetFont);

	/* 会社リストの初期設定*/
	InitCompanyList();

	/* 会社情報の参照 */
	CompanyListUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CCompanyWindow::InitCompanyList()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("会社ID"), _T("会社名"), _T("フリガナ") };
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;

	m_ListCtl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // サブアイテム番号
		lvc.pszText = caption[i];   // 見出しテキスト
		lvc.cx = 100;				 // 横幅
		if (m_ListCtl.InsertColumn(i, &lvc) == -1) { err = 1; break; }
	}

}

void CCompanyWindow::CompanyListUpdate()
{
	CCompanyData tempCompanyData;
	LVITEM lvi;
	int nIndex = 0;
	CString strTemp;
	std::wstring strwTemp;

	m_vCompanyData.clear();

	CDBAccess &clDBA = CDBAccess::getInstance();

	/* スキル情報の参照*/
	clDBA.DBGetCompanyData(m_vCompanyData);

	for (std::vector<CCompanyData>::iterator it = m_vCompanyData.begin();
		it != m_vCompanyData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		strTemp.Format(_T("%d"), it->nCompanyID);
		lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp));
		m_ListCtl.InsertItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 1;
		strwTemp = clDBA.ToWideStr(it->strCompanyName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 2;
		strwTemp = clDBA.ToWideStr(it->strCompanyName_Furi);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);

	}

}


void CCompanyWindow::OnBnClickedButton1()
{
/*
	// 子画面作成
	CRect SetRect;
	//	m_TabCtl.GetWindowRect(SetRect);
	this->ScreenToClient(SetRect);
	SetRect.OffsetRect(0, -25);
	SetRect.bottom = SetRect.bottom + 25;

	this->ShowWindow(SW_HIDE);
	m_cCompanyRegistWindow.Create(IDD_CompanyREGISTWINDOW, this);
	m_cCompanyRegistWindow.MoveWindow(SetRect, 0);
	m_cCompanyRegistWindow.ShowWindow(SW_SHOW);
*/

}

void CCompanyWindow::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
/*
	CMenu    *popupMenuP = NULL;
	CMenu    cMenu;
	CPoint	 posMouse;
	int      err = 0;

	// メニューをロード
	if (!err) if (!cMenu.LoadMenu(IDR_RMENU_Company)) err = 1;
	// サブメニューを取得
	if (!err) if ((popupMenuP = cMenu.GetSubMenu(0)) == NULL) err = 1;
	// メニューをポップアップ
	if (!err)
	{
		ClientToScreen(&posMouse);
		if (!popupMenuP->TrackPopupMenu(
			TPM_LEFTBUTTON, posMouse.x, posMouse.y, this)) err = 1;
	}
	// メニューを破棄
	cMenu.DestroyMenu();

	//	CDialog::OnRButtonUp(nFlags, point);
*/

	*pResult = 0;
}


//***********************************************************//
// 新規登録ボタン押下処理
//***********************************************************//
void CCompanyWindow::OnBnClickedBtnDatareg()
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
	nRet = clDBA.DBGetSeqquence("cmpn_id_seq", nSequence);
	strSequence = nSequence;

	strTemp.Format(_T("%d"), nSequence);
	//	lpcSequence = static_cast<LPCTSTR>(strSequence.c_str());

	// 登録画面を生成
	m_cCmpnRegistWindow.Create(IDD_CMPN_REG_WINDOW, this);
	//	m_cSkillRegistWindow.cSkillIdEdit.SetWindowTextW(const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp)));

	// スキルIDを設定
	m_cCmpnRegistWindow.cCmpnIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cCmpnRegistWindow.bUpdFlg = FALSE;

	// 登録画面を表示
	m_cCmpnRegistWindow.ShowWindow(SW_SHOW);


}


//***********************************************************//
// 編集ボタン押下処理
//***********************************************************//
void CCompanyWindow::OnBnClickedBtnDataedit()
{
	int iSelected;
	CString strTemp;


	// 会社一覧の選択行を取得
	iSelected = m_ListCtl.GetNextItem(-1, LVNI_SELECTED);

	// 未選択時は何もしない
	if (iSelected < 0)
	{
		return;
	}

	// 会社登録画面の生成
	m_cCmpnRegistWindow.Create(IDD_CMPN_REG_WINDOW, this);

	// 会社一覧からIDを取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	m_cCmpnRegistWindow.cCmpnIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// 会社一覧から会社名を取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 1);
	m_cCmpnRegistWindow.cCmpnNameEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// 会社一覧から会社名(フリガナ)を取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 2);
	m_cCmpnRegistWindow.cCmpnFuriEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cCmpnRegistWindow.bUpdFlg = TRUE;

	// スキル登録画面の表示
	m_cCmpnRegistWindow.ShowWindow(SW_SHOW);

}


//***********************************************************//
// 削除ボタン押下処理
//***********************************************************//
void CCompanyWindow::OnBnClickedBtnDatadel()
{
	int nRet;
	int iSelected;
	CString strTemp;
	CDBAccess &clDBA = CDBAccess::getInstance();

	// 会社一覧の選択行を取得
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

	// 会社一覧からIDを取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	nRet = clDBA.DBSetCmpnData(strTemp, NULL, NULL, 2);
}



//***********************************************************//
// 再表示ボタン押下処理
//***********************************************************//
void CCompanyWindow::OnBnClickedBtnRedraw()
{
	m_ListCtl.DeleteAllItems();

	CompanyListUpdate();
}


void CCompanyWindow::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnOK();
}


void CCompanyWindow::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnCancel();
}
