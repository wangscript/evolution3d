#include "../xStdPch.h"
#include "../xcomdoc/xdocfstream.h"
#include "xCoreMesh.h"
#include "xCoreSkeleton.h"
#include "xBaseModel.h"
#include "xCoreMeshMgr.h"
#include "../RenderAPI/xBaseShader.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xBaseModel  , IRenderApiResource);
IMPL_OBJECT_FACTORY_MGR(xBaseModel, xBaseModelCreator , xBaseModelCreatorMgr )
xBaseModel::xBaseModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy)
:IRenderOtherRes( NULL )
{
    m_RefCount   = 1;
	m_pRenderApi = pTexMgr->renderApi();
	m_pTexMgr    = pTexMgr;
	m_pSkeleton  = NULL;
	m_iAction    = 0;
	m_iActTime   = 0;
}

bool xBaseModel::load(const wchar_t* fileName , unsigned long  arg)
{
	return load(fileName , NULL , 0 , arg);
}

const wchar_t* xBaseModel::name()
{
    return m_name.c_str();
}

void xBaseModel::setName(const wchar_t* _name)
{
    m_name = _name;
}

bool xBaseModel::confirmMesh()
{
	size_t _nCoresMesh = m_hCoreMeshs.size() ;
     for(size_t i = 0 ; i < _nCoresMesh ;  i ++)
	 {
		 m_hCoreMeshs[i].confirm();
	 }
	 return true;
}

bool xBaseModel::isLoaded()
{
	return m_Meshs.size() > 0;
}

