// CSkillWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CSkillWindow.h"
#include "CSkillRegistWindow.h"
#include "afxdialogex.h"
#include "CSkillData.h"
#include "CDBAccess.h"

#include <vector>

// CSkillWindow dialog

IMPLEMENT_DYNAMIC(CSkillWindow, CDialogEx)

CSkillWindow::CSkillWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SKILLWINDOW, pParent)
{

}

CSkillWindow::~CSkillWindow()
{
}

void CSkillWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKILL_MASTA, m_MastaTitle);
	DDX_Control(pDX, IDC_LIST2, m_ListCtl);
}


BEGIN_MESSAGE_MAP(CSkillWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSkillWindow::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CSkillWindow::OnNMRClickList2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CSkillWindow::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BTN_DATAREG, &CSkillWindow::OnBnClickedBtnDatareg)
	ON_BN_CLICKED(IDC_BTN_DATAEDIT, &CSkillWindow::OnBnClickedBtnDataedit)
	ON_BN_CLICKED(IDC_BTN_DATADEL, &CSkillWindow::OnBnClickedBtnDatadel)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &CSkillWindow::OnBnClickedBtnRedraw)
END_MESSAGE_MAP()

// CSkillWindow message handlers


BOOL CSkillWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("ＭＳ Ｐゴシック"));
	m_MastaTitle.SetFont(&SetFont);

	/* スキルリストの初期設定*/
	InitSkillList();

	/* スキル情報の参照 */
	SkillListUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//***********************************************************//
// スキル一覧の初期処理
//***********************************************************//
void CSkillWindow::InitSkillList()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("スキルID"), _T("スキル名") };
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

//***********************************************************//
// スキル一覧の更新処理
//***********************************************************//
void CSkillWindow::SkillListUpdate()
{
	CSkillData tempSkillData;
	LVITEM lvi;
	int nIndex = 0;
	CString strTemp;
	std::wstring strwTemp;

	m_vSkillData.clear();

	CDBAccess &clDBA = CDBAccess::getInstance();

	// スキル管理情報の取得
	clDBA.DBGetSkillData(m_vSkillData);

	for (std::vector<CSkillData>::iterator it = m_vSkillData.begin();
		it != m_vSkillData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		// スキルIDのセット
		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		strTemp.Format(_T("%d"), it->nSkillID);
		lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp));
		m_ListCtl.InsertItem(&lvi);

		// スキル名のセット
		lvi.iItem = nIndex;
		lvi.iSubItem = 1;
		strwTemp = clDBA.ToWideStr(it->strSkillName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);


	}

}


void CSkillWindow::OnBnClickedButton1()
{
	// 子画面作成
	CRect SetRect;
//	m_TabCtl.GetWindowRect(SetRect);
	this->ScreenToClient(SetRect);
	SetRect.OffsetRect(0, -25);
	SetRect.bottom = SetRect.bottom + 25;

	this->ShowWindow(SW_HIDE);
	m_cSkillRegistWindow.Create(IDD_SKILLREGISTWINDOW, this);
	m_cSkillRegistWindow.MoveWindow(SetRect, 0);
	m_cSkillRegistWindow.ShowWindow(SW_SHOW);
}


void CSkillWindow::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CMenu    *popupMenuP = NULL;
	CMenu    cMenu;
	CPoint	 posMouse;
	int      err = 0;

	// メニューをロード
	if (!err) if (!cMenu.LoadMenu(IDR_RMENU_SKILL)) err = 1;
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

	*pResult = 0;
}


void CSkillWindow::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}


//***********************************************************//
// 新規登録ボタン押下処理
//***********************************************************//
void CSkillWindow::OnBnClickedBtnDatareg()
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
	nRet = clDBA.DBGetSeqquence("skill_id_seq", nSequence);
	strSequence = nSequence;

	strTemp.Format(_T("%d"), nSequence);
	//	lpcSequence = static_cast<LPCTSTR>(strSequence.c_str());

	// 登録画面を生成
	m_cSkillRegistWindow.Create(IDD_SKILL_REG_WINDOW, this);
	//	m_cSkillRegistWindow.cSkillIdEdit.SetWindowTextW(const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp)));

	// スキルIDを設定
	m_cSkillRegistWindow.cSkillIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cSkillRegistWindow.bUpdFlg = FALSE;

	// 登録画面を表示
	m_cSkillRegistWindow.ShowWindow(SW_SHOW);
	//	m_cSkillRegistWindow.DoModal();


}


//***********************************************************//
// 編集ボタン押下処理
//***********************************************************//
void CSkillWindow::OnBnClickedBtnDataedit()
{
	int iSelected;
	CString strTemp;

		
	// スキル一覧の選択行を取得
	iSelected = m_ListCtl.GetNextItem(-1, LVNI_SELECTED);

	// 未選択時は何もしない
	if (iSelected < 0)
	{
		return;
	}

	// スキル登録画面の生成
	m_cSkillRegistWindow.Create(IDD_SKILL_REG_WINDOW, this);

	// スキル一覧からIDを取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	// 空欄行選択時はエラー
	// 後で・・・

	m_cSkillRegistWindow.cSkillIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// スキル一覧からスキル名を取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 1);
	m_cSkillRegistWindow.cSkillNameEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cSkillRegistWindow.bUpdFlg = TRUE;

	// スキル登録画面の表示
	m_cSkillRegistWindow.ShowWindow(SW_SHOW);

}


//***********************************************************//
// 削除ボタン押下処理
//***********************************************************//
void CSkillWindow::OnBnClickedBtnDatadel()
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

	// スキル一覧からIDを取得
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	nRet = clDBA.DBSetSkillData(strTemp, NULL, 2);


}


//***********************************************************//
// 再表示ボタン押下処理
//***********************************************************//
void CSkillWindow::OnBnClickedBtnRedraw()
{
	m_ListCtl.DeleteAllItems();

	SkillListUpdate();
}


void CSkillWindow::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnOK();
}


void CSkillWindow::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnCancel();
}
