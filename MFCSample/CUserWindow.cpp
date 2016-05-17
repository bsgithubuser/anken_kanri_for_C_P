// CUserWindow.cpp : implementation file
//

#include "stdafx.h"
#include <atlbase.h>
#include <atlconv.h>
#include "MFCSample.h"
#include "CUserWindow.h"
#include "CUserRegistWindow.h"
#include "afxdialogex.h"
#include "CUserData.h"
#include "CDBAccess.h"

#include <vector>

// CUserWindow dialog

IMPLEMENT_DYNAMIC(CUserWindow, CDialogEx)

CUserWindow::CUserWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USERWINDOW, pParent)
{

}

CUserWindow::~CUserWindow()
{
}

void CUserWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_MASTA, m_MastaTitle);
	DDX_Control(pDX, IDC_LIST2, m_ListCtl);
}


BEGIN_MESSAGE_MAP(CUserWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DATAREG, &CUserWindow::OnBnClickedBtnDatareg)
	ON_BN_CLICKED(IDC_BTN_DATAEDIT, &CUserWindow::OnBnClickedBtnDataedit)
	ON_BN_CLICKED(IDC_BTN_DATADEL, &CUserWindow::OnBnClickedBtnDatadel)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &CUserWindow::OnBnClickedBtnRedraw)
END_MESSAGE_MAP()

// CUserWindow message handlers


BOOL CUserWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("�l�r �o�S�V�b�N"));
	m_MastaTitle.SetFont(&SetFont);

	/* �S���҃��X�g�̏����ݒ�*/
	InitUserList();

	/* �S���ҏ��̎Q�� */
	UserListUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CUserWindow::InitUserList()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("�S����ID"), _T("�S���Җ�"),
								_T("�p�X���[�h") , _T("�Ǘ��Ҍ���") };
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;

	m_ListCtl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // �T�u�A�C�e���ԍ�
		lvc.pszText = caption[i];   // ���o���e�L�X�g
		if (i == 2)
		{
			lvc.cx = 0;				 // ����
		}
		else
		{
			lvc.cx = 100;					 // ����
		}

		if (m_ListCtl.InsertColumn(i, &lvc) == -1) { err = 1; break; }
	}

}

void CUserWindow::UserListUpdate()
{
	CUserData tempUserData;
	LVITEM lvi;
	int nIndex = 0;
	CString strTemp;
	std::wstring strwTemp;

	m_vUserData.clear();

	CDBAccess &clDBA = CDBAccess::getInstance();

	/* �S���ҏ��̎Q��*/
	clDBA.DBGetUserList(m_vUserData);

	for (std::vector<CUserData>::iterator it = m_vUserData.begin();
		it != m_vUserData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		strTemp.Format(_T("%d"), it->nUserID);
		lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp));
		m_ListCtl.InsertItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 1;
		strwTemp = clDBA.ToWideStr(it->strUserName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 2;
		clDBA.WStrConv(strwTemp, it->strPassword);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 3;
		clDBA.WStrConv(strwTemp, it->strAdmin);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);
	}

}


//***********************************************************//
// �V�K�o�^�{�^����������
//***********************************************************//
void CUserWindow::OnBnClickedBtnDatareg()
{
	int nSequence;
	int nRet;
	CString strTemp;
	std::wstring strSequence;
	CDBAccess &clDBA = CDBAccess::getInstance();
	LPCTSTR lpcSequence;
	std::wstring strwTemp;

	// �X�L��ID���V�[�P���X����擾
	nSequence = 0;
	nRet = clDBA.DBGetSeqquence("users_id_seq", nSequence);
	strSequence = nSequence;

	strTemp.Format(_T("%d"), nSequence);
	//	lpcSequence = static_cast<LPCTSTR>(strSequence.c_str());

	// �o�^��ʂ𐶐�
	m_cUserRegistWindow.Create(IDD_USER_REG_WINDOW, this);

	// �X�L��ID��ݒ�
	m_cUserRegistWindow.cUserIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cUserRegistWindow.nUpdFlg = 0;

	// �o�^��ʂ�\��
	m_cUserRegistWindow.ShowWindow(SW_SHOW);
	//	m_cSkillRegistWindow.DoModal();

}


//***********************************************************//
// �ҏW�{�^����������
//***********************************************************//
void CUserWindow::OnBnClickedBtnDataedit()
{
	int iSelected;
	CString strTemp;


	// �S���҈ꗗ�̑I���s���擾
	iSelected = m_ListCtl.GetNextItem(-1, LVNI_SELECTED);

	// ���I�����͉������Ȃ�
	if (iSelected < 0)
	{
		return;
	}

	// �S���ғo�^��ʂ̐���
	m_cUserRegistWindow.Create(IDD_USER_REG_WINDOW, this);

	// �S���҈ꗗ����ID���擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	m_cUserRegistWindow.cUserIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// �S���҈ꗗ����X�L�������擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 1);
	m_cUserRegistWindow.cUserNameEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// �S���҈ꗗ����p�X���[�h���擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 2);
	m_cUserRegistWindow.cPasswordEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// �S���҈ꗗ����p�X���[�h���擾(�m�F�p�ɃZ�b�g)
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 2);
	m_cUserRegistWindow.cPasswordEdit2.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// �Ǘ��Ґݒ�m�F
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 3);
	if (strTemp.IsEmpty() == FALSE)
	{
		m_cUserRegistWindow.cAdminEdit.SetCurSel(StrToInt(strTemp));
	}

	m_cUserRegistWindow.nUpdFlg = 1;

	// �X�L���o�^��ʂ̕\��
	m_cUserRegistWindow.ShowWindow(SW_SHOW);
}


//***********************************************************//
// �폜�{�^����������
//***********************************************************//
void CUserWindow::OnBnClickedBtnDatadel()
{
	int nRet;
	int iSelected;
	CString strTemp;
	CDBAccess &clDBA = CDBAccess::getInstance();

	// �X�L���ꗗ�̑I���s���擾
	iSelected = m_ListCtl.GetNextItem(-1, LVNI_SELECTED);

	// ���I�����͉������Ȃ�
	if (iSelected <= 0)
	{
		return;
	}

	// �m�F���b�Z�[�W
	nRet = MessageBoxW(_T("�I�������s���폜���܂��B��낵���ł����H"), _T("�m�F"), MB_OKCANCEL + MB_DEFBUTTON2);
	if (nRet == IDCANCEL)
	{
		OnBnClickedBtnRedraw();
		return;
	}

	// �S���҈ꗗ����ID���擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	nRet = clDBA.DBSetUserData(strTemp, NULL, NULL, NULL, 2);


}


//***********************************************************//
// �ĕ\���{�^����������
//***********************************************************//
void CUserWindow::OnBnClickedBtnRedraw()
{
	m_ListCtl.DeleteAllItems();

	UserListUpdate();
}


void CUserWindow::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnOK();
}


void CUserWindow::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnCancel();
}
