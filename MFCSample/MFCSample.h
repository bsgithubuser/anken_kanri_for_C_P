
// MFCSample.h : MFCSample �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CMFCSampleApp:
// ���̃N���X�̎����ɂ��ẮAMFCSample.cpp ���Q�Ƃ��Ă��������B
//

class CMFCSampleApp : public CWinApp
{
public:
	CMFCSampleApp();
	void SetLoginSts(bool bLoginSts);
	bool m_bLoginSts = false;

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

};

extern CMFCSampleApp theApp;
