// CAnkenDetailWindow.cpp : implementation file
//
/**
* @file CAnkenDetailWindow.cpp
* @brief �Č��ڍ׏��\�����
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
* �Č��ڍ׉�ʂ̏�����
* @brief �Č��ڍ׉�ʂ̏�����
* @return �Ȃ�
* @detail �Č��ڍ׉�ʂ̏�����
*/
BOOL CAnkenDetailWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::wstring strwWork;
	CTime cSetTime;
	std::string strSkill;
	int i = 0;

	/* ����������*/
	m_AnkenNameEdit.SetLimitText(20);
	m_OrverEdit.SetLimitText(2000);
	m_OtherEdit.SetLimitText(1000);

	// �Č�����
	clDBA.WStrConv(strwWork, m_ProjectData.strProjectName);
	m_AnkenNameEdit.SetWindowTextW(strwWork.c_str());

	// �S����
	SetUserCombo();

	// ������
	cSetTime = GetTimeDay(m_ProjectData.strGenDay);
	m_GenDayEdit.SetTime(&cSetTime);

	// ��Ж�
	SetCmpCombo();

	// ����
	cSetTime = GetTimeDay(m_ProjectData.strPeriFrom);
	m_FromDayEdit.SetTime(&cSetTime);
	cSetTime = GetTimeDay(m_ProjectData.strPeriTo);
	m_ToDayEdit.SetTime(&cSetTime);

	//����
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

	// �X�L���ݒ�
	for (std::vector<std::string>::iterator it = m_ProjectData.vSkillList.begin();
		it != m_ProjectData.vSkillList.end();
		it++)
	{
		// ����
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

	//�T�v
	clDBA.WStrConv(strwWork, m_ProjectData.strOrverView);
	m_OrverEdit.SetWindowTextW(strwWork.c_str());

	//���̑�
	clDBA.WStrConv(strwWork, m_ProjectData.strOther);
	m_OtherEdit.SetWindowTextW(strwWork.c_str());

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/**
* @fn
* �Č����ݒ菈��
* @brief �Č����ݒ菈��
* @param (������) �����̐���
* @param (������) �����̐���
* @return �Ȃ�
* @detail �Č����ݒ菈��
*/
void CAnkenDetailWindow::SetProjectData(CProjectData& cProjectData, std::vector<CUserData>& vUserData)
{
	// �\�����̐ݒ�
	m_ProjectData = cProjectData;
	m_vUserData = vUserData;
}

/**
* @fn
* �S���ҏ��̐ݒ菈��
* @brief �S���ҏ��̐ݒ菈��
* @return �Ȃ�
* @detail �S���ҏ��̐ݒ菈��
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
* �������Q�Ə���
* @brief �������Q�Ə���
* @param (������) �����̐���
* @return �Ȃ�
* @detail �������Q�Ə���
*/
CTime CAnkenDetailWindow::GetTimeDay(std::string& strGetTime)
{
	/* ���ԏ����� */
	std::string strYear;
	std::string strMonth;
	std::string strDay;

	strYear = strGetTime.substr(0, 4);
	strMonth = strGetTime.substr(5, 2);
	strDay = strGetTime.substr(8, 2);

	/* �N������ݒ� */
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
* �X�L���ݒ�{�^���N���b�N����
* @brief �X�L���ݒ�{�^���N���b�N����
* @return �Ȃ�
* @detail �X�L���ݒ�{�^���N���b�N����
*/
void CAnkenDetailWindow::OnBnClickedButton4()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDBAccess &clDBA = CDBAccess::getInstance();
	bool bFirst = true;
	std::string strSkillName;
	std::wstring strWork;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	/* �X�L������������ */
	m_vSkillData.clear();
	nDBRtn = clDBA.DBGetSkillMasta(m_vSkillData);

	/* DB�ُ펞 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	/* �X�L���I����ʂ̕\�� */
	CSkillSelectWindow cSkillSelectWindow;

	cSkillSelectWindow.SetSkillData(&m_vSkillData);
	cSkillSelectWindow.DoModal();

	/* �X�L����ݒ� */
	for (std::vector<CSkillData>::iterator it = m_vSkillData.begin();
		it != m_vSkillData.end();
		it++)
	{
		// �I���X�L����
		if (it->bSetSkill == true)
		{
			// ����
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
* �X�V�{�^���N���b�N����
* @brief �X�V�{�^���N���b�N����
* @return �Ȃ�
* @detail �X�V�{�^���N���b�N����
*/
void CAnkenDetailWindow::OnBnClickedButton1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nMessageRtn;
	CProjectData cProjectData;
	int nUpdateRtn;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	nMessageRtn = MessageBox(_T("�X�V�����s���܂���"), _T("�X�V���s"), MB_YESNO);

	if (nMessageRtn == IDNO)
	{
		/* �L�����Z�� */
		return;
	}

	/* ��ʏ��X�V */
	UpdateData();

	/* �X�V���ݒ� */
	nUpdateRtn = SetUpdateData(cProjectData);

	if (nUpdateRtn != 0)
	{
		/* ���ُ͈펞*/

	}

	/* ���t�^�����v���C�X */
	cProjectData.DateReplace();

	/* DB�X�V */
	nDBRtn = clDBA.DBUpdateProject(cProjectData);

	/* DB�ُ펞 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	MessageBox(_T("�X�V���������܂���\n�Č����������Ȃ��ă��X�g���X�V���Ă������B"));

	this->EndDialog(201);
}

/**
* @fn
* �X�V���ݒ菈��
* @brief �X�V���ݒ菈��
* @param (������) �����̐���
* @return �Ȃ�
* @detail �X�V���ݒ菈��
*/
int CAnkenDetailWindow::SetUpdateData(CProjectData& cProjectData)
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::wstring strTemp;
	CString strGetText;
	CTime cGetTime;
	int nCursel = 0;

	/* �Č�ID�Q�� */
	cProjectData.nProjectId = m_ProjectData.nProjectId;

	/* �Č����Q�� */
	m_AnkenNameEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strProjectName, strTemp);

	/* �I�𒆂̒S���� */
	nCursel = m_UserCombo.GetCurSel();
	cProjectData.nUserID = m_vUserData[nCursel].nUserID;

	/* �������� */
	m_GenDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strGenDay, strTemp);

	/* �I�𒆂̉�� */
	nCursel = m_CmpCombo.GetCurSel();
	cProjectData.nCmpID = m_vCmpData[nCursel].nCmpID;

	/* ���� */
	m_FromDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strPeriFrom, strTemp);

	m_ToDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strPeriTo, strTemp);

	/* ���� */
	if (m_ExtentionRadio == 0)
	{
		cProjectData.strExtention = "t";
	}
	else
	{
		cProjectData.strExtention = "f";
	}

	/* ���� */
	if (m_LongTermChk == TRUE)
	{
		cProjectData.strLongTerm = "t";
	}
	else
	{
		cProjectData.strLongTerm = "f";
	}

	/* ���� */
	if (m_SameDayChk == TRUE)
	{
		cProjectData.strSameDay = "t";
	}
	else
	{
		cProjectData.strSameDay = "f";
	}

	/* ���� */
	if (m_AnyTimeChk == TRUE)
	{
		cProjectData.strAnyTime = "t";
	}
	else
	{
		cProjectData.strAnyTime = "f";
	}

	/* �X�L�� */
	cProjectData.nPrjSkillID = m_ProjectData.nPrjSkillID;


	/* �X�L���X�V���s������ */
	if (m_vSkillData.size() != 0)
	{
		cProjectData.vSkillData = m_vSkillData;
		cProjectData.bSkillUpdateFlg = true;
	}

	/* �T�v */
	m_OrverEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strOrverView, strTemp);

	/* ���l */
	m_OtherEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strOther, strTemp);

	/* ���͕␳ */
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
* ��Џ��ݒ菈��
* @brief ��Џ��ݒ菈��
* @return �Ȃ�
* @detail ��Џ��ݒ菈��
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

	/* ��Џ��̎Q�� */
	nDBRtn = clDBA.DBGetCmpData(m_vCmpData);

	/* DB�ُ펞 */
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
* �폜�{�^���N���b�N����
* @brief �폜�{�^���N���b�N����
* @return �Ȃ�
* @detail �폜�{�^���N���b�N����
*/
void CAnkenDetailWindow::OnBnClickedButton3()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nMessageRtn;
	CProjectData cProjectData;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	nMessageRtn = MessageBox(_T("�폜�����s���܂���"), _T("�폜���s"), MB_YESNO);

	if (nMessageRtn == IDNO)
	{
		/* �L�����Z�� */
		return;
	}

	/* �폜�Ώۂ�ݒ� */
	cProjectData.nProjectId = m_ProjectData.nProjectId;
	cProjectData.nPrjSkillID = m_ProjectData.nPrjSkillID;

	/* DB�X�V */
	nDBRtn = clDBA.DBDeleteProject(cProjectData);

	/* DB�ُ펞 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	MessageBox(_T("�폜���������܂���\n�Č����������Ȃ��ă��X�g���X�V���Ă������B"));

	this->EndDialog(202);
}


void CAnkenDetailWindow::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnOK();
}


void CAnkenDetailWindow::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	CDialogEx::OnCancel();
}
