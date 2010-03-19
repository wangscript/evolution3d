#include "../xStdPch.h"
#include "xXrmModel.h"
#include "../xcomdoc/xdocfstream.h"
#include "xCoreMesh.h"
#include "xCoreSkeleton.h"
#include "xBaseModel.h"
#include "xCoreMeshMgr.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xXrmModel , xBaseModel);

IMPL_OBJECT_FACTORY(xXrmModel , xBaseModel , xXrmModelCreator , xBaseModelCreator , xBaseModelCreatorMgr , L"xrm"    , L"xreal model" , xBaseTextureMgr* , bool)
IMPL_OBJECT_FACTORY(xXrmModel , xBaseModel , xErmModelCreator , xBaseModelCreator , xBaseModelCreatorMgr , L"erm"    , L"xreal model" , xBaseTextureMgr* , bool)

xXrmModel::xXrmModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy)
:xBaseModel(pTexMgr , bSysMemCopy)
{
}

xXrmModel::~xXrmModel()
{

}

//====================
bool xXrmModel::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	xcomdoc doc;
	bool bRet = true;
	if(buf == NULL || bufLen == 0)
	{
		bRet = doc.open(fileName,  xcdm_read);
	}
	else
	{
		bRet = doc.open( (const _xcd_int8*)buf, (size_t)bufLen,  xcdm_read);
	}

	//如果不是打包的。
	if( bRet == false )
	{
		return false;
	}

	xcomdocstream* pDesc = doc.open_stream(L"desc.xml");
	if(pDesc != NULL)
	{
		bRet = load(doc , L"" , arg);
		doc.close_stream(pDesc);
		doc.close();
	}
	return bRet;
}

bool xXrmModel::_loadModelMeshMateril( xCoreMesh* mesh , xXmlNode* pMeshNode)
{
	return false;
}

bool xXrmModel::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
	bool bLoadAllMesh = arg != 0;
	//读入文件头
	ds_wstring descName = ds_wstring(_dir) + L"desc.xml";
	xcomdocstream* pdescstream = doc.open_stream(descName.c_str() );
	xcdstream  _in(pdescstream);
	xXmlDocument xml;
	xml.load(_in);
	_in.close();
	doc.close_stream(pdescstream);

	//XML加载完毕
	xXmlNode* pRootNode = xml.root();

	m_name = pRootNode->value(L"name");
	xXmlNode* pAABBNode = pRootNode->findNode(L"BoundBox");
	m_aabb.m_Min = xvec3(0.0f , 0.0f , 0.0f);
	m_aabb.m_Max = xvec3(1.0f , 1.0f , 1.0f);
	if(pAABBNode)
	{
		m_aabb.m_Min.x = pAABBNode->float_value(L"min_x");
		m_aabb.m_Min.y = pAABBNode->float_value(L"min_y");
		m_aabb.m_Min.z = pAABBNode->float_value(L"min_z");

		m_aabb.m_Max.x = pAABBNode->float_value(L"max_x");
		m_aabb.m_Max.y = pAABBNode->float_value(L"max_y");
		m_aabb.m_Max.z = pAABBNode->float_value(L"max_z");
	}

	//开始加载Skeleton
	loadSkeleton(doc , _dir);
    if(m_pSkeleton == NULL)
	{
		loadAction(doc , _dir);
	}
	if(bLoadAllMesh)
	{
		//开始加载Mesh
		loadEmbMeshs(doc , _dir , pRootNode);

		//加载SkinGroup
		loadEmbSkinGroup(doc , _dir , pRootNode); 
	}


	//创建一个BoneFrameBuffer;
	createBoneFrameBuffer();

	//属于自己的Mesh加载完了，现在加载外部的MESH
	//外部的HCoreMesh来源于Mesh管理器，但是Mesh管理器不会进行任何的资源淘汰活动。
	//所有的资源淘汰算法都是基于Model的。
	//而且Mesh即使进行了资源淘汰，也不会删除xCoreMesh对象，因此，将xCoreMesh对象保存起来是没有任何问题的。
	size_t _nCoreMeshs = m_hCoreMeshs.size() ;
	for(size_t i = 0 ; i < _nCoreMeshs ;  i ++)
	{
		xCoreMesh* pMesh = m_hCoreMeshs[i].getResource();

		//因为unload的时候，会Release所有的Object，所以这里得加个AddRef
		pMesh->AddRef();
		m_Meshs.push_back(pMesh);
	}
	return true;
}

void xXrmModel::_setupMesh( xCoreMesh* pMesh, xXmlNode*pMeshNode ) 
{
	_loadModelMeshMateril(pMesh,pMeshNode);
	m_Meshs.push_back(pMesh);
}

