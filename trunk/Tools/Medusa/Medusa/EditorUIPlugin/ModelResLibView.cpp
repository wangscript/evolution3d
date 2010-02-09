
#include "stdafx.h"
#include "ModelResLibView.h"
#include "../Resource.h"
#include <BaseLib/xStringTable.h>
#include "../xMedusaEditor.h"
#include "../xEvolEnviroment.h"
#include "../Editor/EditorEnv.h"
#include "InsertSceneNodeDlg.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelResLibView
#define _MEdUI_NAME L"ModelResLibView";
#define _MEdUI_DESC L"ModelResLibView";
IMPL_OBJECT_FACTORY_NO_ARG(CModelResLibView , IMEdUIElement, CModelResLibViewFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_NAME , _MEdUI_DESC);
// CResourceViewBar
const wchar_t* CModelResLibView::typeName()
{
	return _MEdUI_NAME;
}
CModelResLibView::CModelResLibView()
{
	m_RootItem.m_hTreeItem = NULL;
}

CModelResLibView::~CModelResLibView()
{
}

BEGIN_MESSAGE_MAP(CModelResLibView, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MRV_INSERT, OnInserModel)
	ON_COMMAND(ID_MODELOPERATOR_REFLESHLIB,OnRefleshLib)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

bool  CModelResLibView::onDetach()
{
	return true;
}

bool  CModelResLibView::onAttach()
{
	return true ;
}

HICON  CModelResLibView::hIcon()
{
	BOOL bHiColorIcons = TRUE;
	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE( IDI_MODEL_RESVIEW ), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	return hPropertiesBarIcon;
}

const wchar_t*  CModelResLibView::title()
{
	return NS_XEVOL3D::_TRANSLATE(L"Model Resource Lib");
}

const wchar_t*  CModelResLibView::name()
{
	return NS_XEVOL3D::_TRANSLATE(L"ModelResLibView");
}

void  CModelResLibView::setID(UINT id)
{
	m_CtrlID = id;
}

UINT  CModelResLibView::getID()
{
	return m_CtrlID;
}

HWND  CModelResLibView::hWnd()
{
	return GetSafeHwnd();
}

bool  CModelResLibView::onMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
	if(_event == eMEUI_SceneLoaded || _event == eMEUI_ScenePathChange)
	{
         this->OnRefleshLib();
		 return true;
	}
	return false;	
}

bool  CModelResLibView::createMEdUI(HWND hMainWnd)
{
	CWnd* pWnd = CWnd::FromHandle(hMainWnd);
	LPCTSTR clsName = AfxRegisterWndClass(CS_DBLCLKS);
	if(FALSE !=  CWnd::Create(clsName , L"Hello" , WS_CHILD, CRect(0,0,200,200) , pWnd ,0x00f123, NULL ) )
	{
		return true;
	}
	return false;
}

bool  CModelResLibView::destroyMEdUI()
{
	this->DestroyWindow();
	return true;
}

bool CModelResLibView::HideMEdUI()
{
	GetParent()->ShowWindow(SW_HIDE);
	return true;
}

bool CModelResLibView::ShowMEdUI()
{
	GetParent()->ShowWindow(SW_SHOW);
	GetParent()->UpdateWindow();
	return true;
}


