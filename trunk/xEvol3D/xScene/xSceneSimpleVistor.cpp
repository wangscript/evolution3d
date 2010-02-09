#include "../xStdPch.h"
#include "xSceneSimpleVistor.h"
#include "xSceneNode.h"
#include "xSceneDrawable.h"
#include "xBaseTerrain.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xSceneRenderVisitor , ISceneVisitor);
xSceneRenderVisitor::xSceneRenderVisitor(IBaseRenderer* pRenderer,IRenderCamera* pCamera)
{
	m_RefCount  = 1;
    m_pRenderer = pRenderer;
	m_pCamera   = pCamera;
}

bool xSceneRenderVisitor::visit(ISceneNode* pNode , ISceneGraph* pScne) 
{
	size_t nObject = pNode->nObjects();
	for(size_t i = 0 ;  i < nObject ; i ++)
	{
		ISceneObject* pObject = pNode->getObject(i);
		ISceneDrawable* pDrawable = dynamic_cast<ISceneDrawable*>(pObject);
		if(pDrawable)
		{
			pDrawable->draw(m_pRenderer , m_pCamera );
		}		
	}
	return true;
}

//=============================
IMPL_BASE_OBJECT_DLLSAFE(xSceneColorSelectVisitor , ISceneVisitor);
xSceneColorSelectVisitor::xSceneColorSelectVisitor(IBaseRenderer* pRenderer,IRenderCamera* pCamera)
{
    m_pRenderer = pRenderer;
	m_pCamera   = pCamera;
}

bool xSceneColorSelectVisitor::visit(ISceneNode* pNode , ISceneGraph* pScne) 
{
	size_t nObject = pNode->nObjects();
	for(size_t i = 0 ;  i < nObject ; i ++)
	{
		ISceneObject*   pObject = pNode->getObject(i);
		ISceneDrawable* pDrawable = dynamic_cast<ISceneDrawable*>(pObject);
		if(pDrawable)
		{
			m_pRenderer->renderApi()->colorSelector()->setRenderObjectID( (int)pNode , (int)pObject );
			if(pDrawable)
			{
				pDrawable->draw(m_pRenderer , 0 , m_pCamera);
			}
		}
	}
	return true;
}
//================用来查询地形的高度的==============================
IMPL_BASE_OBJECT_DLLSAFE(xTerrainHeightSelVisitor , ISceneVisitor);
xTerrainHeightSelVisitor::xTerrainHeightSelVisitor(IBaseRenderer* pRenderer, IRenderCamera* pCamera)
{
	m_pRenderer = pRenderer;
	m_pCamera   = pCamera;
	m_bQueue    = false;
}

bool xTerrainHeightSelVisitor::visit(ISceneNode* pNode , ISceneGraph* pScne) 
{
	size_t nObject = pNode->nObjects();
	for(size_t i = 0 ;  i < nObject ; i ++)
	{
		if(pNode->classID().isType( xTerrainBase::ms_CLASSID ) )
		{
			ISceneObject*   pObject = pNode->getObject(i);
			ISceneDrawable* pDrawable = dynamic_cast<ISceneDrawable*>(pObject);
			if(pDrawable)
			{
				m_pRenderer->renderApi()->colorSelector()->setRenderObjectID( (int)pNode , (int)pObject );
				if(pDrawable)
				{ 
					if(m_bQueue)
						pDrawable->draw(m_pRenderer , m_pCamera);  //放到队列去。
					else
					    pDrawable->draw(m_pRenderer , 0 , m_pCamera); //直接渲染
				}
			}
		}
	}
	return true;
}


//===============Update Visistor====================================
IMPL_BASE_OBJECT_DLLSAFE(xSceneUpdateVisitor , ISceneVisitor);
xSceneUpdateVisitor::xSceneUpdateVisitor()
{
    m_PassedTime = 0;
}

bool  xSceneUpdateVisitor::setFrameTime(unsigned long passedTime)
{
	m_PassedTime = passedTime;
	return true ;
}

bool xSceneUpdateVisitor::visit(ISceneNode* pNode , ISceneGraph* pScne) 
{
	pNode->updateFrame(m_PassedTime);
	return true;
}
END_NAMESPACE_XEVOL3D
