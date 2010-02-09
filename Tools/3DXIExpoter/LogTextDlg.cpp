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

// LogTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "LogTextDlg.h"
#include ".\logtextdlg.h"


// CLogTextDlg dialog

IMPLEMENT_DYNAMIC(CLogTextDlg, CDialog)
CLogTextDlg::CLogTextDlg(const char* logtext,CWnd* pParent /*=NULL*/)
	: CDialog(CLogTextDlg::IDD, pParent)
{
    m_logText = logtext;
}

CLogTextDlg::~CLogTextDlg()
{

}

void CLogTextDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LOGTEXT, m_LogText);
}


BEGIN_MESSAGE_MAP(CLogTextDlg, CDialog)
END_MESSAGE_MAP()


// CLogTextDlg message handlers

BOOL CLogTextDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_LogText.SetWindowText(m_logText);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
