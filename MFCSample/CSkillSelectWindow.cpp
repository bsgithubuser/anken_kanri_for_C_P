// CSkillSelectWindow.cpp : implementation file
//
/**
* @file CSkillSelectWindow.cpp
* @brief �X�L�����I�����
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
* �X�L�����ݒ菈��
* @brief �X�L�����ݒ菈��
* @return �Ȃ�
* @detail �X�L�����ݒ菈��
*/
void CSkillSelectWindow::SetSkillData(std::vector<CSkillData>* pvSkillData)
{
	m_pvSkillData = pvSkillData;
}


/**
* @fn
* �X�L�����I����ʂ̏�����
* @brief �X�L�����I����ʂ̏�����
* @return �Ȃ�
* @detail �X�L�����I����ʂ̏�����
*/
BOOL CSkillSelectWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	//�X�L�����X�g�̏�����
	InitSkillSelectList();

	// �X�L�����̂�ݒ�
	SetSkillList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/**
* @fn
* �X�L�����X�g�̏�����
* @brief �X�L�����X�g�̏�����
* @return �Ȃ�
* @detail �X�L�����X�g�̏�����
*/
void CSkillSelectWindow::InitSkillSelectList()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("�X�L������") };
	const int   clmNum = sizeof caption / sizeof caption[0];

	m_SkillSelectList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // �T�u�A�C�e���ԍ�
		lvc.pszText = caption[i];   // ���o���e�L�X�g
		lvc.cx = 250;				 // ����
		if (m_SkillSelectList.InsertColumn(i, &lvc) == -1) { break; }
	}
}

/**
* @fn
* �X�L�����X�g�ݒ菈��
* @brief �X�L�����X�g�ݒ菈��
* @return �Ȃ�
* @detail �X�L�����X�g�ݒ菈��
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

	/* ���X�g�N���A */
	m_SkillSelectList.DeleteAllItems();

	for (std::vector<CSkillData>::iterator it = vSkillData.begin();
		it != vSkillData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		/* �X�L������ */
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
* �X�L���ݒ�{�^���N���b�N����
* @brief �X�L���ݒ�{�^���N���b�N����
* @return �Ȃ�
* @detail �X�L���ݒ�{�^���N���b�N����
*/
void CSkillSelectWindow::OnBnClickedButton1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int iSelected = -1;
	CSkillData *cSkillData;

	// �I�𒆂̃X�L���𒊏o
	do {
		iSelected = m_SkillSelectList.GetNextItem(iSelected, LVNI_SELECTED);

		//�I�𒆂̂��̂�true��
		if (iSelected == -1)break;
		cSkillData = &(m_pvSkillData->at(iSelected));
		cSkillData->bSetSkill = true;

	} while (iSelected != -1);

	this->EndDialog(200);
}
