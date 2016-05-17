// CAnkenList.cpp : implementation file
//
/**
* @file CAnkenListWindow.cpp
* @brief 案件詳報一覧画面
* @date 2016/04/25
*/

#include "stdafx.h"
#include "MFCSample.h"
#include "CAnkenListWindow.h"
#include "afxdialogex.h"
#include "CDBAccess.h"
#include "CAnkenDetailWindow.h"
#include "CAnkenSearchWindow.h"

// CAnkenList dialog

IMPLEMENT_DYNAMIC(CAnkenListWindow, CDialogEx)

CAnkenListWindow::CAnkenListWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANKENLIST, pParent)
{

}

CAnkenListWindow::~CAnkenListWindow()
{
}

void CAnkenListWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANKEN_LABEL, m_AnkenLabel);
	DDX_Control(pDX, IDC_LIST1, m_AnkenListCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_UserCombo);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ToDateEdit);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_FromDateEdit);
	DDX_Control(pDX, IDC_EDIT2, m_CmpEdit);
	DDX_Control(pDX, IDC_EDIT1, m_AnkenNameSearch);
}


BEGIN_MESSAGE_MAP(CAnkenListWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnkenListWindow::OnSerachProject)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CAnkenListWindow::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnkenListWindow::OnBnClickedButton2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CAnkenListWindow::OnLvnColumnclickList1)
END_MESSAGE_MAP()


// CAnkenList message handlers

/**
* @fn
* 案件詳報一覧画面の初期化
* @brief 案件詳報一覧画面の初期化
* @return なし
* @detail 案件詳報一覧画面の初期化
*/
BOOL CAnkenListWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("ＭＳ Ｐゴシック"));
	m_AnkenLabel.SetFont(&SetFont);

	//案件リストの初期化
	InitAnkenListCtrl();
	
	//コンボボックスの初期化
	SetUserCombo();

	// 文字制限設定
	m_AnkenNameSearch.LimitText(20);
	m_CmpEdit.LimitText(60);

	/* 期間初期化 */
	time_t tm1;
	struct tm *ltime;			// 時刻の差を求める

	time(&tm1);					// 今日の時間を取得
	ltime = localtime(&tm1);
	ltime->tm_mon -= 1;			// 月を1ヶ月前に設定する

	tm1 = mktime(ltime);		// mktimeで再設定する

	CTime ToTime(tm1);

	/* ひと月前の時間を設定 */
	m_ToDateEdit.SetTime(&ToTime);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/**
* @fn
* 案件リストの初期化
* @brief 案件リストの初期化
* @return なし
* @detail 案件リストの初期化
*/
void CAnkenListWindow::InitAnkenListCtrl()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("案件ID"), _T("案件名"),
		_T("会社名") , _T("担当") , _T("発生日"), _T("期間"), 
		_T("スキル"), _T("開始日") };
	const int   clmNum = sizeof caption / sizeof caption[0];

	m_AnkenListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // サブアイテム番号
		lvc.pszText = caption[i];    // 見出しテキスト
		lvc.cx = 93;				 // 横幅
		if (m_AnkenListCtrl.InsertColumn(i, &lvc) == -1) { break; }
	}
}

/**
* @fn
* 担当者情報の設定処理
* @brief 担当者情報の設定処理
* @return なし
* @detail 担当者情報の設定処理
*/
void CAnkenListWindow::SetUserCombo()
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::vector<CUserData>::iterator it;
	std::wstring strwSetUser;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	m_vUserData.clear();

	/* 担当者情報の参照 */
	nDBRtn = clDBA.DBGetUserData(m_vUserData);

	/* DB異常時 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	//全検索用に設定
	m_UserCombo.InsertString(-1, _T(""));

	for (it = m_vUserData.begin();
		it != m_vUserData.end();
		it++)
	{
		clDBA.WStrConv(strwSetUser, it->strUserName);
		m_UserCombo.InsertString(-1, strwSetUser.c_str());
	}
	m_UserCombo.SetCurSel(0);
}

/**
* @fn
* 案件検索処理
* @brief 案件検索処理
* @return なし
* @detail 案件検索処理
*/
void CAnkenListWindow::OnSerachProject()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	m_vProjectData.clear();

	/* Waitカーソル */
	BeginWaitCursor();

	if(m_clProjectSearch.bDetailSearch == false)
	{
		/* 検索条件の設定 */
		SetSearchData(m_clProjectSearch);
	}

	/* DB案件情報の検索実行 */ 
	nDBRtn = clDBA.DBGetProjectData(m_clProjectSearch, m_vProjectData);

	/* DB異常時 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	/* ※ここの処理が重いので改善すること */
	/* スキル情報の検索 */
	for (std::vector<CProjectData>::iterator it = m_vProjectData.begin();
		it != m_vProjectData.end();
		it++)
	{
		// スキルリスト設定
		nDBRtn = clDBA.DBGetSkillList(*it);

		/* DB異常時 */
		if (nDBRtn == DB_NG)
		{
			strDBErrMsg = clDBA.OutErrorMsg();
			MessageBox(strDBErrMsg.c_str());
		}
	}

	if (m_vProjectData.size() == 0)
	{
		EndWaitCursor();
		MessageBox(_T("検索結果は0件です"));
	}
	else
	{
		/* 検索結果に表示 */
		SetProjectList(m_vProjectData);
	}

	/* 詳細検索をOFF */
	m_clProjectSearch.bDetailSearch = false;

	// カーソルを通常に
	EndWaitCursor();
}

