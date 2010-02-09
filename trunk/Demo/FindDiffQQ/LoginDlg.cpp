// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FindDiffQQ.h"
#include "LoginDlg.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, m_TextUserName);
	DDX_Text(pDX, IDC_PASSWORD, m_TextPasswd);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDlg message handlers
bool bUserLogin = false;
void CLoginDlg::OnBnClickedOk()
{
	UpdateData();
	bUserLogin = false;
	if(m_TextPasswd == TEXT("zxsaqw") && m_TextUserName == TEXT("user"))
	{
		bUserLogin = true;
	}
	else if(m_TextPasswd == TEXT("qwerty") && m_TextUserName == TEXT("pll"))
	{
		bUserLogin = true;
	}
	// TODO: Add your control notification handler code here
	OnOK();
}

void CLoginDlg::OnBnClickedCancel()
{
	bUserLogin = false;
	// TODO: Add your control notification handler code here
	OnCancel();
}
