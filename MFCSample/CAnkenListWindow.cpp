// CAnkenList.cpp : implementation file
//
/**
* @file CAnkenListWindow.cpp
* @brief �Č��ڕ�ꗗ���
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
* �Č��ڕ�ꗗ��ʂ̏�����
* @brief �Č��ڕ�ꗗ��ʂ̏�����
* @return �Ȃ�
* @detail �Č��ڕ�ꗗ��ʂ̏�����
*/
BOOL CAnkenListWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("�l�r �o�S�V�b�N"));
	m_AnkenLabel.SetFont(&SetFont);

	//�Č����X�g�̏�����
	InitAnkenListCtrl();
	
	//�R���{�{�b�N�X�̏�����
	SetUserCombo();

	// ���������ݒ�
	m_AnkenNameSearch.LimitText(20);
	m_CmpEdit.LimitText(60);

	/* ���ԏ����� */
	time_t tm1;
	struct tm *ltime;			// �����̍������߂�

	time(&tm1);					// �����̎��Ԃ��擾
	ltime = localtime(&tm1);
	ltime->tm_mon -= 1;			// ����1�����O�ɐݒ肷��

	tm1 = mktime(ltime);		// mktime�ōĐݒ肷��

	CTime ToTime(tm1);

	/* �Ђƌ��O�̎��Ԃ�ݒ� */
	m_ToDateEdit.SetTime(&ToTime);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/**
* @fn
* �Č����X�g�̏�����
* @brief �Č����X�g�̏�����
* @return �Ȃ�
* @detail �Č����X�g�̏�����
*/
void CAnkenListWindow::InitAnkenListCtrl()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("�Č�ID"), _T("�Č���"),
		_T("��Ж�") , _T("�S��") , _T("������"), _T("����"), 
		_T("�X�L��"), _T("�J�n��") };
	const int   clmNum = sizeof caption / sizeof caption[0];

	m_AnkenListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // �T�u�A�C�e���ԍ�
		lvc.pszText = caption[i];    // ���o���e�L�X�g
		lvc.cx = 93;				 // ����
		if (m_AnkenListCtrl.InsertColumn(i, &lvc) == -1) { break; }
	}
}

/**
* @fn
* �S���ҏ��̐ݒ菈��
* @brief �S���ҏ��̐ݒ菈��
* @return �Ȃ�
* @detail �S���ҏ��̐ݒ菈��
*/
void CAnkenListWindow::SetUserCombo()
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::vector<CUserData>::iterator it;
	std::wstring strwSetUser;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	m_vUserData.clear();

	/* �S���ҏ��̎Q�� */
	nDBRtn = clDBA.DBGetUserData(m_vUserData);

	/* DB�ُ펞 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	//�S�����p�ɐݒ�
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
* �Č���������
* @brief �Č���������
* @return �Ȃ�
* @detail �Č���������
*/
void CAnkenListWindow::OnSerachProject()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	m_vProjectData.clear();

	/* Wait�J�[�\�� */
	BeginWaitCursor();

	if(m_clProjectSearch.bDetailSearch == false)
	{
		/* ���������̐ݒ� */
		SetSearchData(m_clProjectSearch);
	}

	/* DB�Č����̌������s */ 
	nDBRtn = clDBA.DBGetProjectData(m_clProjectSearch, m_vProjectData);

	/* DB�ُ펞 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	/* �������̏������d���̂ŉ��P���邱�� */
	/* �X�L�����̌��� */
	for (std::vector<CProjectData>::iterator it = m_vProjectData.begin();
		it != m_vProjectData.end();
		it++)
	{
		// �X�L�����X�g�ݒ�
		nDBRtn = clDBA.DBGetSkillList(*it);

		/* DB�ُ펞 */
		if (nDBRtn == DB_NG)
		{
			strDBErrMsg = clDBA.OutErrorMsg();
			MessageBox(strDBErrMsg.c_str());
		}
	}

	if (m_vProjectData.size() == 0)
	{
		EndWaitCursor();
		MessageBox(_T("�������ʂ�0���ł�"));
	}
	else
	{
		/* �������ʂɕ\�� */
		SetProjectList(m_vProjectData);
	}

	/* �ڍ׌�����OFF */
	m_clProjectSearch.bDetailSearch = false;

	// �J�[�\����ʏ��
	EndWaitCursor();
}