/**
* @fn
* 案件リスト設定処理
* @brief 案件リスト設定処理
* @return なし
* @detail 案件リスト設定処理
*/
void CAnkenListWindow::SetProjectList(std::vector<CProjectData>& vProjectData)
{
	LVITEM lvi;
	int nIndex = 0;
	CString strTemp;
	std::wstring strwTemp;
	std::string strWork;
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::string strSkill;
	bool bFirst = false;

	/* リストクリア */
	m_AnkenListCtrl.DeleteAllItems();

	for (std::vector<CProjectData>::iterator it = vProjectData.begin();
		it != vProjectData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		/* プロジェクトID */
		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		strTemp.Format(_T("%d"), it->nProjectId);
		lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp));
		m_AnkenListCtrl.InsertItem(&lvi);

		/* プロジェクト名称 */
		lvi.iItem = nIndex;
		lvi.iSubItem = 1;
		clDBA.WStrConv(strwTemp, it->strProjectName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* 会社名 */
		lvi.iItem = nIndex;
		lvi.iSubItem = 2;
		clDBA.WStrConv(strwTemp, it->strCmpName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* 担当 */
		lvi.iItem = nIndex;
		lvi.iSubItem = 3;
		clDBA.WStrConv(strwTemp, it->strUserName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* 発生日 */
		lvi.iItem = nIndex;
		lvi.iSubItem = 4;
		clDBA.WStrConv(strwTemp, it->strGenDay);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* 期間 */
		lvi.iItem = nIndex;
		lvi.iSubItem = 5;
		strWork.clear();
		it->GetPeriod(strWork);
		clDBA.WStrConv(strwTemp, strWork);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* スキル */
		strSkill.clear();

		for (std::vector<std::string>::iterator its = it->vSkillList.begin();
			its != it->vSkillList.end();
			its++)
		{
			// 初回
			if (bFirst == false)
			{
				strSkill = *its;
				bFirst = true;
			}
			else
			{
				strSkill = strSkill + "," + *its;
			}
		}
		bFirst = false;
		lvi.iItem = nIndex;
		lvi.iSubItem = 6;
		clDBA.WStrConv(strwTemp, strSkill);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* 開始日 */
		lvi.iItem = nIndex;
		lvi.iSubItem = 7;
		clDBA.WStrConv(strwTemp, it->strPeriFrom);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		nIndex++;
	}

	/* ソート用データ設定 */
	for (int i = 0; i < m_AnkenListCtrl.GetItemCount(); i++)
	{
		m_AnkenListCtrl.SetItemData(i, (DWORD)i);
	}
}

/**
* @fn
* 検索条件の設定処理
* @brief 検索条件の設定処理
* @return なし
* @detail 検索条件の設定処理
*/
int CAnkenListWindow::SetSearchData(CProjectSearchData& SearchData)
{
	CUserData cTempUserData;
	int nCursel;
	CString strCTemp;
	std::wstring strwTempDate;
	CDBAccess &clDBA = CDBAccess::getInstance();
	CString cstrAnkenName;

	/* 案件名を設定 */
	m_AnkenNameSearch.GetWindowTextW(strCTemp);
	if (strCTemp != "")
	{
		SearchData.bAnkenNameSet = true;
		strwTempDate = strCTemp.GetBuffer();
		clDBA.StrConv(SearchData.strAnkenName, strwTempDate);
	}
	else
	{
		SearchData.bAnkenNameSet = false;
	}

	/* 選択中の担当者 */
	nCursel = m_UserCombo.GetCurSel();
	if (nCursel == 0)
	{
		SearchData.bUserSet = false;
	}
	else
	{
		SearchData.bUserSet = true;
		cTempUserData = m_vUserData[nCursel - 1];

		/* 検索情報に担当IDを設定 */
		SearchData.nUserID = cTempUserData.nUserID;
	}

	/* 検索期間を設定 */
	/* To */
	m_ToDateEdit.GetWindowTextW(strCTemp);
	strwTempDate = strCTemp.GetBuffer();
	clDBA.StrConv(SearchData.strToDate, strwTempDate);

	/* From */
	m_FromDateEdit.GetWindowTextW(strCTemp);
	strwTempDate = strCTemp.GetBuffer();
	clDBA.StrConv(SearchData.strFromDate, strwTempDate);

	/* 会社名を設定 */
	m_CmpEdit.GetWindowTextW(strCTemp);

	if(!strCTemp.IsEmpty())
	{ 
		strwTempDate = strCTemp.GetBuffer();
		clDBA.StrConv(SearchData.strCmpName, strwTempDate);
	}
	else
	{
		strwTempDate.clear();
		clDBA.StrConv(SearchData.strCmpName, strwTempDate);
	}

	return 0;
}

/**
* @fn
* 検索リストのダブルクリック処理
* @brief 検索リストのダブルクリック処理
* @return なし
* @detail 検索リストのダブルクリック処理
*/
void CAnkenListWindow::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int iSelected = m_AnkenListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CDBAccess &clDBA = CDBAccess::getInstance();
	CProjectData clProjectData;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	// 選択中の場合
	if (iSelected >= 0)
	{
		CAnkenDetailWindow* AnkenDetailWindow = new CAnkenDetailWindow();
		CString strProjectID = m_AnkenListCtrl.GetItemText(iSelected, 0);
		CString strCmpName = m_AnkenListCtrl.GetItemText(iSelected, 2);
		std::wstring strwTemp;
		strwTemp = strCmpName.GetBuffer();

		/* 概要情報の参照 */
		nDBRtn = clDBA.DBGetProjectDetail(clProjectData, strProjectID);

		/* DB異常時 */
		if (nDBRtn == DB_NG)
		{
			strDBErrMsg = clDBA.OutErrorMsg();
			MessageBox(strDBErrMsg.c_str());
		}

		/* スキルリストの設定 */
		nDBRtn = clDBA.DBGetSkillList(clProjectData);

		/* DB異常時 */
		if (nDBRtn == DB_NG)
		{
			strDBErrMsg = clDBA.OutErrorMsg();
			MessageBox(strDBErrMsg.c_str());
		}

		/* 会社名設定 */
		clDBA.StrConv(clProjectData.strCmpName,strwTemp);

		/* 画面情報の設定 */
		AnkenDetailWindow->SetProjectData(clProjectData,m_vUserData);
		AnkenDetailWindow->Create(IDD_ANKENDETAILWINDOW, this);
		AnkenDetailWindow->CenterWindow();
		AnkenDetailWindow->ShowWindow(SW_SHOW);
		AnkenDetailWindow->UpdateWindow();

	}

	*pResult = 0;
}

