// CUserRegistWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CUserWindow.h"
#include "CUserRegistWindow.h"
#include "CDBAccess.h"
#include "afxdialogex.h"


// CUserRegistWindow dialog

IMPLEMENT_DYNAMIC(CUserRegistWindow, CDialogEx)

CUserRegistWindow::CUserRegistWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USER_REG_WINDOW, pParent)
	, aaaa(_T(""))
{

}

CUserRegistWindow::~CUserRegistWindow()
{
}

BOOL CUserRegistWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//	m_combo1 = (CComboBox*)GetDlgItem(IDC_COMBO1);  // �W��

	// �R���{�{�b�N�X�̏����l
	cAdminEdit.InsertString(0, _T("0:�Ǘ���"));
	cAdminEdit.InsertString(1, _T("1:���"));
	cAdminEdit.InsertString(2, _T("9:�J����"));

	cAdminEdit.SetTopIndex(0);

	cError.SetWindowTextW(_T(""));


	return(TRUE);

}

void CUserRegistWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERID_EDIT, cUserIdEdit);
	DDX_Control(pDX, IDC_USERNAME_EDIT, cUserNameEdit);
	DDX_Control(pDX, IDC_PASS_EDIT, cPasswordEdit);
	DDX_Control(pDX, IDC_PASSCHK_EDIT, cPasswordEdit2);
	DDX_Control(pDX, IDC_COMBO1, cAdminEdit);
	DDX_Control(pDX, IDC_ERROR, cError);
}


BEGIN_MESSAGE_MAP(CUserRegistWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BACK, &CUserRegistWindow::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_CLR, &CUserRegistWindow::OnBnClickedBtnClr)
	ON_BN_CLICKED(IDC_BTN_REG, &CUserRegistWindow::OnBnClickedBtnReg)
END_MESSAGE_MAP()


// CUserRegistWindow message handlers


//***********************************************************//
// �߂�{�^����������
//***********************************************************//
void CUserRegistWindow::OnBnClickedBtnBack()
{
	this->EndDialog(100);
	this->DestroyWindow();

	//	m_cUserWindow.UserListUpdate();

}


//***********************************************************//
// �N���A�{�^����������
//***********************************************************//
void CUserRegistWindow::OnBnClickedBtnClr()
{

	// ��ʃN���A
	cUserNameEdit.SetWindowTextW(_T(""));
	cPasswordEdit.SetWindowTextW(_T(""));
	cPasswordEdit2.SetWindowTextW(_T(""));
	cAdminEdit.SetTopIndex(1);
	cError.SetWindowTextW(_T(""));


}


//***********************************************************//
// �o�^�{�^����������
//***********************************************************//
void CUserRegistWindow::OnBnClickedBtnReg()
{
	int nRet;
	int nIndex;

	CDBAccess &clDBA = CDBAccess::getInstance();


	//	std::string strUserId;
	//	std::string strUserName;
	CString  strUserId;
	CString  strUserName;
	CString  strUserPass;
	CString  strUserPassChk;
	CString  strUserAdmin;

	cUserIdEdit.GetWindowTextW(strUserId);
	cUserNameEdit.GetWindowTextW(strUserName);
	cPasswordEdit.GetWindowTextW(strUserPass);
	cPasswordEdit2.GetWindowTextW(strUserPassChk);

	cAdminEdit.GetWindowTextW(strUserAdmin);

	// �S���Җ����̓`�F�b�N
	if (strUserName.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("�S���Җ��͕K�{���͂ł��B"));
		return;

	}
	// �p�X���[�h���̓`�F�b�N
	if (strUserPass.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("�p�X���[�h�͕K�{���͂ł��B"));
		return;

	}
	// �m�F�p�X���[�h���̓`�F�b�N
	if (strUserPassChk.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("�p�X���[�h(�m�F)�͕K�{���͂ł��B"));
		return;

	}
	// �p�X���[�h����`�F�b�N
	if (strUserPass != strUserPassChk)
	{
		cError.SetWindowTextW(_T("�p�X���[�h���m�F�p�ƈ�v���܂���B"));
		return;
	}

	//	// �Ǘ��Ґݒ�m�F
	nIndex = cAdminEdit.GetCurSel();
	switch (nIndex)
	{
	case 0:
		strUserAdmin = _T("0");
		break;
	case 1:
		strUserAdmin = _T("1");
		break;
	case 2:
		strUserAdmin = _T("9");
		break;
	default:
		cError.SetWindowTextW(_T("�Ǘ��҂͕K�{���͂ł��B"));
		return;
	}

	// �m�F���b�Z�[�W
	nRet = MessageBoxW(_T("���͂������e�ɂă}�X�^�o�^���s���܂��B��낵���ł����H"), _T("�m�F"), MB_OKCANCEL + MB_DEFBUTTON2);
	if (nRet == IDCANCEL)
	{
		return;
	}



	nRet = clDBA.DBSetUserData(strUserId, strUserName, strUserPass, strUserAdmin, nUpdFlg);

	this->EndDialog(100);
	this->DestroyWindow();

}


void CUserRegistWindow::OnEnMaxtextPassEdit()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}