int CModelResLibView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_ctrlTree.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_ShaderLibViewImages.Create(IDB_MODEL_RESVIEW, 16, 0, RGB(255, 0, 255));
	m_ctrlTree.SetImageList(&m_ShaderLibViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MODLRESVIEW);
	m_wndToolBar.LoadToolBar(IDR_MODLRESVIEW /* Is locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	m_wndToolBar.ShowWindow(SW_SHOW);

	( (CWinAppEx*)AfxGetApp())->GetContextMenuManager()->AddMenu(TEXT("ModelResView"),IDR_POPUP_MODELRESVIEW);
	// Fill in some static tree view data (dummy code, nothing magic here)
	AdjustLayout();
	return 0;
}

void CModelResLibView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	AdjustLayout();
}

class CModelFileEnumListener : public IDirEnumListenner
{
	CModelResLibView* m_pView;
	CModelResViewTreeItem* m_pRootItem;
public:
	CModelFileEnumListener(CModelResLibView* pView  , CModelResViewTreeItem* pRootItem)
	{
		m_pView = pView;
		m_pRootItem = pRootItem;
	}

	bool onFindFile(xDirEnumItem& item)
	{
		return m_pView->FindModelFile(item , m_pRootItem);
	}
};

bool CModelResLibView::FindModelFile(xDirEnumItem& item , CModelResViewTreeItem* pRootItem)
{
    if(item.m_isDir)
	{
		CModelResViewTreeItem treeItem;
		if(item.m_Name[0] == '.' && item.m_Name != L".")
			return true;

		treeItem.m_hTreeItem = m_ctrlTree.InsertItem( item.m_Name.c_str() , pRootItem->m_hTreeItem , 0);
		m_ctrlTree.SetItemState(treeItem.m_hTreeItem, TVIS_BOLD, TVIS_BOLD);
		treeItem.m_FileName = item.m_FullName.c_str();
		treeItem.m_ItemType = eMRVTI_Folder;
		CModelResViewTreeItem* pChildItem = pRootItem->AddChild(treeItem);

		CModelFileEnumListener dirListenner(this , pChildItem);
		xDirEnum dirEnum;
		dirEnum.addListenner( &dirListenner );
		dirEnum.enumDir(item.m_FullName.c_str() , false);
		return true;
	}
	else
	{
		CModelResViewTreeItem treeItem;
		//if(item.m_Name[0] = '.')
		//	return true;
		std::wstring _extName = xFileSystem::singleton()->getFileExtName( item.m_FullName.c_str() );
		if(_extName == L"xrm" || _extName == L"mesh" || _extName == L"xmesh"  || _extName == L"skeleton"    || item.m_Name == L"desc.xml"  ||//内置的模型
		   _extName == L"3ds" || _extName == L"obj"  || _extName == L"ase"    || _extName == L"dxf"         ||                               //扩展的类型
		   _extName == L"shp") 
		{
			treeItem.m_hTreeItem = m_ctrlTree.InsertItem( item.m_Name.c_str() , 1 , 1 , pRootItem->m_hTreeItem );
			m_ctrlTree.SetItemState(treeItem.m_hTreeItem, TVIS_BOLD, TVIS_BOLD);
			treeItem.m_FileName = item.m_FullName.c_str();
			treeItem.m_ItemType = eMRVTI_File;
			CModelResViewTreeItem* pChildItem = pRootItem->AddChild(treeItem);
		}

		return true;
	}
}

void CModelResLibView::FillModelResTreeView(xResPathManager* PathManager)
{
	m_RootItem.DeleteChildren(m_ctrlTree);
	m_RootItem.m_FileName = L"";
	m_RootItem.m_ItemType = eMRVTI_Root;
	m_RootItem.m_hTreeItem = m_ctrlTree.InsertItem( _TRANSLATE(_T("Model Resource Lib")), 0, 0);
	m_ctrlTree.SetItemState(m_RootItem.m_hTreeItem, TVIS_BOLD, TVIS_BOLD);

	int nItem = PathManager->nPackageItem();
	for(int i = 0 ; i < nItem ; i ++)
	{
		xResPkgPathItem& _item = PathManager->packageItem(i);
		if(_item.m_Package.failed() )
		{
			
			xDirEnumItem dirItem;
			dirItem.m_FileSizeHi = 0;
			dirItem.m_FileSizeLow = 0;
			dirItem.m_FullName = _item.m_ResDir.c_str();
			dirItem.m_isDir = true;
			dirItem.m_Name = _item.m_ResDir.c_str();
			dirItem.m_Path = _item.m_ResDir.c_str();
			FindModelFile(dirItem , &m_RootItem);
		}
	}
}

void CModelResLibView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_ctrlTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CWnd::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	( (CWinAppEx*)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MODELRESVIEW, point.x, point.y, this, TRUE);
}

void CModelResLibView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ctrlTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
void CModelResLibView::OnRefleshLib()
{
	ISceneGraph* pScene = GetEditorEnv()->evol3DEnv()->scene();
	xResPathManager* PathManager = pScene->getPathManager();
	FillModelResTreeView(PathManager);
}
void CModelResLibView::OnInserModel()
{
	HTREEITEM hTreeItem = m_ctrlTree.GetSelectedItem();
	CModelResViewTreeItem* pItem = m_RootItem.FindTreeItem(hTreeItem);
	if(pItem->m_ItemType == eMRVTI_File)
	{
		ISceneGraph* pScene = GetEditorEnv()->evol3DEnv()->scene();
		//获取地形高度和位置
		xTextureDesc _desc;
		GetEvol3DEnv()->renderApi()->getRenderView()->desc(_desc);
		xvec4 pos = GetEvol3DEnv()->GetTerrainHeight( _desc , _desc.m_width/2 , _desc.m_height* 4 / 5 );


		const wchar_t* fileName = pItem->m_FileName.c_str();
		std::ds_wstring _localFile = pScene->getPathManager()->relativePath(fileName);
		if(_localFile == fileName)
		{
			std::ds_wstring pathName = xFileSystem::singleton()->getPathName(fileName) + L"/";
			wchar_t message[1024] = {0};
			swprintf(message , L"文件: %S 不在资源管理器路径下 ，\n 将路径: %S 添加到资源管理器中么?" , fileName , pathName.c_str() );
			if(IDOK == MessageBox(message , L"提示" , MB_OKCANCEL) )
			{
               pScene->getPathManager()->addPath(pathName.c_str() );
				_localFile = pScene->getPathManager()->relativePath(fileName);
				GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_ScenePathChange , (int)pScene , (int)pathName.c_str() );
			}
		}

		const wchar_t* localFile =_localFile.c_str();
        CInsertSceneNodeDlg  insertNodeDlg;
		insertNodeDlg.AddModelFile( localFile );
		insertNodeDlg.InitXmlNode( GetEvol3DEnv()->renderer()  );
		
		insertNodeDlg.m_PosX = pos.x; 	insertNodeDlg.m_PosY = pos.y; 	insertNodeDlg.m_PosZ = pos.z;

		//看看插件里有没有人要处理这个文件的插入动作
        if(GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_PrepaireInsertFile , (int)fileName , 0) == true)
		{
			return ;
		}

		GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_PrepaireInsertNode , (int)insertNodeDlg.GetXmlNode() , 0 );
		if(IDOK == insertNodeDlg.DoModal() )
		{
			HBaseModel hModel = pScene->getModelManager()->add(localFile , true , true);
			xBaseModel* pModel = hModel.getResource();
			if(pModel == NULL)
			{
				MessageBox(TEXT("选择的文件不是个模型文件"),TEXT("错误"),MB_OK);
			}

			//if(pModel->skeleton() == NULL)
			//{
			//	_node.setValue(L"entity", L"material" , L"SimpleModelBlend");
			//}
			//else
			//{
			//	_node.setValue(L"entity", L"material" , L"SimpleSkinBlend");
			//}		
			//_node.setValue(L"entity", L"type"     , L"xSceneModel");

			ISceneNode* pSelSceneNode = GetSceneSelection()->GetCurSelNode();
			if(pSelSceneNode == NULL) 
			{
				pSelSceneNode = pScene;
			}
		
			xmat4 transMat = pSelSceneNode->tansMatrix();
			//父节点的偏移要减掉。
			insertNodeDlg.m_PosX -= transMat.m[3][0];
			insertNodeDlg.m_PosY -= transMat.m[3][1];
			insertNodeDlg.m_PosZ -= transMat.m[3][2];
			
			ISceneNode* pNewSceneNode = XEvol_InsertSceneNode(insertNodeDlg.Construct()->findNode(L"child") , pScene , pSelSceneNode);
			if(pNewSceneNode)
			{
				GetMedusaEditor()->GetUI()->InsertCommand(xInsertNodeCommand::newInstacne(pSelSceneNode , pNewSceneNode));
				pNewSceneNode->updateChildrenTrans();
				IMEdMainUI* pUI = (GetMedusaEditor()->GetUI());
				if (NULL != pUI)
				{
					pUI->fireMEdUIEvent(eMEUI_SceneInsertNode , (int)pScene , (int)pNewSceneNode );
				}
			}
			else
			{
				MessageBox(TEXT("插入场景节点错误"),TEXT("错误"),MB_OK);
			}
		}


		
	}
}

void CModelResLibView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_ctrlTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CModelResLibView::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	m_ctrlTree.SetFocus();
}

void CModelResLibView::OnChangeVisualStyle()
{
}