/**
* @fn
* 詳細検索クリック処理
* @brief 詳細検索クリック処理
* @return なし
* @detail 詳細検索クリック処理
*/
void CAnkenListWindow::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CAnkenSearchWindow clSearchWindow;

	/* 検索条件の設定 */
	SetSearchData(m_clProjectSearch);

	/* 検索情報の設定 */
	clSearchWindow.SetSearchData(&m_clProjectSearch);

	/* 画面の表示 */
	clSearchWindow.DoModal();

	if (m_clProjectSearch.bDetailSearch == true)
	{
		//検索実行
		OnSerachProject();
	}
}

/**
* @fn
* リストタイトルクリック処理
* @brief リストタイトルクリック処理
* @return なし
* @detail リストタイトルクリック処理
*/
void CAnkenListWindow::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	m_iSubItem = pNMLV->iSubItem;
	m_AnkenListCtrl.SortItems(CompProc, (LPARAM)this);

	if (m_iItemSort == SORT_UP)
	{
		m_iItemSort = SORT_DOWN;
	}
	else 
	{
		m_iItemSort = SORT_UP;
	}

	for (int i = 0; i < m_AnkenListCtrl.GetItemCount(); i++)
	{
		m_AnkenListCtrl.SetItemData(i, (DWORD)i);
	}

	*pResult = 0;
}

/**
* @fn
* リストソート処理
* @brief リストソート処理
* @return なし
* @detail リストソート処理
*/
int CAnkenListWindow::CompProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CAnkenListWindow* pMyDlg = (CAnkenListWindow*)lParamSort;
	int nSubItem = pMyDlg->m_iSubItem;
	int nItemSort = pMyDlg->m_iItemSort;
	CString strItem1 = pMyDlg->m_AnkenListCtrl.GetItemText(lParam1, nSubItem);
	CString strItem2 = pMyDlg->m_AnkenListCtrl.GetItemText(lParam2, nSubItem);

	// 案件IDの場合
	if (nSubItem == 0)
	{
		int nItem1 = 0;
		int nItem2 = 0;

		nItem1 = _ttoi(strItem1);
		nItem2 = _ttoi(strItem2);

		if (nItemSort == SORT_UP)
		{
			return (nItem1 - nItem2);
		}
		else
		{
			return ((nItem1 - nItem2) * -1);
		}
	}

	if (nItemSort == SORT_UP)
	{
		return(wcscmp(strItem1, strItem2));
	}
	else
	{
		return(wcscmp(strItem1, strItem2) * -1);
	}
}

void CAnkenListWindow::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnOK();
}

void CAnkenListWindow::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnCancel();
}
