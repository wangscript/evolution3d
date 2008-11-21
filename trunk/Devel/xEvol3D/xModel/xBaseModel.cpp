#include "xBaseModel.h"
#include "../xcomdoc/xdocfstream.h"

BEGIN_NAMESPACE_XEVOL3D

xBaseModel::xBaseModel(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr , bool bSysMemCopy)
{
	m_pRenderApi = pRenderApi;
	m_pTexMgr    = pTexMgr;
	m_pSkeleton  = NULL;
	m_iAction    = 0;
	m_iActTime   = 0;
}

bool xBaseModel::load(const wchar_t* fileName , unsigned long  arg)
{
	return load(fileName , NULL , 0 , arg);
}

bool xBaseModel::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
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

bool xBaseModel::isLoaded()
{
	return m_Meshs.size() > 0;
}

bool xBaseModel::unload()
{
	for(size_t i = 0 ; i < m_Meshs.size() ; i ++)
	{
		m_Meshs[i].m_pMesh->unload();
		delete m_Meshs[i].m_pMesh;
	}
	m_Meshs.clear();
    
	for(size_t i = 0 ; i < m_Actions.size() ; i ++)
	{
		m_Actions[i]->unload();
		delete m_Actions[i];
	}
	m_Actions.clear();
	m_ActionFrame.unload();

	XSAFE_DELETE(m_pSkeleton);
	m_aabb.m_Max = xvec3(0.0f,0.0f,0.0f);
	m_aabb.m_Min = xvec3(0.0f,0.0f,0.0f);
	m_name = L"";
	return true;
}

unsigned long xBaseModel::memUsage()
{
	return 1;
}

bool xBaseModel::loadModelMeshMateril( xBaseModel::xModelMesh& mesh , xCfgNode* pMeshNode)
{
	return false;
}

bool xBaseModel::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
	//读入文件头
	ds_wstring descName = ds_wstring(_dir) + L"desc.xml";
	xcomdocstream* pdescstream = doc.open_stream(descName.c_str() );
	xcdstream  _in(pdescstream);
	xCfgDocument xml;
	xml.load(_in);
	_in.close();
	doc.close_stream(pdescstream);

	//XML加载完毕
	xCfgNode* pRootNode = xml.root();

	m_name = pRootNode->value(L"name");
	xCfgNode* pAABBNode = pRootNode->findNode(L"BoundBox");
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
    
	//开始加载Mesh
	loadEmbMeshs(doc , _dir , pRootNode);

	//加载SkinGroup
	loadEmbSkinGroup(doc , _dir , pRootNode); 

	//创建一个BoneFrameBuffer;
    createBoneFrameBuffer();
	return true;
}

