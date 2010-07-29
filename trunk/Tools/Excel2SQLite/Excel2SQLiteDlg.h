
// Excel2SQLiteDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ExcelWriter.h"
#include "BaseLib/xXmlDocument.h"
USING_NS_XEVOL3D
// CExcel2SQLiteDlg 对话框
class CExcel2SQLiteDlg : public CDialog
{
// 构造
public:
	CExcel2SQLiteDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CExcel2SQLiteDlg();
// 对话框数据
	enum { IDD = IDD_EXCEL2SQLITE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    virtual void SelectExcelSheet(int iSheet);
	void         GenerateSQL();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CComboBox      m_cbExcelSheets;
    CComboBox      m_cbExcelKey;
    CListCtrl      m_ListExcelFields;
    CComboBox      m_cbSqliteTable;
    CListCtrl      m_listExcelData;
    CEdit          m_textUpdateSql;
    CEdit          m_textInsertExcel;
	xXmlNode       m_DataField;
	xXmlNode       m_ExcelData;
	IExcelWriter*  m_pExcelWriter;
	std::wstring   m_strUpdateSQL;
	std::wstring   m_strQuerySQL;
	std::wstring   m_strInsertSQL;
    afx_msg void   OnBnClickedBtnOpenSqlite();

	void           UpdateSQliteTable() ;
	afx_msg void   OnBnClickedBtnOpenExcel();
	afx_msg void OnCbnSelchangeListExcelSheet();
	afx_msg void OnCbnSelchangeExcelKey();
	afx_msg void OnCbnSelchangeSqliteTable();
	afx_msg void OnBnClickedBtnExcute();
    afx_msg void OnBnClickedBtnGenSql();
};
