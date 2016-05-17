// CAnkenRegistWindow.cpp : implementation file
//
/**
* @file CAnkenRegistWindow.cpp
* @brief 新規案件情報登録画面
* @date 2016/04/25
*/

#include "stdafx.h"
#include "MFCSample.h"
#include "CAnkenRegistWindow.h"
#include "afxdialogex.h"
#include "CDBAccess.h"
#include "CSkillSelectWindow.h"

// CAnkenRegistWindow dialog

IMPLEMENT_DYNAMIC(CAnkenRegistWindow, CDialogEx)

CAnkenRegistWindow::CAnkenRegistWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANKENREGISTWINDOW, pParent)
{

}

CAnkenRegistWindow::~CAnkenRegistWindow()
{
}

void CAnkenRegistWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANKENR_LABEL, m_AnkenRLabel);
	DDX_Control(pDX, IDC_EDIT5, m_AnkenNameEdit);
	DDX_Control(pDX, IDC_COMBO1, m_UserCombo);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_GenDayEdit);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_FromDayEdit);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_ToDayEdit);
	DDX_Radio(pDX, IDC_RADIO1, m_ExtentionRadio);
	DDX_Check(pDX, IDC_CHECK1, m_LongTermChk);
	DDX_Check(pDX, IDC_CHECK2, m_SameDayChk);
	DDX_Check(pDX, IDC_CHECK3, m_AnyTimeChk);
	DDX_Control(pDX, IDC_EDIT4, m_SkillNameEdit);
	DDX_Control(pDX, IDC_EDIT3, m_OrverEdit);
	DDX_Control(pDX, IDC_EDIT2, m_OtherEdit);
	DDX_Control(pDX, IDC_COMBO2, m_CmpCombo);
}


BEGIN_MESSAGE_MAP(CAnkenRegistWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnkenRegistWindow::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnkenRegistWindow::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnkenRegistWindow::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAnkenRegistWindow message handlers

/**
* @fn
* 新規案件情報登録画面の初期化
* @brief 新規案件情報登録画面の初期化
* @return なし
* @detail 新規案件情報登録画面の初期化
*/
BOOL CAnkenRegistWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("ＭＳ Ｐゴシック"));
	m_AnkenRLabel.SetFont(&SetFont);

	/* 文字数制限*/
	m_AnkenNameEdit.SetLimitText(20);
	m_OrverEdit.SetLimitText(2000);
	m_OtherEdit.SetLimitText(1000);

	/* 担当者リストの更新 */
	InitUserCombo();

	/* 会社リストの更新 */
	InitCmpCombo();

	m_ExtentionRadio = 1;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/**
* @fn
* 担当者情報の設定処理
* @brief 担当者情報の設定処理
* @return なし
* @detail 担当者情報の設定処理
*/
void CAnkenRegistWindow::InitUserCombo()
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
* 登録ボタンクリック処理
* @brief 登録ボタンクリック処理
* @return なし
* @detail 登録ボタンクリック処理
*/
void CAnkenRegistWindow::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nMessageRtn;
	CProjectData cProjectData;
	int nRegistRtn;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	nMessageRtn = MessageBox(_T("登録を実行しますか"),_T("登録実行"), MB_YESNO);
	
	if (nMessageRtn == IDNO)
	{
		/* キャンセル */
		return;
	}

	/* 画面情報更新 */
	UpdateData();

	/* 登録処理実行 */
	nRegistRtn = SetRegistData(cProjectData);

	if (nRegistRtn != 0)
	{
		/* 入力異常時*/

	}

	/* 日付型をリプレイス */
	cProjectData.DateReplace();

	/* DB登録 */
	nDBRtn = clDBA.DBSetNewProject(cProjectData);

	/* DB異常時 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	MessageBox(_T("登録が完了しました"));
}

/**
* @fn
* 登録情報の設定処理
* @brief 登録情報の設定処理
* @return なし
* @detail 登録情報の設定処理
*/
int CAnkenRegistWindow::SetRegistData(CProjectData& cProjectData)
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::wstring strTemp;
	CString strGetText;
	CTime cGetTime;
	int nCursel = 0;

	/* 案件名参照 */
	m_AnkenNameEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strProjectName, strTemp);

	/* 選択中の担当者 */
	nCursel = m_UserCombo.GetCurSel();
	cProjectData.nUserID = m_vUserData[nCursel].nUserID;

	/* 発生日時 */
	m_GenDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strGenDay, strTemp);

	/* 選択中の会社 */
	nCursel = m_CmpCombo.GetCurSel();
	cProjectData.nCmpID = m_vCmpData[nCursel].nCmpID;

	/* 期間 */
	m_FromDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strPeriFrom, strTemp);

	m_ToDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strPeriTo, strTemp);

	/* 延長 */
	if (m_ExtentionRadio == 0)
	{
		cProjectData.strExtention = "t";
	}
	else
	{
		cProjectData.strExtention = "f";
	}

	/* 長期 */
	if (m_LongTermChk == TRUE)
	{
		cProjectData.strLongTerm = "t";
	}
	else
	{
		cProjectData.strLongTerm = "f";
	}

	/* 即日 */
	if (m_SameDayChk == TRUE)
	{
		cProjectData.strSameDay = "t";
	}
	else
	{
		cProjectData.strSameDay = "f";
	}

	/* 随時 */
	if (m_AnyTimeChk == TRUE)
	{
		cProjectData.strAnyTime = "t";
	}
	else
	{
		cProjectData.strAnyTime = "f";
	}

	/* スキル */
	cProjectData.vSkillData = m_vSkillData;

	/* 概要 */
	m_OrverEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strOrverView, strTemp);

	/* 備考 */
	m_OtherEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strOther, strTemp);

	/* 入力チェック */
	if (cProjectData.strProjectName == "")
	{
		cProjectData.strProjectName = " ";
	}

	if (cProjectData.strOrverView == "")
	{
		cProjectData.strOrverView = " ";
	}

	if (cProjectData.strOther == "")
	{
		cProjectData.strOther = " ";
	}

	return 0;
}

