// CAnkenSearchWindow.cpp : implementation file
//
/**
* @file CAnkenSearchWindow.cpp
* @brief 案件詳細検索画面
* @date 2016/04/25
*/

#include "stdafx.h"
#include "MFCSample.h"
#include "CAnkenSearchWindow.h"
#include "afxdialogex.h"
#include "CDBAccess.h"
#include "CSkillSelectWindow.h"

// CAnkenSearchWindow dialog

IMPLEMENT_DYNAMIC(CAnkenSearchWindow, CDialogEx)

CAnkenSearchWindow::CAnkenSearchWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANKENSEARCHWINDOW, pParent)
	, m_ValidDay(FALSE)
{

}

CAnkenSearchWindow::~CAnkenSearchWindow()
{
}

void CAnkenSearchWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_FromDayEdit);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_ToDayEdit);
	DDX_Radio(pDX, IDC_RADIO1, m_ExtentionRadio);
	DDX_Check(pDX, IDC_CHECK1, m_LongTermChk);
	DDX_Check(pDX, IDC_CHECK2, m_SameDayChk);
	DDX_Check(pDX, IDC_CHECK3, m_AnyTimeChk);
	DDX_Control(pDX, IDC_EDIT4, m_SkillNameEdit);
	DDX_Check(pDX, IDC_CHECK4, m_ValidDay);
}


BEGIN_MESSAGE_MAP(CAnkenSearchWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnkenSearchWindow::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnkenSearchWindow::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK4, &CAnkenSearchWindow::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CAnkenSearchWindow message handlers

/**
* @fn
* 検索情報設定処理
* @brief 検索情報設定処理
* @return なし
* @detail 検索情報設定処理
*/
void CAnkenSearchWindow::SetSearchData(CProjectSearchData* cProjectSearch)
{
	m_pProjectSearchData = cProjectSearch;
}

/**
* @fn
* 案件詳細検索画面の初期化
* @brief 案件詳細検索画面の初期化
* @return なし
* @detail 案件詳細検索画面の初期化
*/
BOOL CAnkenSearchWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	m_ExtentionRadio = 1;

	/* まだ未作成のため非活性 */
	m_FromDayEdit.EnableWindow(false);
	m_ToDayEdit.EnableWindow(false);
	m_ValidDay = false;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/**
* @fn
* スキル設定ボタンクリック処理
* @brief スキル設定ボタンクリック処理
* @return なし
* @detail スキル設定ボタンクリック処理
*/
void CAnkenSearchWindow::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDBAccess &clDBA = CDBAccess::getInstance();
	bool bFirst = true;
	std::string strSkillName;
	std::wstring strWork;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	/* スキル情報を初期化 */
	m_vSkillData.clear();

	/* スキル情報の参照 */
	nDBRtn = clDBA.DBGetSkillMasta(m_vSkillData);

	/* DB異常時 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	/* スキル選択画面の表示 */
	CSkillSelectWindow cSkillSelectWindow;

	cSkillSelectWindow.SetSkillData(&m_vSkillData);
	cSkillSelectWindow.DoModal();

	/* スキルを設定 */
	for (std::vector<CSkillData>::iterator it = m_vSkillData.begin();
		it != m_vSkillData.end();
		it++)
	{
		// 選択スキルか
		if (it->bSetSkill == true)
		{
			// 初回
			if (bFirst == true)
			{
				strSkillName = it->strSkillName;
				bFirst = false;
			}
			else
			{
				strSkillName = strSkillName + "," + it->strSkillName;
			}
		}
	}

	clDBA.WStrConv(strWork, strSkillName);
	m_SkillNameEdit.SetWindowTextW(strWork.c_str());
}

/**
* @fn
* 詳細検索ボタンクリック処理
* @brief 詳細検索ボタンクリック処理
* @return なし
* @detail 詳細検索ボタンクリック処理
*/
void CAnkenSearchWindow::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::wstring strTemp;
	CString strGetText;
	CTime cGetTime;

	// 詳細検索を指定
	m_pProjectSearchData->bDetailSearch = true;

	// 検索するスキル情報を設定
	m_pProjectSearchData->vSkillData = m_vSkillData;

	// 画面更新
	UpdateData();

	/* 期間 */
	m_FromDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(m_pProjectSearchData->strPeriFrom, strTemp);

	m_ToDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(m_pProjectSearchData->strPeriTo, strTemp);

	/* 延長 */
	if (m_ExtentionRadio == 0)
	{
		m_pProjectSearchData->strExtention = "t";
	}
	else
	{
		m_pProjectSearchData->strExtention = "f";
	}

	/* 長期 */
	if (m_LongTermChk == TRUE)
	{
		m_pProjectSearchData->strLongTerm = "t";
	}
	else
	{
		m_pProjectSearchData->strLongTerm = "f";
	}

	/* 即日 */
	if (m_SameDayChk == TRUE)
	{
		m_pProjectSearchData->strSameDay = "t";
	}
	else
	{
		m_pProjectSearchData->strSameDay = "f";
	}

	/* 随時 */
	if (m_AnyTimeChk == TRUE)
	{
		m_pProjectSearchData->strAnyTime = "t";
	}
	else
	{
		m_pProjectSearchData->strAnyTime = "f";
	}

	// 画面を閉じる
	this->EndDialog(203);
}

/**
* @fn
* 期間指定チェック処理
* @brief 期間指定チェック処理
* @return なし
* @detail 期間指定チェック処理
*/
void CAnkenSearchWindow::OnBnClickedCheck4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData();

	if (m_ValidDay == true)
	{
		m_pProjectSearchData->bValidDay = true;
		m_FromDayEdit.EnableWindow(true);
		m_ToDayEdit.EnableWindow(true);
	}
	else
	{
		m_pProjectSearchData->bValidDay = false;
		m_FromDayEdit.EnableWindow(false);
		m_ToDayEdit.EnableWindow(false);
	}
}
