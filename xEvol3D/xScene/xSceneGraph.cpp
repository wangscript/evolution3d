#include "../xStdPch.h"
#include "xSceneGraph.h"
#include "xSceneVisitor.h"

#include "xSceneModel.h"

#include "Renderer/xRenderer.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(ISceneGraph     , xSceneBasicNode)
IMPL_BASE_OBJECT_DLLSAFE(xBasicScene     , ISceneGraph);
IMPL_OBJECT_FACTORY_MGR(ISceneGraph      , xSceneCreator , xSceneFactoryMgr )

ISceneGraph::ISceneGraph(ISceneGraph*  pScene , ISceneNode* pParent )
:xSceneBasicNode(pScene , pParent )
{
	m_pResPathMgr = NULL;
	m_pRenderer   = NULL;
	if(pScene == NULL )
	{
		m_pScene    = this;
	}
}

ISceneGraph::~ISceneGraph()
{
    unload();
} 

ISceneGraph* ISceneGraph::sceneGraph()
{
	if(m_pScene == NULL)
		return this;
	return m_pScene;
}

IResManager* ISceneGraph::findResManager(const wchar_t* _name)
{
     int _iName = xStringHash(_name);
	 mapResManager::iterator pos = m_vResManagers.find(_iName);
	 if(pos == m_vResManagers.end() )
		 return NULL;
	 return pos->second;
}

bool ISceneGraph::unload()
{
	return xSceneBasicNode::unload();
}

bool ISceneGraph::registeResManager(IResManager* _pResManager)
{
	const wchar_t* _name =  _pResManager->name();
	if(findResManager( _name ) )
		return false;
	
	 int _iName = xStringHash(_name);
	 m_vResManagers[_iName] = _pResManager;
	 return true;
}

xBaseModelMgr* ISceneGraph::getModelManager()
{
	return m_pModelMgr;
}

bool ISceneGraph::setModelManager(xBaseModelMgr* pMgr)
{
	m_pModelMgr = pMgr;
	registeResManager(pMgr);
	return m_pModelMgr != NULL;
}

xBaseTextureMgr* ISceneGraph::getTextureManger()
{
	return m_pTextureMgr;
}

bool ISceneGraph::setTextureManager(xBaseTextureMgr* pTextureManagerpMgr)
{
	m_pTextureMgr = pTextureManagerpMgr;
	registeResManager(pTextureManagerpMgr);
	return m_pTextureMgr != NULL;
}

