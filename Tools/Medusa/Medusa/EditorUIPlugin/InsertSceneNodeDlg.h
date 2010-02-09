#pragma once
#include "afxwin.h"


// CInsertSceneNodeDlg dialog

class CInsertSceneNodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CInsertSceneNodeDlg)

public:
	CInsertSceneNodeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInsertSceneNodeDlg();

	void       AddModelFile(const wchar_t* file);
	void       InitXmlNode(IBaseRenderer* pRenderer);
	xXmlNode*  Construct();
public:
	xXmlNode*  GetXmlNode() { return &m_XmlNode ; }
// Dialog Data
	enum { IDD = IDD_INSERT_NODE };

	xXmlNode m_XmlNode;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    bool    ReadPlacement();
	void    SetCurrentModelInfo(int curSel , const wchar_t* modelName);
	void    SetNodeInfo();
	DECLARE_MESSAGE_MAP()
public:
	CString m_ModelName;
	CString m_MatName;
	CString m_EffectName;
	CString m_NodeType;
	float m_PosX;
	float m_PosY;
	float m_PosZ;
	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleZ;
	float m_RotX;
	float m_RotY;
	float m_RotZ;
	float m_RotAngle;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_EnitiyType;
	CComboBox m_CbEntityType;
	CComboBox m_CbEffectType;
	CComboBox m_CbMaterialType;
	CComboBox m_CbNodeTypes;
	CComboBox m_CbFileName;
	CString   m_NodeName;

	ds_vector(ds_wstring) m_vFileList;
};
