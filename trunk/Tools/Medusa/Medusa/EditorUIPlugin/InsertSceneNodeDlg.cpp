// EditorUIPlugin\InsertSceneNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../EditorFramework/Medusa.h"
#include "InsertSceneNodeDlg.h"


// CInsertSceneNodeDlg dialog

IMPLEMENT_DYNAMIC(CInsertSceneNodeDlg, CDialog)

CInsertSceneNodeDlg::CInsertSceneNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertSceneNodeDlg::IDD, pParent)
	, m_ModelName(_T(""))
	, m_MatName(_T("SimpleModelBlend"))
	, m_EffectName(_T("KidBasicEffect"))
	, m_NodeType(_T("xSceneBasicNode"))
	, m_EnitiyType(_T("xSceneModel"))
	, m_PosX(0.0f)
	, m_PosY(0.0f)
	, m_PosZ(0.0f)
	, m_ScaleX(1.0f)
	, m_ScaleY(1.0f)
	, m_ScaleZ(1.0f)
	, m_RotX(0.0f)
	, m_RotY(0.0f)
	, m_RotZ(1.0f)
	, m_RotAngle(0.0f)
	, m_NodeName(_T(""))
{
    
	wchar_t _buf[1024] = {0};
	swprintf(_buf , L"NodeName:%d" , rand()%100000);
	m_NodeName = _buf;
}

void CInsertSceneNodeDlg::AddModelFile(const wchar_t* fileName)
{
	m_vFileList.push_back(fileName);
}