/**
* @fn
* 会社情報設定処理
* @brief 会社情報設定処理
* @return なし
* @detail 会社情報設定処理
*/
void CAnkenRegistWindow::InitCmpCombo()
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::vector<CCmpData>::iterator it;
	std::wstring strwSetUser;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	m_vCmpData.clear();

	/* 会社情報の参照 */
	nDBRtn = clDBA.DBGetCmpData(m_vCmpData);

	/* DB異常時 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	for (it = m_vCmpData.begin();
		it != m_vCmpData.end();
		it++)
	{
		clDBA.WStrConv(strwSetUser, it->strCmpName);
		m_CmpCombo.InsertString(-1, strwSetUser.c_str());
	}

	m_CmpCombo.SetCurSel(0);
}

/**
* @fn
* スキル設定ボタンクリック処理
* @brief スキル設定ボタンクリック処理
* @return なし
* @detail スキル設定ボタンクリック処理
*/
void CAnkenRegistWindow::OnBnClickedButton4()
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
* クリアボタンクリック処理
* @brief クリアボタンクリック処理
* @return なし
* @detail クリアボタンクリック処理
*/
void CAnkenRegistWindow::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CTime    cTime = CTime::GetCurrentTime();

	m_AnkenNameEdit.SetWindowTextW(_T(""));

	m_UserCombo.SetCurSel(0);

	m_GenDayEdit.SetTime(&cTime);

	m_CmpCombo.SetCurSel(0);

	m_FromDayEdit.SetTime(&cTime);
	m_ToDayEdit.SetTime(&cTime);

	m_ExtentionRadio = 1;
	m_LongTermChk = false;
	m_AnyTimeChk = false;
	m_SameDayChk = false;

	m_SkillNameEdit.SetWindowTextW(_T(""));
	m_vSkillData.clear();

	m_OrverEdit.SetWindowTextW(_T(""));
	m_OtherEdit.SetWindowTextW(_T(""));

	UpdateData(false);
}


void CAnkenRegistWindow::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnOK();
}


void CAnkenRegistWindow::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnCancel();
}
