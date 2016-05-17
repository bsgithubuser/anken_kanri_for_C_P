// CAnkenDetailWindow.cpp : implementation file
//
/**
* @file CAnkenDetailWindow.cpp
* @brief 案件詳細情報表示画面
* @date 2016/04/25
*/

#include "stdafx.h"
#include "MFCSample.h"
#include "CAnkenDetailWindow.h"
#include "CDBAccess.h"
#include "afxdialogex.h"
#include "CSkillSelectWindow.h"

// CAnkenDetailWindow dialog

IMPLEMENT_DYNAMIC(CAnkenDetailWindow, CDialogEx)

CAnkenDetailWindow::CAnkenDetailWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANKENDETAILWINDOW, pParent)
	, m_ExtentionRadio(0)
	, m_LongTermChk(FALSE)
	, m_SameDayChk(FALSE)
	, m_AnyTimeChk(FALSE)
{

}

CAnkenDetailWindow::~CAnkenDetailWindow()
{
}

void CAnkenDetailWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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


BEGIN_MESSAGE_MAP(CAnkenDetailWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnkenDetailWindow::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnkenDetailWindow::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnkenDetailWindow::OnBnClickedButton3)
END_MESSAGE_MAP()


// CAnkenDetailWindow message handlers

/**
* @fn
* 案件詳細画面の初期化
* @brief 案件詳細画面の初期化
* @return なし
* @detail 案件詳細画面の初期化
*/
BOOL CAnkenDetailWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::wstring strwWork;
	CTime cSetTime;
	std::string strSkill;
	int i = 0;

	/* 文字数制限*/
	m_AnkenNameEdit.SetLimitText(20);
	m_OrverEdit.SetLimitText(2000);
	m_OtherEdit.SetLimitText(1000);

	// 案件名称
	clDBA.WStrConv(strwWork, m_ProjectData.strProjectName);
	m_AnkenNameEdit.SetWindowTextW(strwWork.c_str());

	// 担当者
	SetUserCombo();

	// 発生日
	cSetTime = GetTimeDay(m_ProjectData.strGenDay);
	m_GenDayEdit.SetTime(&cSetTime);

	// 会社名
	SetCmpCombo();

	// 期間
	cSetTime = GetTimeDay(m_ProjectData.strPeriFrom);
	m_FromDayEdit.SetTime(&cSetTime);
	cSetTime = GetTimeDay(m_ProjectData.strPeriTo);
	m_ToDayEdit.SetTime(&cSetTime);

	//延長
	if(m_ProjectData.strExtention == "t")
	{
		m_ExtentionRadio = 0;
	}
	else
	{
		m_ExtentionRadio = 1;
	}
	
	if (m_ProjectData.strLongTerm == "t")
	{
		m_LongTermChk = true;
	}

	if (m_ProjectData.strSameDay == "t")
	{
		m_SameDayChk = true;
	}

	if (m_ProjectData.strAnyTime == "t")
	{
		m_AnyTimeChk = true;
	}

	// スキル設定
	for (std::vector<std::string>::iterator it = m_ProjectData.vSkillList.begin();
		it != m_ProjectData.vSkillList.end();
		it++)
	{
		// 初回
		if (i == 0)
		{
			strSkill = *it;
		}
		else
		{
			strSkill = strSkill + "," + *it;
		}

		i++;
	}

	clDBA.WStrConv(strwWork, strSkill);
	m_SkillNameEdit.SetWindowTextW(strwWork.c_str());

	//概要
	clDBA.WStrConv(strwWork, m_ProjectData.strOrverView);
	m_OrverEdit.SetWindowTextW(strwWork.c_str());

	//その他
	clDBA.WStrConv(strwWork, m_ProjectData.strOther);
	m_OtherEdit.SetWindowTextW(strwWork.c_str());

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/**
* @fn
* 案件情報設定処理
* @brief 案件情報設定処理
* @param (引数名) 引数の説明
* @param (引数名) 引数の説明
* @return なし
* @detail 案件情報設定処理
*/
void CAnkenDetailWindow::SetProjectData(CProjectData& cProjectData, std::vector<CUserData>& vUserData)
{
	// 表示情報の設定
	m_ProjectData = cProjectData;
	m_vUserData = vUserData;
}

/**
* @fn
* 担当者情報の設定処理
* @brief 担当者情報の設定処理
* @return なし
* @detail 担当者情報の設定処理
*/
void CAnkenDetailWindow::SetUserCombo()
{
	std::vector<CUserData>::iterator it;
	std::wstring strwSetUser;
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nSetIndex = 0;
	int nCurselCnt = 0;

	for (it = m_vUserData.begin();
		it != m_vUserData.end();
		it++)
	{
		clDBA.WStrConv(strwSetUser, it->strUserName);
		m_UserCombo.InsertString(-1, strwSetUser.c_str());

		if (it->nUserID == m_ProjectData.nUserID)
		{
			nSetIndex = nCurselCnt;
		}
		nCurselCnt++;
	}

	m_UserCombo.SetCurSel(nSetIndex);
}

