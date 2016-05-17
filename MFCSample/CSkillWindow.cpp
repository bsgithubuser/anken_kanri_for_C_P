// CSkillWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CSkillWindow.h"
#include "CSkillRegistWindow.h"
#include "afxdialogex.h"
#include "CSkillData.h"
#include "CDBAccess.h"

#include <vector>

// CSkillWindow dialog

IMPLEMENT_DYNAMIC(CSkillWindow, CDialogEx)

CSkillWindow::CSkillWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SKILLWINDOW, pParent)
{

}

CSkillWindow::~CSkillWindow()
{
}

void CSkillWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKILL_MASTA, m_MastaTitle);
	DDX_Control(pDX, IDC_LIST2, m_ListCtl);
}


BEGIN_MESSAGE_MAP(CSkillWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSkillWindow::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CSkillWindow::OnNMRClickList2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CSkillWindow::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BTN_DATAREG, &CSkillWindow::OnBnClickedBtnDatareg)
	ON_BN_CLICKED(IDC_BTN_DATAEDIT, &CSkillWindow::OnBnClickedBtnDataedit)
	ON_BN_CLICKED(IDC_BTN_DATADEL, &CSkillWindow::OnBnClickedBtnDatadel)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &CSkillWindow::OnBnClickedBtnRedraw)
END_MESSAGE_MAP()

// CSkillWindow message handlers


BOOL CSkillWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("�l�r �o�S�V�b�N"));
	m_MastaTitle.SetFont(&SetFont);

	/* �X�L�����X�g�̏����ݒ�*/
	InitSkillList();

	/* �X�L�����̎Q�� */
	SkillListUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


//***********************************************************//
// �X�L���ꗗ�̏�������
//***********************************************************//
void CSkillWindow::InitSkillList()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("�X�L��ID"), _T("�X�L����") };
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;

	m_ListCtl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // �T�u�A�C�e���ԍ�
		lvc.pszText = caption[i];   // ���o���e�L�X�g
		lvc.cx = 100;				 // ����
		if (m_ListCtl.InsertColumn(i, &lvc) == -1) { err = 1; break; }
	}

}

//***********************************************************//
// �X�L���ꗗ�̍X�V����
//***********************************************************//
void CSkillWindow::SkillListUpdate()
{
	CSkillData tempSkillData;
	LVITEM lvi;
	int nIndex = 0;
	CString strTemp;
	std::wstring strwTemp;

	m_vSkillData.clear();

	CDBAccess &clDBA = CDBAccess::getInstance();

	// �X�L���Ǘ����̎擾
	clDBA.DBGetSkillData(m_vSkillData);

	for (std::vector<CSkillData>::iterator it = m_vSkillData.begin();
		it != m_vSkillData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		// �X�L��ID�̃Z�b�g
		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		strTemp.Format(_T("%d"), it->nSkillID);
		lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp));
		m_ListCtl.InsertItem(&lvi);

		// �X�L�����̃Z�b�g
		lvi.iItem = nIndex;
		lvi.iSubItem = 1;
		strwTemp = clDBA.ToWideStr(it->strSkillName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);


	}

}


void CSkillWindow::OnBnClickedButton1()
{
	// �q��ʍ쐬
	CRect SetRect;
//	m_TabCtl.GetWindowRect(SetRect);
	this->ScreenToClient(SetRect);
	SetRect.OffsetRect(0, -25);
	SetRect.bottom = SetRect.bottom + 25;

	this->ShowWindow(SW_HIDE);
	m_cSkillRegistWindow.Create(IDD_SKILLREGISTWINDOW, this);
	m_cSkillRegistWindow.MoveWindow(SetRect, 0);
	m_cSkillRegistWindow.ShowWindow(SW_SHOW);
}


void CSkillWindow::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CMenu    *popupMenuP = NULL;
	CMenu    cMenu;
	CPoint	 posMouse;
	int      err = 0;

	// ���j���[�����[�h
	if (!err) if (!cMenu.LoadMenu(IDR_RMENU_SKILL)) err = 1;
	// �T�u���j���[���擾
	if (!err) if ((popupMenuP = cMenu.GetSubMenu(0)) == NULL) err = 1;
	// ���j���[���|�b�v�A�b�v
	if (!err)
	{
		ClientToScreen(&posMouse);
		if (!popupMenuP->TrackPopupMenu(
			TPM_LEFTBUTTON, posMouse.x, posMouse.y, this)) err = 1;
	}
	// ���j���[��j��
	cMenu.DestroyMenu();

//	CDialog::OnRButtonUp(nFlags, point);

	*pResult = 0;
}


void CSkillWindow::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;
}


//***********************************************************//
// �V�K�o�^�{�^����������
//***********************************************************//
void CSkillWindow::OnBnClickedBtnDatareg()
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
	nRet = clDBA.DBGetSeqquence("skill_id_seq", nSequence);
	strSequence = nSequence;

	strTemp.Format(_T("%d"), nSequence);
	//	lpcSequence = static_cast<LPCTSTR>(strSequence.c_str());

	// �o�^��ʂ𐶐�
	m_cSkillRegistWindow.Create(IDD_SKILL_REG_WINDOW, this);
	//	m_cSkillRegistWindow.cSkillIdEdit.SetWindowTextW(const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp)));

	// �X�L��ID��ݒ�
	m_cSkillRegistWindow.cSkillIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cSkillRegistWindow.bUpdFlg = FALSE;

	// �o�^��ʂ�\��
	m_cSkillRegistWindow.ShowWindow(SW_SHOW);
	//	m_cSkillRegistWindow.DoModal();


}


//***********************************************************//
// �ҏW�{�^����������
//***********************************************************//
void CSkillWindow::OnBnClickedBtnDataedit()
{
	int iSelected;
	CString strTemp;

		
	// �X�L���ꗗ�̑I���s���擾
	iSelected = m_ListCtl.GetNextItem(-1, LVNI_SELECTED);

	// ���I�����͉������Ȃ�
	if (iSelected < 0)
	{
		return;
	}

	// �X�L���o�^��ʂ̐���
	m_cSkillRegistWindow.Create(IDD_SKILL_REG_WINDOW, this);

	// �X�L���ꗗ����ID���擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	// �󗓍s�I�����̓G���[
	// ��ŁE�E�E

	m_cSkillRegistWindow.cSkillIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// �X�L���ꗗ����X�L�������擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 1);
	m_cSkillRegistWindow.cSkillNameEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cSkillRegistWindow.bUpdFlg = TRUE;

	// �X�L���o�^��ʂ̕\��
	m_cSkillRegistWindow.ShowWindow(SW_SHOW);

}


//***********************************************************//
// �폜�{�^����������
//***********************************************************//
void CSkillWindow::OnBnClickedBtnDatadel()
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

	// �X�L���ꗗ����ID���擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	nRet = clDBA.DBSetSkillData(strTemp, NULL, 2);


}


//***********************************************************//
// �ĕ\���{�^����������
//***********************************************************//
void CSkillWindow::OnBnClickedBtnRedraw()
{
	m_ListCtl.DeleteAllItems();

	SkillListUpdate();
}


void CSkillWindow::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnOK();
}


void CSkillWindow::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnCancel();
}
