// CMainWindow.cpp : implementation file
//
/**
* @file CMainWindow.cpp
* @brief �Č��Ǘ����C�����
* @date 2016/04/25
*/

#include "stdafx.h"
#include "MFCSample.h"
#include "CMainWindow.h"
#include "afxdialogex.h"
#include "CDBAccess.h"

//#include "CLOGIN.h"

// CMainWindow dialog

IMPLEMENT_DYNAMIC(CMainWindow, CDialogEx)

CMainWindow::CMainWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINWINDOW, pParent)
{
	//Create(IDD_MAINWINDOW, pParent);
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtl);
	DDX_Control(pDX, IDC_BUTTON4, m_AnkenListBtn);
	DDX_Control(pDX, IDC_BUTTON1, m_UserBtn);
	DDX_Control(pDX, IDC_BUTTON2, m_SkillBtn);
	DDX_Control(pDX, IDC_BUTTON3, m_CmpBtn);
	DDX_Control(pDX, IDC_BUTTON6, m_AnkenReadBtn);
}


BEGIN_MESSAGE_MAP(CMainWindow, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMainWindow::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainWindow::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainWindow::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainWindow::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMainWindow::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMainWindow::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMainWindow message handlers

/**
* @fn
* �N���[�Y�{�^���N���b�N����
* @brief �N���[�Y�{�^���N���b�N����
* @return �Ȃ�
* @detail �N���[�Y�{�^���N���b�N����
*/
void CMainWindow::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	//CWnd* cParentWindow = this->GetParent();
	//((CLOGIN*)cParentWindow)->OnClose();

	CDialogEx::OnClose();
}

/**
* @fn
* �S���ҊǗ��{�^���N���b�N����
* @brief �S���ҊǗ��{�^���N���b�N����
* @return �Ȃ�
* @detail �S���ҊǗ��{�^���N���b�N����
*/
void CMainWindow::OnBnClickedButton1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	HideChildWindow();
	m_cUserWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* �Č��Ǘ����C����ʂ̏�����
* @brief �Č��Ǘ����C����ʂ̏�����
* @return �Ȃ�
* @detail �Č��Ǘ����C����ʂ̏�����
*/
BOOL CMainWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	// �q��ʍ쐬
	CRect SetRect;
	m_TabCtl.GetWindowRect(SetRect);
	this->ScreenToClient(SetRect);
	SetRect.OffsetRect(0, -25);
	SetRect.bottom = SetRect.bottom + 25;

	/* ��ʐݒ� */
	m_cUserWindow.Create(IDD_USERWINDOW, this);
	m_cUserWindow.MoveWindow(SetRect, 0);
	m_cUserWindow.ShowWindow(SW_HIDE);

	m_cSkillWindow.Create(IDD_SKILLWINDOW, this);
	m_cSkillWindow.MoveWindow(SetRect, 0);
	m_cSkillWindow.ShowWindow(SW_HIDE);

	m_cCmpWindow.Create(IDD_CMPNWINDOW, this);
	m_cCmpWindow.MoveWindow(SetRect, 0);
	m_cCmpWindow.ShowWindow(SW_HIDE);

	m_cAnkenListWindow.Create(IDD_ANKENLIST, this);
	m_cAnkenListWindow.MoveWindow(SetRect, 0);
	m_cAnkenListWindow.ShowWindow(SW_SHOW);

	m_cAnkenRegistWindow.Create(IDD_ANKENREGISTWINDOW, this);
	m_cAnkenRegistWindow.MoveWindow(SetRect, 0);
	m_cAnkenRegistWindow.ShowWindow(SW_HIDE);

	m_cAnkenReadWindow.Create(IDD_ANKENREADWINDOW, this);
	m_cAnkenReadWindow.MoveWindow(SetRect, 0);
	m_cAnkenReadWindow.ShowWindow(SW_HIDE);

	CDBAccess &cDBA = CDBAccess::getInstance();

	/* ���O�C���͊Ǘ��҈ȊO�� */
	if (cDBA.m_strAdmin != "0")
	{
		m_UserBtn.EnableWindow(FALSE);
		m_SkillBtn.EnableWindow(FALSE);
		m_CmpBtn.EnableWindow(FALSE);
	}

	/* ���쐬�̂��ߔ񊈐� */
	m_AnkenReadBtn.EnableWindow(FALSE);

	m_AnkenListBtn.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/**
* @fn
* �q��ʔ�\������
* @brief �q��ʔ�\������
* @return �Ȃ�
* @detail �q��ʔ�\������
*/
void CMainWindow::HideChildWindow()
{
	m_cUserWindow.ShowWindow(SW_HIDE);
	m_cSkillWindow.ShowWindow(SW_HIDE);
	m_cCmpWindow.ShowWindow(SW_HIDE);
	m_cAnkenListWindow.ShowWindow(SW_HIDE);
	m_cAnkenRegistWindow.ShowWindow(SW_HIDE);
	m_cAnkenReadWindow.ShowWindow(SW_HIDE);
}

/**
* @fn
* �X�L���Ǘ��{�^���N���b�N����
* @brief �X�L���Ǘ��{�^���N���b�N����
* @return �Ȃ�
* @detail �X�L���Ǘ��{�^���N���b�N����
*/
void CMainWindow::OnBnClickedButton2()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	HideChildWindow();
	m_cSkillWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* ��Џ��Ǘ��{�^���N���b�N����
* @brief ��Џ��Ǘ��{�^���N���b�N����
* @return �Ȃ�
* @detail ��Џ��Ǘ��{�^���N���b�N����
*/
void CMainWindow::OnBnClickedButton3()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	HideChildWindow();
	m_cCmpWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* �Č����ꗗ�{�^���N���b�N����
* @brief �Č����ꗗ�{�^���N���b�N����
* @return �Ȃ�
* @detail �Č����ꗗ�{�^���N���b�N����
*/
void CMainWindow::OnBnClickedButton4()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	HideChildWindow();
	m_cAnkenListWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* �Č����o�^�{�^���N���b�N����
* @brief �Č����o�^�{�^���N���b�N����
* @return �Ȃ�
* @detail �Č����o�^�{�^���N���b�N����
*/
void CMainWindow::OnBnClickedButton5()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	HideChildWindow();
	m_cAnkenRegistWindow.ShowWindow(SW_SHOW);
}

/**
* @fn
* �Č����捞�{�^���N���b�N����
* @brief �Č����捞�{�^���N���b�N����
* @return �Ȃ�
* @detail �Č����捞�{�^���N���b�N����
*/
void CMainWindow::OnBnClickedButton6()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	HideChildWindow();
	m_cAnkenReadWindow.ShowWindow(SW_SHOW);
}


void CMainWindow::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	//CDialogEx::OnOK();
}


void CMainWindow::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	CDialogEx::OnCancel();
}