/**
* @fn
* 日時情報参照処理
* @brief 日時情報参照処理
* @param (引数名) 引数の説明
* @return なし
* @detail 日時情報参照処理
*/
CTime CAnkenDetailWindow::GetTimeDay(std::string& strGetTime)
{
	/* 期間初期化 */
	std::string strYear;
	std::string strMonth;
	std::string strDay;

	strYear = strGetTime.substr(0, 4);
	strMonth = strGetTime.substr(5, 2);
	strDay = strGetTime.substr(8, 2);

	/* 年月日を設定 */
	CTime ToTime(
		atol(strYear.c_str()),
		atol(strMonth.c_str()),
		atol(strDay.c_str()),
		0,
		0,
		0);

	return ToTime;
}

/**
* @fn
* スキル設定ボタンクリック処理
* @brief スキル設定ボタンクリック処理
* @return なし
* @detail スキル設定ボタンクリック処理
*/
void CAnkenDetailWindow::OnBnClickedButton4()
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
* 更新ボタンクリック処理
* @brief 更新ボタンクリック処理
* @return なし
* @detail 更新ボタンクリック処理
*/
void CAnkenDetailWindow::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nMessageRtn;
	CProjectData cProjectData;
	int nUpdateRtn;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	nMessageRtn = MessageBox(_T("更新を実行しますか"), _T("更新実行"), MB_YESNO);

	if (nMessageRtn == IDNO)
	{
		/* キャンセル */
		return;
	}

	/* 画面情報更新 */
	UpdateData();

	/* 更新情報設定 */
	nUpdateRtn = SetUpdateData(cProjectData);

	if (nUpdateRtn != 0)
	{
		/* 入力異常時*/

	}

	/* 日付型をリプレイス */
	cProjectData.DateReplace();

	/* DB更新 */
	nDBRtn = clDBA.DBUpdateProject(cProjectData);

	/* DB異常時 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	MessageBox(_T("更新が完了しました\n再検索をおこなってリストを更新してださい。"));

	this->EndDialog(201);
}

/**
* @fn
* 更新情報設定処理
* @brief 更新情報設定処理
* @param (引数名) 引数の説明
* @return なし
* @detail 更新情報設定処理
*/
int CAnkenDetailWindow::SetUpdateData(CProjectData& cProjectData)
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::wstring strTemp;
	CString strGetText;
	CTime cGetTime;
	int nCursel = 0;

	/* 案件ID参照 */
	cProjectData.nProjectId = m_ProjectData.nProjectId;

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
	cProjectData.nPrjSkillID = m_ProjectData.nPrjSkillID;


	/* スキル更新を行ったか */
	if (m_vSkillData.size() != 0)
	{
		cProjectData.vSkillData = m_vSkillData;
		cProjectData.bSkillUpdateFlg = true;
	}

	/* 概要 */
	m_OrverEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strOrverView, strTemp);

	/* 備考 */
	m_OtherEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strOther, strTemp);

	/* 入力補正 */
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
void CAnkenDetailWindow::SetCmpCombo()
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::vector<CCmpData>::iterator it;
	std::wstring strwSetUser;
	int nCursel = 0;
	int nCurselCnt = 0;
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
		if (it->nCmpID == m_ProjectData.nCmpID)
		{
			nCursel = nCurselCnt;
		}
		nCurselCnt++;
	}

	m_CmpCombo.SetCurSel(nCursel);
}

/**
* @fn
* 削除ボタンクリック処理
* @brief 削除ボタンクリック処理
* @return なし
* @detail 削除ボタンクリック処理
*/
void CAnkenDetailWindow::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nMessageRtn;
	CProjectData cProjectData;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	nMessageRtn = MessageBox(_T("削除を実行しますか"), _T("削除実行"), MB_YESNO);

	if (nMessageRtn == IDNO)
	{
		/* キャンセル */
		return;
	}

	/* 削除対象を設定 */
	cProjectData.nProjectId = m_ProjectData.nProjectId;
	cProjectData.nPrjSkillID = m_ProjectData.nPrjSkillID;

	/* DB更新 */
	nDBRtn = clDBA.DBDeleteProject(cProjectData);

	/* DB異常時 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	MessageBox(_T("削除が完了しました\n再検索をおこなってリストを更新してださい。"));

	this->EndDialog(202);
}


void CAnkenDetailWindow::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDialogEx::OnOK();
}


void CAnkenDetailWindow::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDialogEx::OnCancel();
}
