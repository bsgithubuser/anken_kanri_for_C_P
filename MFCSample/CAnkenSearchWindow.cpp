// CAnkenSearchWindow.cpp : implementation file
//
/**
* @file CAnkenSearchWindow.cpp
* @brief �Č��ڍ׌������
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
* �������ݒ菈��
* @brief �������ݒ菈��
* @return �Ȃ�
* @detail �������ݒ菈��
*/
void CAnkenSearchWindow::SetSearchData(CProjectSearchData* cProjectSearch)
{
	m_pProjectSearchData = cProjectSearch;
}

/**
* @fn
* �Č��ڍ׌�����ʂ̏�����
* @brief �Č��ڍ׌�����ʂ̏�����
* @return �Ȃ�
* @detail �Č��ڍ׌�����ʂ̏�����
*/
BOOL CAnkenSearchWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	m_ExtentionRadio = 1;

	/* �܂����쐬�̂��ߔ񊈐� */
	m_FromDayEdit.EnableWindow(false);
	m_ToDayEdit.EnableWindow(false);
	m_ValidDay = false;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/**
* @fn
* �X�L���ݒ�{�^���N���b�N����
* @brief �X�L���ݒ�{�^���N���b�N����
* @return �Ȃ�
* @detail �X�L���ݒ�{�^���N���b�N����
*/
void CAnkenSearchWindow::OnBnClickedButton4()
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
* �ڍ׌����{�^���N���b�N����
* @brief �ڍ׌����{�^���N���b�N����
* @return �Ȃ�
* @detail �ڍ׌����{�^���N���b�N����
*/
void CAnkenSearchWindow::OnBnClickedButton1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDBAccess &clDBA = CDBAccess::getInstance();
	std::wstring strTemp;
	CString strGetText;
	CTime cGetTime;

	// �ڍ׌������w��
	m_pProjectSearchData->bDetailSearch = true;

	// ��������X�L������ݒ�
	m_pProjectSearchData->vSkillData = m_vSkillData;

	// ��ʍX�V
	UpdateData();

	/* ���� */
	m_FromDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(m_pProjectSearchData->strPeriFrom, strTemp);

	m_ToDayEdit.GetWindowTextW(strGetText);
	strTemp = strGetText.GetBuffer();
	clDBA.StrConv(m_pProjectSearchData->strPeriTo, strTemp);

	/* ���� */
	if (m_ExtentionRadio == 0)
	{
		m_pProjectSearchData->strExtention = "t";
	}
	else
	{
		m_pProjectSearchData->strExtention = "f";
	}

	/* ���� */
	if (m_LongTermChk == TRUE)
	{
		m_pProjectSearchData->strLongTerm = "t";
	}
	else
	{
		m_pProjectSearchData->strLongTerm = "f";
	}

	/* ���� */
	if (m_SameDayChk == TRUE)
	{
		m_pProjectSearchData->strSameDay = "t";
	}
	else
	{
		m_pProjectSearchData->strSameDay = "f";
	}

	/* ���� */
	if (m_AnyTimeChk == TRUE)
	{
		m_pProjectSearchData->strAnyTime = "t";
	}
	else
	{
		m_pProjectSearchData->strAnyTime = "f";
	}

	// ��ʂ����
	this->EndDialog(203);
}

/**
* @fn
* ���Ԏw��`�F�b�N����
* @brief ���Ԏw��`�F�b�N����
* @return �Ȃ�
* @detail ���Ԏw��`�F�b�N����
*/
void CAnkenSearchWindow::OnBnClickedCheck4()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
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
