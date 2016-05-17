// CSkillRegistWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CSkillRegistWindow.h"
#include "CDBAccess.h"
#include "afxdialogex.h"


// CSkillRegistWindow dialog

IMPLEMENT_DYNAMIC(CSkillRegistWindow, CDialogEx)

CSkillRegistWindow::CSkillRegistWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SKILL_REG_WINDOW, pParent)
{
//	cSkillIdEdite.SetWindowTextW(strUserName);
//	cSkillNamEdit.SetWindowTextW(strPassword);

}

CSkillRegistWindow::~CSkillRegistWindow()
{
}

void CSkillRegistWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKILLID_EDIT, cSkillIdEdit);
	DDX_Control(pDX, IDC_SKILLNAME_EDIT, cSkillNameEdit);
	DDX_Control(pDX, IDC_ERROR, cError);
}


BEGIN_MESSAGE_MAP(CSkillRegistWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BACK, &CSkillRegistWindow::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_CLR, &CSkillRegistWindow::OnBnClickedBtnClr)
	ON_BN_CLICKED(IDC_BTN_REG, &CSkillRegistWindow::OnBnClickedBtnReg)
END_MESSAGE_MAP()


BOOL CSkillRegistWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	cError.SetWindowTextW(_T(""));

	return(TRUE);

}


//***********************************************************//
// �߂�{�^����������
//***********************************************************//
void CSkillRegistWindow::OnBnClickedBtnBack()
{
	this->EndDialog(100);
	this->DestroyWindow();

}


//***********************************************************//
// �N���A�{�^����������
//***********************************************************//
void CSkillRegistWindow::OnBnClickedBtnClr()
{
	// ��ʃN���A
	cSkillNameEdit.SetWindowTextW(_T(""));

}


//***********************************************************//
// �o�^�{�^����������
//***********************************************************//
void CSkillRegistWindow::OnBnClickedBtnReg()
{
	int nRet;
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int nSkillID;

	CDBAccess &clDBA = CDBAccess::getInstance();

	
//	std::string strSkillId;
//	std::string strSkillName;
	CString  strSkillId;
	CString  strSkillName;

	cSkillIdEdit.GetWindowTextW(strSkillId);
	cSkillNameEdit.GetWindowTextW(strSkillName);

	// �X�L�������̓`�F�b�N
	if (strSkillName.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("�X�L�����͕K�{���͂ł��B"));
		return;

	}

	// �m�F���b�Z�[�W
	nRet = MessageBoxW(_T("���͂������e�ɂă}�X�^�o�^���s���܂��B��낵���ł����H"), _T("�m�F"), MB_OKCANCEL + MB_DEFBUTTON2);
	if (nRet == IDCANCEL)
	{
		return;
	}

	nRet = clDBA.DBSetSkillData(strSkillId, strSkillName, bUpdFlg);

	this->EndDialog(100);
	this->DestroyWindow();

}