void CInsertSceneNodeDlg::InitXmlNode(IBaseRenderer* pRenderer)
{
	xXmlNode* pChindNode = m_XmlNode.insertNode(L"child");
	pChindNode->setValue(L"type" , m_NodeType.GetBuffer() );
	pChindNode->setValue(L"name" , m_NodeName.GetBuffer() );
	for(int i = 0 ; i < m_vFileList.size() ; i ++)
	{
		xXmlNode* pEntityNode = pChindNode->insertNode(L"entity");
		pEntityNode->setValue(L"effect" , m_EffectName.GetBuffer() );
		pEntityNode->setValue(L"type" , m_EnitiyType.GetBuffer());
		pEntityNode->setValue(L"file" , m_vFileList[i].c_str() );
		pEntityNode->setValue(L"material" , m_MatName.GetBuffer() );
	}


	xXmlNode* pRot = m_XmlNode.insertNode(L"Rotate");
	pRot->setValue(L"x"     , m_RotX);
	pRot->setValue(L"y"     , m_RotY);
	pRot->setValue(L"z"     , m_RotZ);
	pRot->setValue(L"angle" , m_RotAngle);

	xXmlNode* pPos = m_XmlNode.insertNode(L"Position");
	pPos->setValue(L"x" , m_PosX);
	pPos->setValue(L"y" , m_PosY);
	pPos->setValue(L"z" , m_PosZ);

	xXmlNode* pScale = m_XmlNode.insertNode(L"Scale");
	pScale->setValue(L"x" , m_ScaleX);
	pScale->setValue(L"y" , m_ScaleY);
	pScale->setValue(L"z" , m_ScaleZ);

	m_XmlNode.setValue(L"file"     , m_ModelName.GetBuffer() );
	m_XmlNode.setValue(L"material" , m_MatName.GetBuffer() );
	m_XmlNode.setValue(L"effect"   , m_EffectName.GetBuffer() );
	m_XmlNode.setValue(L"nodetype" , m_NodeType.GetBuffer() );


	//初始化Material列表
	xXmlNode* pMatList = m_XmlNode.insertNode(L"Materials");
	int nMat = pRenderer->nMaterial();
	for(int i = 0 ; i < nMat ; i ++)
	{
		const wchar_t* matName = pRenderer->materialName(i);
		pMatList->insertNode(L"type")->setValue(L"name" , matName);
	}

	//初始化Effect列表
	xXmlNode* pEffectList = m_XmlNode.insertNode(L"Effects");
	int nEff = pRenderer->nRenderEffect();
	for(int i = 0 ; i < nEff ; i ++)
	{
		const wchar_t* effectName = pRenderer->renderEffectName(i);
		pEffectList->insertNode(L"type")->setValue(L"name" , effectName);
	}

	//初始化NodeTypes列表
	xXmlNode* pNodeTypes = m_XmlNode.insertNode(L"NodeTypes");
	int nNodeType = xSceneNodeFactoryMgr::singleton()->nFactories();
	for(int i = 0 ; i < nNodeType ; i ++)
	{
		const wchar_t* nodeTypeName =  xSceneNodeFactoryMgr::singleton()->find(i)->name();
		pNodeTypes->insertNode(L"type")->setValue(L"name" , nodeTypeName);
	}

	//初始化Entity列表
	xXmlNode* pEntityTypes = m_XmlNode.insertNode(L"EntityTypes");
	int nEntityType = xSceneObjectFactoryMgr::singleton()->nFactories();
	for(int i = 0 ; i < nEntityType ; i ++)
	{
		const wchar_t* entityTypeName =  xSceneObjectFactoryMgr::singleton()->find(i)->name();
		pEntityTypes->insertNode(L"type")->setValue(L"name" , entityTypeName);
	}

	//
}
xXmlNode*   CInsertSceneNodeDlg::Construct()
{
	SetNodeInfo();
	ReadPlacement();
	for(size_t i = 0 ; i < m_vFileList.size() ; i ++)
	{
		SetCurrentModelInfo(i , m_vFileList[i].c_str() );
	}
	return &m_XmlNode;
}
BOOL CInsertSceneNodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Enitiy Types;
	xXmlNode::XmlNodes nodes;
	m_XmlNode.findNode(L"EntityTypes")->findNode(L"type", nodes) ;
	for(size_t i = 0 ; i < nodes.size() ; i ++)
	{
		xXmlNode* pNode = nodes[i];
		m_CbEntityType.InsertString(i , pNode->value(L"name") );
	}
	nodes.clear();

	//Effect Types;
	m_XmlNode.findNode(L"Effects")->findNode(L"type", nodes) ;
	for(size_t i = 0 ; i < nodes.size() ; i ++)
	{
		xXmlNode* pNode = nodes[i];
		m_CbEffectType.InsertString(i , pNode->value(L"name") );
	}
	nodes.clear();

	//Material Types;
	m_XmlNode.findNode(L"Materials")->findNode(L"type", nodes) ;
	for(size_t i = 0 ; i < nodes.size() ; i ++)
	{
		xXmlNode* pNode = nodes[i];
		m_CbMaterialType.InsertString(i , pNode->value(L"name") );
	}
	nodes.clear();

	//NodeType Types;
	m_XmlNode.findNode(L"NodeTypes")->findNode(L"type", nodes) ;
	for(size_t i = 0 ; i < nodes.size() ; i ++)
	{
		xXmlNode* pNode = nodes[i];
		m_CbNodeTypes.InsertString(i , pNode->value(L"name") );
	}
	nodes.clear();

	//文件列表
	for(size_t i = 0 ; i < m_vFileList.size()  ; i ++)
	{
		m_CbFileName.InsertString(0,m_vFileList[i].c_str() );
	}
	m_CbFileName.SetWindowText(m_vFileList[0].c_str());




	xXmlNode* pChildNode = m_XmlNode.findNode(L"child");
	m_NodeType = pChildNode->value(L"type");
	m_NodeName = pChildNode->value(L"name");

	xXmlNode* pEntityNode = pChildNode->findNode(L"entity");
	m_EnitiyType = pEntityNode->value(L"type");
	m_EffectName = pEntityNode->value(L"effect");
	m_MatName    = pEntityNode->value(L"material");

	UpdateData(FALSE);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CInsertSceneNodeDlg::ReadPlacement()
{
     xXmlNode* pChildNode = m_XmlNode.findNode(L"child");
	 xXmlNode* pPlacement = pChildNode->findNode(L"placement");
	 if(pPlacement == NULL)  pPlacement = pChildNode->insertNode(L"placement");
	 wchar_t buf[1024] = {0};
	 swprintf(buf , L"[%f,%f,%f]" , m_PosX , m_PosY , m_PosZ);
	 pPlacement->setValue(L"pos" , buf);

	 swprintf(buf , L"[%f,%f,%f]" , m_ScaleX , m_ScaleY , m_ScaleZ);
	 pPlacement->setValue(L"scale" , buf);

	 swprintf(buf , L"[%f,%f,%f,%f]" , m_RotX , m_RotY , m_RotZ , m_RotAngle);
	 pPlacement->setValue(L"rotate" , buf);
	 return true;

}
void CInsertSceneNodeDlg::SetNodeInfo()
{
	xXmlNode* pChildNode = m_XmlNode.findNode(L"child");
	pChildNode->setValue(L"type" , m_NodeType.GetBuffer() );
	pChildNode->setValue(L"name" , m_NodeName.GetBuffer() );
}

