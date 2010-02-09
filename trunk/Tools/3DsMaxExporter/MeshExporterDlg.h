#pragma once


// CMeshExporterDlg dialog

class CMeshExporterDlg : public CDialog
{
	DECLARE_DYNAMIC(CMeshExporterDlg)

public:
	CMeshExporterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMeshExporterDlg();

// Dialog Data
	enum { IDD = IDD_MESH_EXPORTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSaveMesh();
	bool  get_saved_filename(wchar_t* file_name);
	float m_fFPS;
	bool  m_bInited;
	int   m_iFirstFrame;
	int   m_iLastFrame;
	float m_fModelScale;
};
