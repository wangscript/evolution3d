/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// BoneGroupNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "BoneGroupNameDlg.h"
#include ".\bonegroupnamedlg.h"
#include "MaxEnv.h"

// CBoneGroupNameDlg dialog

IMPLEMENT_DYNAMIC(CBoneGroupNameDlg, CDialog)
CBoneGroupNameDlg::CBoneGroupNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBoneGroupNameDlg::IDD, pParent)
{
}

CBoneGroupNameDlg::~CBoneGroupNameDlg()
{
}

void CBoneGroupNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_NAME, m_GroupName);
}


BEGIN_MESSAGE_MAP(CBoneGroupNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CBoneGroupNameDlg message handlers

void CBoneGroupNameDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	wchar_t buf[256] = {0};
	GetWindowTextW(m_GroupName.GetSafeHwnd() , buf , 256);
	m_strGroupName =  buf;
	OnOK();
}

void CBoneGroupNameDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CBoneGroupNameDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	wchar_t buf[256]={0};
	WGetWndText(&m_GroupName,buf,256);
	m_strGroupName =  buf;
	CDialog::OnClose();
}

BOOL CBoneGroupNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
