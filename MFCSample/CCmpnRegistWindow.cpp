// CCmpnRegistWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CCmpnRegistWindow.h"
#include "CDBAccess.h"
#include "afxdialogex.h"


// CCmpnRegistWindow dialog

IMPLEMENT_DYNAMIC(CCmpnRegistWindow, CDialogEx)

CCmpnRegistWindow::CCmpnRegistWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CMPN_REG_WINDOW, pParent)
{

}

CCmpnRegistWindow::~CCmpnRegistWindow()
{
}

void CCmpnRegistWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMPNID_EDIT, cCmpnIdEdit);
	DDX_Control(pDX, IDC_CMPNNAME_EDIT, cCmpnNameEdit);
	DDX_Control(pDX, IDC_CMPNKANA_EDIT, cCmpnFuriEdit);
	DDX_Control(pDX, IDC_ERROR, cError);
}


BEGIN_MESSAGE_MAP(CCmpnRegistWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BACK, &CCmpnRegistWindow::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_CLR, &CCmpnRegistWindow::OnBnClickedBtnClr)
	ON_BN_CLICKED(IDC_BTN_REG, &CCmpnRegistWindow::OnBnClickedBtnReg)
END_MESSAGE_MAP()


BOOL CCmpnRegistWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	cError.SetWindowTextW(_T(""));
//	cError.SetTextColor(RGB(0, 0, 255));
	SetTextColor((HDC)cError.GetWindowDC(), RGB(255, 0, 0));


	return(TRUE);

}


//***********************************************************//
// �߂�{�^����������
//***********************************************************//
void CCmpnRegistWindow::OnBnClickedBtnBack()
{
	this->EndDialog(100);
	this->DestroyWindow();

}


//***********************************************************//
// �N���A�{�^����������
//***********************************************************//
void CCmpnRegistWindow::OnBnClickedBtnClr()
{
	// ��ʃN���A
	cCmpnNameEdit.SetWindowTextW(_T(""));
	cCmpnFuriEdit.SetWindowTextW(_T(""));

}


//***********************************************************//
// �o�^�{�^����������
//***********************************************************//
void CCmpnRegistWindow::OnBnClickedBtnReg()
{
	int nRet;
	int nCmpnID;

	CDBAccess &clDBA = CDBAccess::getInstance();

	
//	std::string strCmpnId;
//	std::string strCmpnName;
	CString  strCmpnId;
	CString  strCmpnName;
	CString  strCmpnFuri;

	cCmpnIdEdit.GetWindowTextW(strCmpnId);
	cCmpnNameEdit.GetWindowTextW(strCmpnName);
	cCmpnFuriEdit.GetWindowTextW(strCmpnFuri);

	// ��Ж����̓`�F�b�N
	if (strCmpnName.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("��Ж��͕K�{���͂ł��B"));
		return;

	}
	// �t���K�i���̓`�F�b�N
	if (strCmpnFuri.IsEmpty() == TRUE)
	{
		cError.SetWindowTextW(_T("�t���K�i�͕K�{���͂ł��B"));
		return;

	}

	// �m�F���b�Z�[�W
	nRet = MessageBoxW(_T("���͂������e�ɂă}�X�^�o�^���s���܂��B��낵���ł����H"), _T("�m�F"), MB_OKCANCEL + MB_DEFBUTTON2);
	if (nRet == IDCANCEL)
	{
		return;
	}

	nRet = clDBA.DBSetCmpnData(strCmpnId, strCmpnName, strCmpnFuri, bUpdFlg);

	this->EndDialog(100);
	this->DestroyWindow();

}
