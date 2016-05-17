// CAnkenReadWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "CAnkenReadWindow.h"
#include "afxdialogex.h"


// CAnkenReadWindow dialog

IMPLEMENT_DYNAMIC(CAnkenReadWindow, CDialogEx)

CAnkenReadWindow::CAnkenReadWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANKENREADWINDOW, pParent)
{

}

CAnkenReadWindow::~CAnkenReadWindow()
{
}

void CAnkenReadWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnkenReadWindow, CDialogEx)
END_MESSAGE_MAP()


// CAnkenReadWindow message handlers
