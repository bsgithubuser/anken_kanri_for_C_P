// CSkillSelectWindow.cpp : implementation file
//
/**
* @file CSkillSelectWindow.cpp
* @brief スキル情報選択画面
* @date 2016/04/25
*/

#include "stdafx.h"
#include "MFCSample.h"
#include "CSkillSelectWindow.h"
#include "afxdialogex.h"
#include "CDBAccess.h"

// CSkillSelectWindow dialog

IMPLEMENT_DYNAMIC(CSkillSelectWindow, CDialogEx)

CSkillSelectWindow::CSkillSelectWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SKILLSELECTWINDOW, pParent)
{

}

CSkillSelectWindow::~CSkillSelectWindow()
{
}

void CSkillSelectWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SkillSelectList);
}


BEGIN_MESSAGE_MAP(CSkillSelectWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSkillSelectWindow::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSkillSelectWindow message handlers

/**
* @fn
* スキル情報設定処理
* @brief スキル情報設定処理
* @return なし
* @detail スキル情報設定処理
*/
void CSkillSelectWindow::SetSkillData(std::vector<CSkillData>* pvSkillData)
{
	m_pvSkillData = pvSkillData;
}


/**
* @fn
* スキル情報選択画面の初期化
* @brief スキル情報選択画面の初期化
* @return なし
* @detail スキル情報選択画面の初期化
*/
BOOL CSkillSelectWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	//スキルリストの初期化
	InitSkillSelectList();

	// スキル名称を設定
	SetSkillList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/**
* @fn
* スキルリストの初期化
* @brief スキルリストの初期化
* @return なし
* @detail スキルリストの初期化
*/
void CSkillSelectWindow::InitSkillSelectList()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("スキル名称") };
	const int   clmNum = sizeof caption / sizeof caption[0];

	m_SkillSelectList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // サブアイテム番号
		lvc.pszText = caption[i];   // 見出しテキスト
		lvc.cx = 250;				 // 横幅
		if (m_SkillSelectList.InsertColumn(i, &lvc) == -1) { break; }
	}
}

/**
* @fn
* スキルリスト設定処理
* @brief スキルリスト設定処理
* @return なし
* @detail スキルリスト設定処理
*/
void CSkillSelectWindow::SetSkillList()
{
	LVITEM lvi;
	int nIndex = 0;
	CString strTemp;
	std::wstring strwTemp;
	std::string strWork;
	std::vector<CSkillData> vSkillData = *m_pvSkillData;
	CDBAccess &clDBA = CDBAccess::getInstance();

	/* リストクリア */
	m_SkillSelectList.DeleteAllItems();

	for (std::vector<CSkillData>::iterator it = vSkillData.begin();
		it != vSkillData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		/* スキル名称 */
		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		clDBA.WStrConv(strwTemp, it->strSkillName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_SkillSelectList.InsertItem(&lvi);

		nIndex++;
	}
}

/**
* @fn
* スキル設定ボタンクリック処理
* @brief スキル設定ボタンクリック処理
* @return なし
* @detail スキル設定ボタンクリック処理
*/
void CSkillSelectWindow::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int iSelected = -1;
	CSkillData *cSkillData;

	// 選択中のスキルを抽出
	do {
		iSelected = m_SkillSelectList.GetNextItem(iSelected, LVNI_SELECTED);

		//選択中のものはtrueに
		if (iSelected == -1)break;
		cSkillData = &(m_pvSkillData->at(iSelected));
		cSkillData->bSetSkill = true;

	} while (iSelected != -1);

	this->EndDialog(200);
}