bool xXrmModel::loadAction(xcomdoc& doc ,const wchar_t* _dir)
{
	ds_wstring pActionDescName = ds_wstring(_dir) + L"actions.xml";
	xcomdocstream* pActDescStream = doc.open_stream(pActionDescName.c_str() );
	if(pActDescStream != NULL)
	{
		xcdstream  _in(pActDescStream);
		xXmlDocument xml;
		xml.load(_in);
		_in.close();
		xXmlNode* pRootNode = xml.root();
		xXmlNode::XmlNodes actNodes;
		pRootNode->findNode(actNodes);

		size_t _ActNodes = actNodes.size() ;
		for(size_t  i = 0 ;i < _ActNodes ; i ++)
		{
			xCoreAction* pAction = new xEmbAction();
			pAction->setBoneNumber( 0 );
			pAction->load(actNodes[i]);
			m_Actions.push_back(pAction);
		}   

		doc.close_stream(pActDescStream);
		return true;
		//加载内置的动作
	}
	return false;
}

bool xXrmModel::loadSkeleton(xcomdoc& doc , const wchar_t* _dir)
{
	m_pSkeleton = NULL;
	ds_wstring skeletonName = ds_wstring(_dir) + L"skeleton/skeleton";
	xcomdocstream* pSkelStream = doc.open_stream(skeletonName.c_str() );
	if(pSkelStream != NULL)
	{
		m_pSkeleton = new xCoreSkeleton;
		if( false == m_pSkeleton->load(doc , _dir ) )
		{
			delete m_pSkeleton;
			m_pSkeleton = NULL;
		}
		doc.close_stream(pSkelStream);
		//加载内置的动作
	}

	ds_wstring pActionDescName = ds_wstring(_dir) + L"skeleton/actions.xml";
	xcomdocstream* pActDescStream = doc.open_stream(pActionDescName.c_str() );
	if(pActDescStream != NULL)
	{
		xcdstream  _in(pActDescStream);
		xXmlDocument xml;
		xml.load(_in);
		_in.close();
		ds_wstring actDir = ds_wstring(_dir) + L"skeleton/";
		xXmlNode* pRootNode = xml.root();
		xXmlNode::XmlNodes actNodes;
		pRootNode->findNode(actNodes);

		size_t _ActNodes = actNodes.size() ;
		for(size_t  i = 0 ;i < _ActNodes ; i ++)
		{
			xCoreAction* pAction = new xEmbAction();
			pAction->setBoneNumber(m_pSkeleton->nBone() );
			xXmlNode* pNode = actNodes[i];
			if(false == pAction->load(pNode , doc , actDir.c_str()) )
			{
				delete pAction;
			}
			else
			{
				m_Actions.push_back(pAction);
			}
		}   

		doc.close_stream(pActDescStream);
		//加载内置的动作
	}
	return true;
}

bool xXrmModel::loadEmbMeshs(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode)
{
	xXmlNode::XmlNodes meshNodes;
	pRootNode->findNode(L"mesh" , meshNodes);
	size_t _MeshNodes = meshNodes.size() ;
	for(size_t i = 0 ; i <  _MeshNodes ; i ++)
	{
		xXmlNode* pMeshNode = meshNodes[i];
		const wchar_t* name = pMeshNode->value(L"name");
		xCoreMesh* pMesh = new xCoreMesh(m_pTexMgr , m_pSkeleton);
		ds_wstring meshName = ds_wstring(_dir) + L"/" + name + L"/";
		if( pMesh->load(doc , meshName.c_str(), 0) == false)
		{
			delete pMesh;
			continue;
		}
		_setupMesh(pMesh, pMeshNode);

	}
	return true;
}

bool xXrmModel::loadEmbSkinGroup(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode)
{
	xXmlNode::XmlNodes skinGroupNodes;
	pRootNode->findNode(L"skinGroup" , skinGroupNodes);

	size_t _nSkinGroupNodes = skinGroupNodes.size() ;
	for(size_t i = 0 ; i < _nSkinGroupNodes ; i ++)
	{
		xXmlNode* pSkinGrouNode = skinGroupNodes[i];
		const wchar_t* skinGroupName = pSkinGrouNode->value(L"name");
		xCoreMesh* pMesh = new xCoreMesh(m_pTexMgr , m_pSkeleton);
		ds_wstring skinGroupDesName = ds_wstring(_dir) + L"/" + skinGroupName + L"/skins.xml";
		xcomdocstream* pdescstream = doc.open_stream(skinGroupDesName.c_str() );
		xcdstream  _in(pdescstream);
		xXmlDocument xml;
		xml.load(_in);
		_in.close();
		xXmlNode* pRootNode = xml.root();

		xXmlNode::XmlNodes meshNodes;
		pRootNode->findNode(L"mesh" , meshNodes);
		size_t _nMeshNodes = meshNodes.size() ;
		for(size_t i = 0 ; i < _nMeshNodes;  i ++)
		{
			xXmlNode* pMeshNode = meshNodes[i];
			const wchar_t* name = pMeshNode->value(L"name");
			xCoreMesh* pMesh = new xCoreMesh(m_pTexMgr , m_pSkeleton);
			ds_wstring meshName = ds_wstring(_dir) + L"/" + skinGroupName + L"/" + name + L"/" ;
			if( pMesh->load(doc , meshName.c_str(), 0) == false)
			{
				delete pMesh;
				continue;
			}
			_setupMesh(pMesh, pMeshNode);
		}

	}
	return true;
}

END_NAMESPACE_XEVOL3D