void CInsertSceneNodeDlg::SetCurrentModelInfo(int curSel , const wchar_t* modelName)
{
	//wchar_t modelName[1024] = {0};
	//m_CbFileName.GetWindowText(modelName , 1024);
	//int curSel = m_CbFileName.GetCurSel();
	m_vFileList[curSel] = modelName;
	for(size_t i = 0 ; i < m_vFileList.size()  ; i ++)
	{
		if(m_vFileList[i] == modelName)
		{
			xXmlNode* pChildNode = m_XmlNode.findNode(L"child");
			xXmlNode* pEnitiyNode = pChildNode->findNode(L"entity" , i);
			if(pEnitiyNode)
			{
				pEnitiyNode->setValue(L"type"    , m_EnitiyType.GetBuffer() ) ;
				pEnitiyNode->setValue(L"material", m_MatName.GetBuffer() ) ;
				pEnitiyNode->setValue(L"effect"  , m_EffectName.GetBuffer() ) ;
				pEnitiyNode->setValue(L"file"    , modelName) ;
			}
		}
	}
}

CInsertSceneNodeDlg::~CInsertSceneNodeDlg()
{
}

void CInsertSceneNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_ModelName);
	DDX_CBString(pDX, IDC_CB_MATERIAL, m_MatName);
	DDX_CBString(pDX, IDC_CB_EFFECT, m_EffectName);
	DDX_CBString(pDX, IDC_CB_NODETYPE, m_NodeType);
	DDX_Text(pDX, IDC_POS_X, m_PosX);
	DDX_Text(pDX, IDC_POS_Y, m_PosY);
	DDX_Text(pDX, IDC_POS_Z, m_PosZ);
	DDX_Text(pDX, IDC_SCALE_X, m_ScaleX);
	DDX_Text(pDX, IDC_SCALE_Y, m_ScaleY);
	DDX_Text(pDX, IDC_SCALE_Z, m_ScaleZ);
	DDX_Text(pDX, IDC_ROT_X, m_RotX);
	DDX_Text(pDX, IDC_ROT_Y, m_RotY);
	DDX_Text(pDX, IDC_ROT_Z, m_RotZ);
	DDX_Text(pDX, IDC_ROT_ANGLE, m_RotAngle);
	DDX_CBString(pDX, IDC_CB_ENTITIY_TYPE, m_EnitiyType);
	DDX_Control(pDX, IDC_CB_ENTITIY_TYPE, m_CbEntityType);
	DDX_Control(pDX, IDC_CB_EFFECT, m_CbEffectType);
	DDX_Control(pDX, IDC_CB_MATERIAL, m_CbMaterialType);
	DDX_Control(pDX, IDC_CB_NODETYPE, m_CbNodeTypes);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_CbFileName);
	DDX_Text(pDX, IDC_TEXT_NODENAME, m_NodeName);
}


BEGIN_MESSAGE_MAP(CInsertSceneNodeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInsertSceneNodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInsertSceneNodeDlg message handlers

void CInsertSceneNodeDlg::OnBnClickedOk()
{
	UpdateData();
	Construct();
	// TODO: Add your control notification handler code here
	OnOK();
}

