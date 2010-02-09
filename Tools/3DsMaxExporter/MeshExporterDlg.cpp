// MeshExporterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MeshExporterDlg.h"
#include "MaxEnv.h"
#include "MaxMeshNode.h"
#include "xExportFunction.h"
// CMeshExporterDlg dialog

IMPLEMENT_DYNAMIC(CMeshExporterDlg, CDialog)

CMeshExporterDlg::CMeshExporterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMeshExporterDlg::IDD, pParent)
, m_fFPS( CMaxEnv::singleton().getFps() )
, m_iFirstFrame(0)
, m_iLastFrame(0)
, m_fModelScale(1.0f)
{
	if(CMaxEnv::singleton().m_pInterace->GetSelNodeCount() !=1 )
	{
		::MessageBox(::GetActiveWindow() , "只能导出一个Mesh" , "错误" ,MB_OK);
		m_bInited = false;
		return ;
	}
	m_bInited = true;
}

CMeshExporterDlg::~CMeshExporterDlg()
{
}

void CMeshExporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FPS, m_fFPS);
	DDV_MinMaxFloat(pDX, m_fFPS, 1, 120);
	DDX_Text(pDX, IDC_FIRST_FRAME, m_iFirstFrame);
	DDV_MinMaxInt(pDX, m_iFirstFrame, 0, 10000000);
	DDX_Text(pDX, IDC_LAST_FRAME, m_iLastFrame);
	DDV_MinMaxInt(pDX, m_iLastFrame, 0, 10000000);
}


BEGIN_MESSAGE_MAP(CMeshExporterDlg, CDialog)
	ON_BN_CLICKED(IDB_SAVE_MESH, &CMeshExporterDlg::OnBnClickedSaveMesh)
END_MESSAGE_MAP()


// CMeshExporterDlg message handlers
bool CMeshExporterDlg::get_saved_filename(wchar_t* file_name)
{
	OPENFILENAMEW ofn; 
	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = ::GetActiveWindow();
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"XEvol模型文件(*.xrm)\0*.xrm\0XEvol网格文件(*.mesh)\0*.mesh\0所有文件(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrDefExt = L"mesh";
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if(GetSaveFileNameW(&ofn) == FALSE)
	{
		return false;
	}
	return true;
}

void CMeshExporterDlg::OnBnClickedSaveMesh()
{
	int nFrame = abs(m_iLastFrame-m_iFirstFrame) + 1;
	CMaxMesh mesh;
	mesh.init(CMaxEnv::singleton().m_pInterace->GetSelNode(0));
	if(mesh.is_exportable() == false)
	{
		::MessageBox(::GetActiveWindow() , "Mesh不能导出" , "错误" , MB_OK);
		return ;
	}
	mesh.m_MeshData.m_fFps = m_fFPS;
	unsigned int cur_time = CMaxEnv::singleton().m_pInterace->GetTime();
	mesh.create(cur_time);
	mesh.set_scale(m_fModelScale);
    std::wstring mesh_dir = INodeName(CMaxEnv::singleton().m_pInterace->GetSelNode(0));

	wchar_t file_name[256];
	file_name[0] = 0;
	if(false == get_saved_filename(file_name) )
	{
		::MessageBoxA(::GetActiveWindow(),"没有选择文件","操作被取消",MB_OK);
		return ;
	}
    


	//得到当前的时间．我们只需要导出当前那一帧

	DeleteFileW(file_name);
	xcomdoc doc ;
	doc.create(file_name);

	std::wstring errostring ;
	class CMeshExporter : public CModelBaseExportor{};

	CMeshExporter exporter;
	sSkeletonID_t sid;
	sid.m_HiWord = 0;
	sid.m_LoWord = 0;
	XEVOL_LOG(eXL_DEBUG_HIGH , L"Begin to export mesh to %s\n",file_name );

	xMeshData& meshData = mesh.GetOutputData();
	//建立这个mesh


	//简历这个Mesh的面的信息
	if(false == mesh.build_face_info(false) )
	{
		errostring = std::wstring(L"物体[")+INodeName(mesh.m_pNode)+L"]创建面的信息出错";
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errostring.c_str() );
		mesh.free();
		return ;
	}

	//如果导出到一个.xskin文件。那么导出将覆盖这个文件。所以先删除它。
	if(std::wstring(file_name).find(L".mesh") != std::wstring::npos )
	{
		DeleteFileW(file_name);
		mesh_dir = L"";
	}

	if(nFrame == 1)
	{
		//建立这个mesh的数据信息，找回顶点纹理等信息
		if(false == mesh.build_data_info() )
		{
			errostring = std::wstring(L"物体[")+INodeName(mesh.m_pNode)+L"]创建顶点数据信息出错";
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errostring.c_str() );
			mesh.free();
			return ;
		}

		if(true == mesh.build_morph_target())
		{
			errostring = std::wstring(L"物体[")+INodeName(mesh.m_pNode)+L"] 有Morpher修改器，启用表情系统";
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {提示} %s \n", errostring.c_str()	);
		}


		exporter.export_mesh_data(meshData , mesh_dir , doc , sid , false , false);  
	}
	else
	{
		CModelBaseExportor::vMaxTimes exportTimes;
		int frame_s = m_iFirstFrame;
		int frame_e = m_iLastFrame;
		int frame_n = abs(frame_e - frame_s) + 1;
		int frame_delta = 1;
		unsigned int iMaxTime = frame_s * GetTicksPerFrame();
		if(frame_e < frame_s) frame_delta = -1;

		for(int iframe = frame_s ; iframe != frame_e ; iframe += frame_delta)
		{
			iMaxTime = iframe * GetTicksPerFrame();
			exportTimes.push_back(iMaxTime);
		}

		vector<sBoundBox_t>   FrameBoundBoxs;
		if(false == exporter.export_mesh_action(L"", doc , FrameBoundBoxs , mesh_dir ,mesh, exportTimes) )
		{
			errostring = std::wstring(L"物体[")+INodeName(mesh.m_pNode)+L"] 导出失败";
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {错误} %s \n", errostring.c_str()	);
		}

	}


	doc.close();
	mesh.free();

    OnOK();

	// TODO: Add your control notification handler code here
}