void ISceneGraph::setRenderer(IBaseRenderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

IBaseRenderer* ISceneGraph::renderer()
{
    return m_pRenderer;
}

bool ISceneGraph::save(const wchar_t * fileName )
{
	xXmlDocument doc;
	xXmlNode* pXml = doc.insertNode(L"Scene");
	if(save(pXml))
	{
		doc.save(fileName,true);
		return true;
	}
	doc.unload();
	return false;
}
bool ISceneGraph::save(xXmlNode* pNode)
{
	return xSceneBasicNode::save(pNode);
}

bool ISceneGraph::load(xXmlNode* pNode)
{
	return xSceneBasicNode::load(pNode);
}

//-----------------------------------------------------------
//
//===========================================================
xBasicScene::xBasicScene(ISceneGraph* pScene , ISceneNode* pParent  )
:ISceneGraph(pScene , pParent )
{
	m_TypeName = L"xBasicScene";
}

xBasicScene::~xBasicScene()
{

}

bool xBasicScene::load(xXmlNode* pNode)
{
	//Load thing right here.
	return ISceneGraph::load(pNode);
}

bool xBasicScene::save(xXmlNode* pXml)
{
	if(pXml == NULL)
		return false;
    pXml->setValue(L"type" , L"BasicScene");
	beginEnumChildren();
	ISceneNode* pChildNode = NULL;
	while(pChildNode = nextChild() )
	{
		xXmlNode* pChildXml = pXml->insertNode(L"child");
		pChildNode->save(pChildXml);
	}
	endEnumChildren();
	return true;
}

bool xBasicScene::visit(ISceneVisitor* pVisitor , eVisitOrder _order, IRenderCamera* pCamera )
{
    if     (_order == eVO_PreOrder ) return _visit_preorder (this , pVisitor , pCamera);
	else if(_order == eVO_PostOrder) return _visit_postorder(this , pVisitor , pCamera); 
	return false;
}

bool xBasicScene::_visit_preorder(ISceneNode* pSceneNode , ISceneVisitor* pVisitor , IRenderCamera* pCamera)
{
    if(pCamera != NULL)
	{
		XEVOL_WARNNING_NOT_IMPLEMENT_INFO(L"Please do frustum culling here\n");
	}
	//前续遍历
	if(true == pVisitor->visit(pSceneNode , this) )
	{
		pSceneNode->beginEnumChildren();
		ISceneNode* pChild = NULL;
		while(pChild = pSceneNode->nextChild() )
		{
			if(pChild->isVisible() == false)
				continue;
			// 如果是SG，则直接执行SG的
			ISceneGraph* pSG = dynamic_cast<ISceneGraph*>(pChild);
			if ( pSG  )
			{
				pSG->visit(pVisitor, eVO_PreOrder , pCamera);
			}
			else
			{
				_visit_preorder(pChild , pVisitor , pCamera);
			}
		}
		pSceneNode->endEnumChildren();
	}

	
	return true ;
}

bool xBasicScene::_visit_postorder(ISceneNode* pSceneNode , ISceneVisitor* pVisitor ,  IRenderCamera* pCamera )
{
	if(pCamera != NULL)
	{
		XEVOL_WARNNING_NOT_IMPLEMENT_INFO(L"Please do frustum culling here\n");
	}

	//后续遍历
	pSceneNode->beginEnumChildren();
	ISceneNode* pChild = NULL;
	while(pChild = pSceneNode->nextChild() )
	{
		if(pChild->isVisible() == false)
			continue;
		// 如果是SG，则直接执行SG的
		ISceneGraph* pSG = dynamic_cast<ISceneGraph*>(pChild);
		if ( pSG )
		{
			pSG->visit(pVisitor, eVO_PostOrder , pCamera);
		}
		else
		{
			_visit_postorder(pChild , pVisitor , pCamera);
		}
	}
	pSceneNode->endEnumChildren();

	pVisitor->visit(pSceneNode , this);
	return true ;
}

bool xBasicScene::updateFrame(unsigned long passedTime, bool bRecursive)
{
	return ISceneNode::updateFrame(passedTime , bRecursive);
}



IMPL_SCENENODE_FACTORY(xBasicScene);
IMPL_SCENE_FACTORY(xBasicScene     ,  L"BasicScene" );






ISceneGraph*    XEvol_CreateScene(const wchar_t* _sceneFile , const xSceneInitArg& arg)
{
      xXmlDocument doc;
	  if(doc.load(_sceneFile) == false )
	  {
		  XEVOL_LOG(eXL_DEBUG_TIPS , L"Load scene xml failed : %s\n" , _sceneFile);
		  return NULL;
	  }

	  xXmlNode* pRootNode = doc.root();
	  const wchar_t* _type =  pRootNode->value(L"type");
	  if( _type != NULL)
	  {
		  ISceneGraph* pScene = xSceneFactoryMgr::singleton()->createInstance( _type , NULL , NULL );
		  if(pScene == NULL)
		  {
			  XEVOL_LOG(eXL_DEBUG_NORMAL , L"Scene type not registed type=%s\n" , _type);
			  return NULL;
		  }

		  pScene->setRenderer(arg.m_pRenderer);
		  pScene->setModelManager(arg.m_pModelMgr);
		  pScene->setTextureManager(arg.m_pTextureMgr);
		  pScene->setPathManager(arg.m_pResPathMgr);

		  if(false ==  pScene->load(pRootNode) )
		  {
			  pScene->ReleaseObject();
			  return NULL;
		  }

		  //更新所有子节点的变换矩阵。
		  pScene->updateChildrenTrans();
		  return pScene;
	  }
	  return NULL;
}
END_NAMESPACE_XEVOL3D
