
// MFCSample.h : MFCSample アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CMFCSampleApp:
// このクラスの実装については、MFCSample.cpp を参照してください。
//

class CMFCSampleApp : public CWinApp
{
public:
	CMFCSampleApp();
	void SetLoginSts(bool bLoginSts);
	bool m_bLoginSts = false;

// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

};

extern CMFCSampleApp theApp;