bool xBaseModel::unload()
{
	size_t _nMesh = m_Meshs.size() ;
	for(size_t i = 0 ; i < _nMesh ; i ++)
	{
		m_Meshs[i]->ReleaseObject();
	}
	m_Meshs.clear();
    
	size_t _nAction =  m_Actions.size();
	for(size_t i = 0 ; i < _nAction ; i ++)
	{
		m_Actions[i]->unload();
		delete m_Actions[i];
	}
	m_Actions.clear();
	m_ActionFrame.unload();


	//这个模型不需要了。 现在可以删除了。 同时要对hMesh做一次release
	size_t _nCoreMeshs = m_hCoreMeshs.size() ;
	for(size_t i = 0 ; i < _nCoreMeshs ;  i ++)
	{
		HCoreMesh hMesh = m_hCoreMeshs[i] ;
        hMesh.Release();
	}

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



bool xBaseModel::attachMesh(HCoreMesh hMesh)
{
	xCoreMesh* pMesh = hMesh.getResource();
	if(pMesh == NULL)
	{
         return false;
	}
	this->m_hCoreMeshs.push_back(hMesh);
	return attachMesh(pMesh);
}

bool xBaseModel::detachMesh(HCoreMesh hMesh)
{
	xCoreMesh* pMesh = hMesh.getResource();
	return detachMesh(pMesh);

}

bool xBaseModel::attachMesh(xCoreMesh* pMesh)
{
	if(pMesh == NULL)
		return false;

     m_Meshs.push_back(pMesh);
	 return true;
}

bool xBaseModel::detachMesh(xCoreMesh* pMesh)
{
	//首先,如果这个pMesh是属于某个HMESH的,就给它连HMESH一起删除掉
	vHCoreMeshs::iterator pos = m_hCoreMeshs.begin();
	for(; pos != m_hCoreMeshs.end() ; pos ++)
	{
		HCoreMesh hMesh = *pos;
		if(hMesh.getResource() == pMesh)
		{
			m_hCoreMeshs.erase(pos);
			break;
		}
	}

	//删除Mesh
	vCoreMeshs::iterator pos2 = m_Meshs.begin();
	for( ; pos2 != m_Meshs.end() ; pos2 ++)
	{
		if(*pos2 == pMesh)
		{
			m_Meshs.erase(pos2);
			return true;
		}
	}
	return false;
}

bool  xBaseModel::createBoneFrameBuffer()
{
      m_pBoneFrameBuffer = m_pRenderApi->findInputBuffer(L"BoneFrame");
      if(m_pBoneFrameBuffer )
          return true;

      m_pBoneFrameBuffer = m_pRenderApi->createConstBuffer( sizeof(xvec4) * 1024); 
	  m_pRenderApi->setInputBuffer(L"BoneFrame" , m_pBoneFrameBuffer);
	
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

xBaseAction*  xBaseModel::coreAction(size_t idx )
{
	return m_Actions[idx];
}

size_t xBaseModel::nAction()
{
    return m_Actions.size();
}

xCoreActionFrame*  xBaseModel::createActionFrame()
{
	if(m_pSkeleton)
	{
		xCoreActionFrame* pActFrame = new xCoreActionFrame;
		pActFrame->load(m_pSkeleton->nBone() );
        return pActFrame;
	}
	return NULL;
}

void  xBaseModel::destoryActionFrame(xCoreActionFrame* pAcFrame)
{
	delete pAcFrame;
}

bool  xBaseModel::updateFrame(xCoreActionFrame* pAcFrame, int actIdx , unsigned long actTime)
{
	if(m_pSkeleton)
	{
		m_Actions[actIdx]->blend( (long)actTime,pAcFrame,m_pSkeleton);
		return true;
	}
	return false;
}

bool xBaseModel::updateFrame(unsigned long passedTime)
{
	m_iActTime += passedTime;
	if(m_pSkeleton)
	{
		m_Actions[m_iAction]->blend( (long)m_iActTime, &m_ActionFrame,m_pSkeleton);
		return true;
	}
	return true;
}

bool xBaseModel::setupAnimationTime(int actIdx , float actTime)
{
    if(actIdx >= (int)m_Actions.size() )
		return false;
	if(m_pSkeleton)
		return true;

	xBaseAction* _action = m_Actions[actIdx];
    const xActionInfo* pActInfo = _action->info();
	float frameTime = pActInfo->m_lTime /(float) pActInfo->m_nFrame;
	int frame = (int)(actTime/frameTime);
	frame = frame % pActInfo->m_nFrame;

	size_t _nMeshs = m_Meshs.size();
	for(size_t i = 0 ; i <  _nMeshs; i ++)
	{
		m_Meshs[i]->setCurrentFrame(frame);
	}

	_nMeshs = m_hCoreMeshs.size();
	for(size_t i = 0 ; i <  _nMeshs; i ++)
	{
		HCoreMesh hMesh = m_hCoreMeshs[i];
		if(hMesh.getResource() )
		{
			hMesh->setCurrentFrame(frame);
		}
	}
	return true;
}

bool xBaseModel::setupAnimation(xCoreActionFrame* pAcFrame)
{
	if(m_pSkeleton && m_pBoneFrameBuffer)
	{
		void* pData = NULL;
		m_pBoneFrameBuffer->lock(eLock_WriteDiscard , &pData);
		memcpy(pData , pAcFrame->data() , sizeof(xmat4) * pAcFrame->nBone() );
		m_pBoneFrameBuffer->unlock();
		return true;
	}
	return false;
}

bool xBaseModel::setupAnimation()
{
	return setupAnimation(&m_ActionFrame);
}

size_t xBaseModel::nMeshPtr()
{
	return m_Meshs.size();
}

size_t xBaseModel::nMeshHandle()
{
	return m_hCoreMeshs.size();
}

xCoreMesh* xBaseModel::getMesh(size_t idx)
{
    size_t nMeshHandle = m_hCoreMeshs.size();
	if(idx < nMeshHandle )
		return m_hCoreMeshs[idx].getResource() ;

	idx -= nMeshHandle;
	if(idx < m_Meshs.size() )
		return m_Meshs[idx];
	return NULL;
}

HCoreMesh xBaseModel::getMeshHandle(size_t idx)
{
	size_t nMeshHandle = m_hCoreMeshs.size();
	if(idx < nMeshHandle )
		return m_hCoreMeshs[idx] ;
	HCoreMesh ret;
	return ret;
}

xCoreMesh* xBaseModel::getMeshPtr(size_t idx)
{
	size_t nMeshPtr =  m_Meshs.size();
	if(idx < nMeshPtr )
		return m_Meshs[idx];
	return NULL;
}

size_t  xBaseModel::nMesh()
{
	size_t nMeshPtr    = m_Meshs.size();
	size_t nMeshHandle = m_hCoreMeshs.size() ;
	return nMeshPtr + nMeshHandle;
}

bool  xBaseModel::drawMeshs(xCoreActionFrame* pActionFrame)
{
    if(pActionFrame == NULL) pActionFrame = &m_ActionFrame;
	size_t _nMeshs = m_Meshs.size();
	for(size_t i = 0 ; i <  _nMeshs; i ++)
	{
		m_Meshs[i]->draw(m_pSkeleton , pActionFrame);
	}

	_nMeshs = m_hCoreMeshs.size();
	for(size_t i = 0 ; i <  _nMeshs; i ++)
	{
		HCoreMesh hMesh = m_hCoreMeshs[i];
		if(hMesh.getResource() )
		{
			hMesh->draw(m_pSkeleton , pActionFrame);
		}
	}
	return true;
}

bool  xBaseModel::draw(xCoreActionFrame* pActionFrame)
{
    setupAnimation(pActionFrame);
	return drawMeshs(pActionFrame);
}

xGeomLib::xaabb&  xBaseModel::boundBox()
{
	return m_aabb;
}

END_NAMESPACE_XEVOL3D

