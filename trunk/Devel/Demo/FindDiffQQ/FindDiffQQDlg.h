// FindDiffQQDlg.h : header file
//

#pragma once

#include "xILImage.h"
using namespace XEvol3D;
// CFindDiffQQDlg dialog
class CFindDiffQQDlg : public CDialog
{

	HWND m_hQQGameWnd;
	int  X1;
	int  Y1;
// Construction
public:
	CFindDiffQQDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FINDDIFFQQ_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	void OnIdle();
    void DoFindDiff(xILImage& iamge);
	void FindByClipBoard();
	void FindBySnapWindow();
	BOOL FindAWindow(HWND hwnd);
// Implementation
protected:
	HICON m_hIcon;
    bool   GrabDataFromClipboard(xILImage& image);
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void toggleAuto();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAtuo();
	afx_msg void OnStop();
	afx_msg void OnUpdateAtuo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStop(CCmdUI *pCmdUI);
};
