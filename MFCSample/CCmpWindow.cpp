// CCompanyWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CCmpWindow.h"
#include "afxdialogex.h"
#include "CCompanyData.h"
#include "CDBAccess.h"

#include <vector>

// CCompanyWindow dialog

IMPLEMENT_DYNAMIC(CCompanyWindow, CDialogEx)

CCompanyWindow::CCompanyWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CMPNWINDOW, pParent)
{

}

CCompanyWindow::~CCompanyWindow()
{
}

void CCompanyWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMPN_MASTA, m_MastaTitle);
	DDX_Control(pDX, IDC_LIST2, m_ListCtl);
}


BEGIN_MESSAGE_MAP(CCompanyWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCompanyWindow::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CCompanyWindow::OnNMRClickList2)
	ON_BN_CLICKED(IDC_BTN_DATAREG, &CCompanyWindow::OnBnClickedBtnDatareg)
	ON_BN_CLICKED(IDC_BTN_DATAEDIT, &CCompanyWindow::OnBnClickedBtnDataedit)
	ON_BN_CLICKED(IDC_BTN_DATADEL, &CCompanyWindow::OnBnClickedBtnDatadel)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &CCompanyWindow::OnBnClickedBtnRedraw)
END_MESSAGE_MAP()

// CCompanyWindow message handlers


BOOL CCompanyWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������
	CFont SetFont;
	SetFont.CreatePointFont(20, _T("�l�r �o�S�V�b�N"));
	m_MastaTitle.SetFont(&SetFont);

	/* ��Ѓ��X�g�̏����ݒ�*/
	InitCompanyList();

	/* ��Џ��̎Q�� */
	CompanyListUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CCompanyWindow::InitCompanyList()
{
	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("���ID"), _T("��Ж�"), _T("�t���K�i") };
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

void CCompanyWindow::CompanyListUpdate()
{
	CCompanyData tempCompanyData;
	LVITEM lvi;
	int nIndex = 0;
	CString strTemp;
	std::wstring strwTemp;

	m_vCompanyData.clear();

	CDBAccess &clDBA = CDBAccess::getInstance();

	/* �X�L�����̎Q��*/
	clDBA.DBGetCompanyData(m_vCompanyData);

	for (std::vector<CCompanyData>::iterator it = m_vCompanyData.begin();
		it != m_vCompanyData.end();
		it++)
	{
		lvi.mask = LVIF_TEXT;

		lvi.iItem = nIndex;
		lvi.iSubItem = 0;
		strTemp.Format(_T("%d"), it->nCompanyID);
		lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp));
		m_ListCtl.InsertItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 1;
		strwTemp = clDBA.ToWideStr(it->strCompanyName);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);

		lvi.iItem = nIndex;
		lvi.iSubItem = 2;
		strwTemp = clDBA.ToWideStr(it->strCompanyName_Furi);
		lvi.pszText = const_cast<LPTSTR>(strwTemp.c_str());
		m_ListCtl.SetItem(&lvi);

	}

}


void CCompanyWindow::OnBnClickedButton1()
{
/*
	// �q��ʍ쐬
	CRect SetRect;
	//	m_TabCtl.GetWindowRect(SetRect);
	this->ScreenToClient(SetRect);
	SetRect.OffsetRect(0, -25);
	SetRect.bottom = SetRect.bottom + 25;

	this->ShowWindow(SW_HIDE);
	m_cCompanyRegistWindow.Create(IDD_CompanyREGISTWINDOW, this);
	m_cCompanyRegistWindow.MoveWindow(SetRect, 0);
	m_cCompanyRegistWindow.ShowWindow(SW_SHOW);
*/

}

void CCompanyWindow::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
/*
	CMenu    *popupMenuP = NULL;
	CMenu    cMenu;
	CPoint	 posMouse;
	int      err = 0;

	// ���j���[�����[�h
	if (!err) if (!cMenu.LoadMenu(IDR_RMENU_Company)) err = 1;
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
*/

	*pResult = 0;
}


//***********************************************************//
// �V�K�o�^�{�^����������
//***********************************************************//
void CCompanyWindow::OnBnClickedBtnDatareg()
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
	nRet = clDBA.DBGetSeqquence("cmpn_id_seq", nSequence);
	strSequence = nSequence;

	strTemp.Format(_T("%d"), nSequence);
	//	lpcSequence = static_cast<LPCTSTR>(strSequence.c_str());

	// �o�^��ʂ𐶐�
	m_cCmpnRegistWindow.Create(IDD_CMPN_REG_WINDOW, this);
	//	m_cSkillRegistWindow.cSkillIdEdit.SetWindowTextW(const_cast<LPTSTR>(static_cast<LPCTSTR>(strTemp)));

	// �X�L��ID��ݒ�
	m_cCmpnRegistWindow.cCmpnIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cCmpnRegistWindow.bUpdFlg = FALSE;

	// �o�^��ʂ�\��
	m_cCmpnRegistWindow.ShowWindow(SW_SHOW);


}


//***********************************************************//
// �ҏW�{�^����������
//***********************************************************//
void CCompanyWindow::OnBnClickedBtnDataedit()
{
	int iSelected;
	CString strTemp;


	// ��Јꗗ�̑I���s���擾
	iSelected = m_ListCtl.GetNextItem(-1, LVNI_SELECTED);

	// ���I�����͉������Ȃ�
	if (iSelected < 0)
	{
		return;
	}

	// ��Гo�^��ʂ̐���
	m_cCmpnRegistWindow.Create(IDD_CMPN_REG_WINDOW, this);

	// ��Јꗗ����ID���擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	m_cCmpnRegistWindow.cCmpnIdEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// ��Јꗗ�����Ж����擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 1);
	m_cCmpnRegistWindow.cCmpnNameEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	// ��Јꗗ�����Ж�(�t���K�i)���擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 2);
	m_cCmpnRegistWindow.cCmpnFuriEdit.SetWindowTextW(static_cast<LPCTSTR>(strTemp));

	m_cCmpnRegistWindow.bUpdFlg = TRUE;

	// �X�L���o�^��ʂ̕\��
	m_cCmpnRegistWindow.ShowWindow(SW_SHOW);

}


//***********************************************************//
// �폜�{�^����������
//***********************************************************//
void CCompanyWindow::OnBnClickedBtnDatadel()
{
	int nRet;
	int iSelected;
	CString strTemp;
	CDBAccess &clDBA = CDBAccess::getInstance();

	// ��Јꗗ�̑I���s���擾
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

	// ��Јꗗ����ID���擾
	strTemp = "";
	strTemp = m_ListCtl.GetItemText(iSelected, 0);

	nRet = clDBA.DBSetCmpnData(strTemp, NULL, NULL, 2);
}



//***********************************************************//
// �ĕ\���{�^����������
//***********************************************************//
void CCompanyWindow::OnBnClickedBtnRedraw()
{
	m_ListCtl.DeleteAllItems();

	CompanyListUpdate();
}


void CCompanyWindow::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnOK();
}


void CCompanyWindow::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnCancel();
}