/**
* @fn
* �Č����X�g�ݒ菈��
* @brief �Č����X�g�ݒ菈��
* @return �Ȃ�
* @detail �Č����X�g�ݒ菈��
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

	/* ���X�g�N���A */
	m_AnkenListCtrl.DeleteAllItems();

	for (std::vector<CProjectData>::iterator it = vProjectData.begin();
		it != vProjectData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		/* �v���W�F�N�gID */
		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		strTemp.Format(_T("%d"), it->nProjectId);
		lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp));
		m_AnkenListCtrl.InsertItem(&lvi);

		/* �v���W�F�N�g���� */
		lvi.iItem = nIndex;
		lvi.iSubItem = 1;
		clDBA.WStrConv(strwTemp, it->strProjectName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* ��Ж� */
		lvi.iItem = nIndex;
		lvi.iSubItem = 2;
		clDBA.WStrConv(strwTemp, it->strCmpName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* �S�� */
		lvi.iItem = nIndex;
		lvi.iSubItem = 3;
		clDBA.WStrConv(strwTemp, it->strUserName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* ������ */
		lvi.iItem = nIndex;
		lvi.iSubItem = 4;
		clDBA.WStrConv(strwTemp, it->strGenDay);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* ���� */
		lvi.iItem = nIndex;
		lvi.iSubItem = 5;
		strWork.clear();
		it->GetPeriod(strWork);
		clDBA.WStrConv(strwTemp, strWork);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		/* �X�L�� */
		strSkill.clear();

		for (std::vector<std::string>::iterator its = it->vSkillList.begin();
			its != it->vSkillList.end();
			its++)
		{
			// ����
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

		/* �J�n�� */
		lvi.iItem = nIndex;
		lvi.iSubItem = 7;
		clDBA.WStrConv(strwTemp, it->strPeriFrom);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_AnkenListCtrl.SetItem(&lvi);

		nIndex++;
	}

	/* �\�[�g�p�f�[�^�ݒ� */
	for (int i = 0; i < m_AnkenListCtrl.GetItemCount(); i++)
	{
		m_AnkenListCtrl.SetItemData(i, (DWORD)i);
	}
}

/**
* @fn
* ���������̐ݒ菈��
* @brief ���������̐ݒ菈��
* @return �Ȃ�
* @detail ���������̐ݒ菈��
*/
int CAnkenListWindow::SetSearchData(CProjectSearchData& SearchData)
{
	CUserData cTempUserData;
	int nCursel;
	CString strCTemp;
	std::wstring strwTempDate;
	CDBAccess &clDBA = CDBAccess::getInstance();
	CString cstrAnkenName;

	/* �Č�����ݒ� */
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

	/* �I�𒆂̒S���� */
	nCursel = m_UserCombo.GetCurSel();
	if (nCursel == 0)
	{
		SearchData.bUserSet = false;
	}
	else
	{
		SearchData.bUserSet = true;
		cTempUserData = m_vUserData[nCursel - 1];

		/* �������ɒS��ID��ݒ� */
		SearchData.nUserID = cTempUserData.nUserID;
	}

	/* �������Ԃ�ݒ� */
	/* To */
	m_ToDateEdit.GetWindowTextW(strCTemp);
	strwTempDate = strCTemp.GetBuffer();
	clDBA.StrConv(SearchData.strToDate, strwTempDate);

	/* From */
	m_FromDateEdit.GetWindowTextW(strCTemp);
	strwTempDate = strCTemp.GetBuffer();
	clDBA.StrConv(SearchData.strFromDate, strwTempDate);

	/* ��Ж���ݒ� */
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
* �������X�g�̃_�u���N���b�N����
* @brief �������X�g�̃_�u���N���b�N����
* @return �Ȃ�
* @detail �������X�g�̃_�u���N���b�N����
*/
void CAnkenListWindow::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int iSelected = m_AnkenListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CDBAccess &clDBA = CDBAccess::getInstance();
	CProjectData clProjectData;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	// �I�𒆂̏ꍇ
	if (iSelected >= 0)
	{
		CAnkenDetailWindow* AnkenDetailWindow = new CAnkenDetailWindow();
		CString strProjectID = m_AnkenListCtrl.GetItemText(iSelected, 0);
		CString strCmpName = m_AnkenListCtrl.GetItemText(iSelected, 2);
		std::wstring strwTemp;
		strwTemp = strCmpName.GetBuffer();

		/* �T�v���̎Q�� */
		nDBRtn = clDBA.DBGetProjectDetail(clProjectData, strProjectID);

		/* DB�ُ펞 */
		if (nDBRtn == DB_NG)
		{
			strDBErrMsg = clDBA.OutErrorMsg();
			MessageBox(strDBErrMsg.c_str());
		}

		/* �X�L�����X�g�̐ݒ� */
		nDBRtn = clDBA.DBGetSkillList(clProjectData);

		/* DB�ُ펞 */
		if (nDBRtn == DB_NG)
		{
			strDBErrMsg = clDBA.OutErrorMsg();
			MessageBox(strDBErrMsg.c_str());
		}

		/* ��Ж��ݒ� */
		clDBA.StrConv(clProjectData.strCmpName,strwTemp);

		/* ��ʏ��̐ݒ� */
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
* �ڍ׌����N���b�N����
* @brief �ڍ׌����N���b�N����
* @return �Ȃ�
* @detail �ڍ׌����N���b�N����
*/
void CAnkenListWindow::OnBnClickedButton2()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CAnkenSearchWindow clSearchWindow;

	/* ���������̐ݒ� */
	SetSearchData(m_clProjectSearch);

	/* �������̐ݒ� */
	clSearchWindow.SetSearchData(&m_clProjectSearch);

	/* ��ʂ̕\�� */
	clSearchWindow.DoModal();

	if (m_clProjectSearch.bDetailSearch == true)
	{
		//�������s
		OnSerachProject();
	}
}

/**
* @fn
* ���X�g�^�C�g���N���b�N����
* @brief ���X�g�^�C�g���N���b�N����
* @return �Ȃ�
* @detail ���X�g�^�C�g���N���b�N����
*/
void CAnkenListWindow::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B

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
* ���X�g�\�[�g����
* @brief ���X�g�\�[�g����
* @return �Ȃ�
* @detail ���X�g�\�[�g����
*/
int CAnkenListWindow::CompProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CAnkenListWindow* pMyDlg = (CAnkenListWindow*)lParamSort;
	int nSubItem = pMyDlg->m_iSubItem;
	int nItemSort = pMyDlg->m_iItemSort;
	CString strItem1 = pMyDlg->m_AnkenListCtrl.GetItemText(lParam1, nSubItem);
	CString strItem2 = pMyDlg->m_AnkenListCtrl.GetItemText(lParam2, nSubItem);

	// �Č�ID�̏ꍇ
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
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnOK();
}

void CAnkenListWindow::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnCancel();
}
