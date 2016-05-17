// CLOGIN.cpp : implementation file
//
/**
* @file CLOGIN.cpp
* @brief ���O�C�����
* @date 2016/04/25
*/
#include "stdafx.h"
#include "MFCSample.h"
#include "CLOGIN.h"
#include "afxdialogex.h"
#include "CDBAccess.h"

// CLOGIN dialog

IMPLEMENT_DYNAMIC(CLOGIN, CDialogEx)

CLOGIN::CLOGIN(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

CLOGIN::~CLOGIN()
{
}

void CLOGIN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, cUserEdit);
	DDX_Control(pDX, IDC_EDIT2, cPasswordEdit);
	DDX_Control(pDX, IDC_BUTTON1, m_LonginBtn);
	DDX_Control(pDX, IDC_BUTTON2, m_ClearBtn);
}


BEGIN_MESSAGE_MAP(CLOGIN, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLOGIN::OnLogin)
	ON_BN_CLICKED(IDC_BUTTON2, &CLOGIN::OnClear)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLOGIN message handlers

/**
* @fn
* ���O�C����ʍ쐬����
* @brief ���O�C����ʍ쐬����
* @return �Ȃ�
* @detail ���O�C����ʍ쐬����
*/
BOOL CLOGIN::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}

/**
* @fn
* ���O�C���{�^���N���b�N����
* @brief ���O�C���{�^���N���b�N����
* @return �Ȃ�
* @detail ���O�C���{�^���N���b�N����
*/
void CLOGIN::OnLogin()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	std::wstring strwUserName;
	std::wstring strwPassword;
	std::string strDBUserName;
	std::string strDBPassword;
	CString strUserName;
	CString strPassword;
	long lChkFlg;
	std::wstring strErrorMsg;

	/* DB�A�N�Z�X�C���X�^���X���Q�� */
	CDBAccess &cDBA = CDBAccess::getInstance();

	/* ������Q�� */
	cUserEdit.GetWindowTextW(strUserName);
	cPasswordEdit.GetWindowTextW(strPassword);
	strwUserName = strUserName;
	strwPassword = strPassword;

	/* �����ϊ� */
	cDBA.StrConv(strDBUserName, strwUserName);
	cDBA.StrConv(strDBPassword, strwPassword);

	/* �A�N�Z�X */
	lChkFlg = cDBA.DBLoginChk(strDBUserName, strDBPassword);

	switch (lChkFlg)
	{
	case 0:
		/* ���C����ʂ̕\�� */
		//this->ShowWindow(SW_HIDE);
		//cMainWindow.DoModal();
		((CMFCSampleApp*)m_pParentApp)->SetLoginSts(true);
		this->EndDialog(100);

		break;

	case 1:
		MessageBox(_T("DB�ڑ��ُ�"));

		strErrorMsg = cDBA.OutErrorMsg();
		MessageBox(strErrorMsg.c_str());

		break;

	case 2:
		MessageBox(_T("���s���܂����B�ē��͂��Ă�������"));
		break;

	default:
		break;
	}

}

/**
* @fn
* �N���A�{�^���N���b�N����
* @brief �N���A�{�^���N���b�N����
* @return �Ȃ�
* @detail �N���A�{�^���N���b�N����
*/
void CLOGIN::OnClear()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	cUserEdit.SetWindowTextW(_T(""));
	cPasswordEdit.SetWindowTextW(_T(""));
}

/**
* @fn
* �N���[�Y�{�^���N���b�N����
* @brief �N���[�Y�{�^���N���b�N����
* @return �Ȃ�
* @detail �N���[�Y�{�^���N���b�N����
*/
void CLOGIN::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CDialogEx::OnClose();
}

/**
* @fn
* ���b�Z�[�W�f�B�X�p�b�`�O����
* @brief ���b�Z�[�W�f�B�X�p�b�`�O����
* @return �Ȃ�
* @detail ���b�Z�[�W�f�B�X�p�b�`�O����
*/
BOOL CLOGIN::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

/**
* @fn
* �A�v���P�[�V�����|�C���^�ݒ菈��
* @brief �A�v���P�[�V�����|�C���^�ݒ菈��
* @return �Ȃ�
* @detail �A�v���P�[�V�����|�C���^�ݒ菈��
*/
void CLOGIN::ParentApp(CWinApp * pParentApp)
{
	m_pParentApp = pParentApp;
}

/**
* @fn
* ���O�C����ʂ̏�����
* @brief ���O�C����ʂ̏�����
* @return �Ȃ�
* @detail ���O�C����ʂ̏�����
*/
BOOL CLOGIN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