bool xBaseModel::loadSkeleton(xcomdoc& doc , const wchar_t* _dir)
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
		xCfgDocument xml;
		xml.load(_in);
		_in.close();
        ds_wstring actDir = ds_wstring(_dir) + L"skeleton/";
        xCfgNode* pRootNode = xml.root();
		xCfgNode::CfgNodes actNodes;
		pRootNode->findNode(actNodes);
		for(size_t  i = 0 ;i < actNodes.size() ; i ++)
		{
			xCoreAction* pAction = new xCoreAction;
			pAction->m_nBone = m_pSkeleton->nBone();
			xCfgNode* pNode = actNodes[i];
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

bool xBaseModel::loadEmbMeshs(xcomdoc& doc , const wchar_t* _dir , xCfgNode* pRootNode)
{
	xCfgNode::CfgNodes meshNodes;
	pRootNode->findNode(L"mesh" , meshNodes);
	for(size_t i = 0 ; i < meshNodes.size() ; i ++)
	{
		xCfgNode* pMeshNode = meshNodes[i];
		const wchar_t* name = pMeshNode->value(L"name");
		xCoreMesh* pMesh = new xCoreMesh(m_pRenderApi,m_pTexMgr);
		ds_wstring meshName = ds_wstring(_dir) + L"/" + name + L"/";
		if( pMesh->load(doc , meshName.c_str(), 0) == false)
		{
			delete pMesh;
			continue;
		}
		xModelMesh mesh;
		mesh.m_pMesh = pMesh;
		loadModelMeshMateril(mesh,pMeshNode);
		m_Meshs.push_back(mesh);
	}
	return true;
}

bool xBaseModel::loadEmbSkinGroup(xcomdoc& doc , const wchar_t* _dir , xCfgNode* pRootNode)
{
	xCfgNode::CfgNodes skinGroupNodes;
	pRootNode->findNode(L"skinGroup" , skinGroupNodes);

	for(size_t i = 0 ; i < skinGroupNodes.size() ; i ++)
	{
		xCfgNode* pSkinGrouNode = skinGroupNodes[i];
		const wchar_t* skinGroupName = pSkinGrouNode->value(L"name");
		xCoreMesh* pMesh = new xCoreMesh(m_pRenderApi,m_pTexMgr);
		ds_wstring skinGroupDesName = ds_wstring(_dir) + L"/" + skinGroupName + L"/skins.xml";
		xcomdocstream* pdescstream = doc.open_stream(skinGroupDesName.c_str() );
		xcdstream  _in(pdescstream);
		xCfgDocument xml;
		xml.load(_in);
		_in.close();
		xCfgNode* pRootNode = xml.root();

		xCfgNode::CfgNodes meshNodes;
		pRootNode->findNode(L"mesh" , meshNodes);
		for(size_t i = 0 ; i < meshNodes.size() ; i ++)
		{
			xCfgNode* pMeshNode = meshNodes[i];
			const wchar_t* name = pMeshNode->value(L"name");
			xCoreMesh* pMesh = new xCoreMesh(m_pRenderApi,m_pTexMgr);
			ds_wstring meshName = ds_wstring(_dir) + L"/" + skinGroupName + L"/" + name + L"/" ;
			if( pMesh->load(doc , meshName.c_str(), 0) == false)
			{
				delete pMesh;
				continue;
			}
			xModelMesh mesh;
			mesh.m_pMesh = pMesh;
			loadModelMeshMateril(mesh,pMeshNode);
			m_Meshs.push_back(mesh);
		}

	}
	return true;
}

bool  xBaseModel::createBoneFrameBuffer()
{
      m_pBoneFrameBuffer = m_pRenderApi->findInputBuffer(L"BoneFrame");
	  if(m_pBoneFrameBuffer == NULL)
	  {
		  m_pBoneFrameBuffer = m_pRenderApi->createConstBuffer( sizeof(xvec4) * 1024); 
		  m_pRenderApi->setInputBuffer(L"BoneFrame" , m_pBoneFrameBuffer);
	  }
	  xmat4 matList[256];
	  for(int i  = 0 ; i < 256 ; i ++)
	  {
		  matList[i].identity();
	  }
	
	  if(m_pSkeleton)
	  {
		  m_ActionFrame.load(m_pSkeleton->nBone() );
	  }

	
	  void* pData = NULL;
	  m_pBoneFrameBuffer->lock(eLock_WriteDiscard , &pData);
	  memcpy(pData , matList , sizeof(xmat4) * 256);
	  m_pBoneFrameBuffer->unlock();

	  return true;
}

bool xBaseModel::setAction(size_t idx)
{
	 if(idx >= m_Actions.size() )
		 return false;

	 m_iActTime = 0;
     m_iAction  = idx;
	 return true;
}

bool xBaseModel::updateFrame(unsigned long passedTime)
{
	m_iActTime += passedTime;
	if(m_pSkeleton)
	{
		m_Actions[m_iAction]->blend( (long)m_iActTime,m_ActionFrame,m_pSkeleton);
	}
	return true;
}

bool xBaseModel::draw()
{
	if(m_pSkeleton)
	{
		void* pData = NULL;
		m_pBoneFrameBuffer->lock(eLock_WriteDiscard , &pData);
		memcpy(pData , m_ActionFrame.data() , sizeof(xmat4) * m_ActionFrame.nBone() );
		m_pBoneFrameBuffer->unlock();
	}

	for(size_t i = 0 ; i < m_Meshs.size() ; i ++)
	{
		m_Meshs[i].m_pMesh->draw();
	}
	return true;	
}

xGeomLib::xaabb&  xBaseModel::boundBox()
{
	return m_aabb;
}
END_NAMESPACE_XEVOL3D

