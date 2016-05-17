// CAnkenRegistWindow.cpp : implementation file
//
/**
* @file CAnkenRegistWindow.cpp
* @brief �V�K�Č����o�^���
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
* �V�K�Č����o�^��ʂ̏�����
* @brief �V�K�Č����o�^��ʂ̏�����
* @return �Ȃ�
* @detail �V�K�Č����o�^��ʂ̏�����
*/
BOOL CAnkenRegistWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("�l�r �o�S�V�b�N"));
	m_AnkenRLabel.SetFont(&SetFont);

	/* ����������*/
	m_AnkenNameEdit.SetLimitText(20);
	m_OrverEdit.SetLimitText(2000);
	m_OtherEdit.SetLimitText(1000);

	/* �S���҃��X�g�̍X�V */
	InitUserCombo();

	/* ��Ѓ��X�g�̍X�V */
	InitCmpCombo();

	m_ExtentionRadio = 1;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/**
* @fn
* �S���ҏ��̐ݒ菈��
* @brief �S���ҏ��̐ݒ菈��
* @return �Ȃ�
* @detail �S���ҏ��̐ݒ菈��
*/
void CAnkenRegistWindow::InitUserCombo()
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
* �o�^�{�^���N���b�N����
* @brief �o�^�{�^���N���b�N����
* @return �Ȃ�
* @detail �o�^�{�^���N���b�N����
*/
void CAnkenRegistWindow::OnBnClickedButton3()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDBAccess &clDBA = CDBAccess::getInstance();
	int nMessageRtn;
	CProjectData cProjectData;
	int nRegistRtn;
	int nDBRtn = 0;
	std::wstring strDBErrMsg;

	nMessageRtn = MessageBox(_T("�o�^�����s���܂���"),_T("�o�^���s"), MB_YESNO);
	
	if (nMessageRtn == IDNO)
	{
		/* �L�����Z�� */
		return;
	}

	/* ��ʏ��X�V */
	UpdateData();

	/* �o�^�������s */
	nRegistRtn = SetRegistData(cProjectData);

	if (nRegistRtn != 0)
	{
		/* ���ُ͈펞*/

	}

	/* ���t�^�����v���C�X */
	cProjectData.DateReplace();

	/* DB�o�^ */
	nDBRtn = clDBA.DBSetNewProject(cProjectData);

	/* DB�ُ펞 */
	if (nDBRtn == DB_NG)
	{
		strDBErrMsg = clDBA.OutErrorMsg();
		MessageBox(strDBErrMsg.c_str());
	}

	MessageBox(_T("�o�^���������܂���"));
}

/**
* @fn
* �o�^���̐ݒ菈��
* @brief �o�^���̐ݒ菈��
* @return �Ȃ�
* @detail �o�^���̐ݒ菈��
*/
int CAnkenRegistWindow::SetRegistData(CProjectData& cProjectData)
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::wstring strTemp;
	CString strGetText;
	CTime cGetTime;
	int nCursel = 0;

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
	cProjectData.vSkillData = m_vSkillData;

	/* �T�v */
	m_OrverEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strOrverView, strTemp);

	/* ���l */
	m_OtherEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(cProjectData.strOther, strTemp);

	/* ���̓`�F�b�N */
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
void CAnkenRegistWindow::InitCmpCombo()
{
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::vector<CCmpData>::iterator it;
	std::wstring strwSetUser;
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
	}

	m_CmpCombo.SetCurSel(0);
}

/**
* @fn
* �X�L���ݒ�{�^���N���b�N����
* @brief �X�L���ݒ�{�^���N���b�N����
* @return �Ȃ�
* @detail �X�L���ݒ�{�^���N���b�N����
*/
void CAnkenRegistWindow::OnBnClickedButton4()
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

	/* �X�L�����̎Q�� */
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
* �N���A�{�^���N���b�N����
* @brief �N���A�{�^���N���b�N����
* @return �Ȃ�
* @detail �N���A�{�^���N���b�N����
*/
void CAnkenRegistWindow::OnBnClickedButton1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
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
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnOK();
}


void CAnkenRegistWindow::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnCancel();
}
