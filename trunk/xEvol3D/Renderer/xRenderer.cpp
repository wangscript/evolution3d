#include "../xStdPch.h"
#include "xRenderer.h"
#include "xMaterial.h"
#include "xRendererQueue.h"
#include "BaseLib/xStringHash.h"
#include <map>
using namespace std;

BEGIN_NAMESPACE_XEVOL3D

DECL_OBJECT_FACTORY_MGR(IBaseRenderer,IRendererCreator , xBaseRenderFactoryMgr , IRenderApi* , int);
IMPL_OBJECT_FACTORY_MGR(IBaseRenderer,IRendererCreator , xBaseRenderFactoryMgr);

IMPL_BASE_OBJECT_DLLSAFE(IBaseRenderer , IBaseObject)
IMPL_BASE_OBJECT_DLLSAFE(IRenderTech   , IRenderApiObject)


class  xRenderQueuePtrCompare
{
public:
	bool operator()(xRendererQueue* lhv, xRendererQueue* rhv)
	{
		return  *lhv < *rhv;
	}
};

IBaseRenderer::IBaseRenderer(IRenderApi* pApi) : m_pRenderApi(pApi)
{
    m_pCallback = NULL;
}

xRendererQueue*  IBaseRenderer::__findQueue(int _idName) const
{
	int _id = _idName;
	xRendererQueueMaps::const_iterator pos = m_mapQueues.find(_id);
	if(pos == m_mapQueues.end() ) return NULL;
	return pos->second;
}

xRendererQueue* IBaseRenderer::__findQueue(const wchar_t* rqName) const 
{
	int id = xStringHash(rqName);
	return __findQueue(id);
}


bool IBaseRenderer::loadConfig(const wchar_t* xmlName , const int8* xmlData , int len)
{
	xXmlDocument xml;
	bool bLoaded = false;
	if(xmlData == NULL)
		bLoaded = xml.load(xmlName);
	else
		bLoaded = xml.load(xmlData , len);
	if(bLoaded == false)
		return false;

	xXmlNode* pRootNode = xml.root() ? xml.root()->findNode(this->name()) : NULL;
	if(pRootNode == NULL)
		return false;

	xXmlNode* pQueRootNode = pRootNode->findNode(L"Queues");
	if(pQueRootNode)
	{
		xXmlNode::XmlNodes nodes ;
		pQueRootNode->findNode(L"Queue" , nodes);
		size_t nNodes = nodes.size() ;
		for(size_t i = 0 ; i < nNodes ; i ++)
		{
			xXmlNode* pQueNode = nodes[i];
			xRendererQueue* pQue = this->createQueue(pQueNode);
			m_vQueues.push_back(pQue);
			m_mapQueues.insert(xRendererQueueMaps::value_type( pQue->idName() , pQue));
		}

		std::sort(m_vQueues.begin() , m_vQueues.end() , xRenderQueuePtrCompare() );
	}
	return true;
}

xRendererQueue*   IBaseRenderer::createQueue(xXmlNode* pCfg)
{
      xRendererQueue* pRet = new xRendererQueue();
	  IRenderObjectCmp* pCmp = xRenderObjCmpFactoryMgr::singleton()->createInstance(pCfg->value(L"compare") , 0  , 0 ) ;
	  if(pCmp)
	  {
		  pRet->setComparer(pCmp);
	  }
	  pRet->setName(pCfg->value(L"name"));
	  int priority = pCfg->int_value(L"priority");
	  pRet->setPriority(priority);
	  return pRet;
}

size_t IBaseRenderer::nRenderQueue()  const 
{
	return m_vQueues.size();
}

xRendererQueue* IBaseRenderer::RenderQueueByIndex(size_t iQueue)  const 
{
	if(iQueue < 0 || iQueue >= (int)m_vQueues.size() ) 
		return NULL;

	return m_vQueues[iQueue];
}

xRendererQueue* IBaseRenderer::RenderQueueByName(int QueueName)  const 
{
	return __findQueue(QueueName);
}

xRendererQueue* IBaseRenderer::RenderQueueByName(const wchar_t* QueueName) const 
{
	return __findQueue(QueueName);
}

IRendererEventCallback*  IBaseRenderer::getEventCallBack()
{
    return m_pCallback;
}

bool IBaseRenderer::setEventcallback(IRendererEventCallback* pCallback)
{
    m_pCallback = pCallback;
	return true;
}


xRendererManager* xRendererManager::singleton()
{
	static xRendererManager g_RM;
	return &g_RM;
}

bool xRendererManager::registeRenderer(IRendererCreator* pRendererCreator)
{
	return xBaseRenderFactoryMgr::singleton()->registe(pRendererCreator);
}

bool xRendererManager::unregisteRenderer(IRendererCreator* pRendererCreator)
{
	return xBaseRenderFactoryMgr::singleton()->unregiste(pRendererCreator);
}

xRendererManager::xRendererManager()
{
	xBaseRenderFactoryMgr* pFactoryMgr = xBaseRenderFactoryMgr::singleton();
	pFactoryMgr->find(L"");
}

IBaseRenderer* xRendererManager::createRenderer(const wchar_t* name , IRenderApi* pRenderApi)
{
	return xBaseRenderFactoryMgr::singleton()->createInstance(name , pRenderApi , 0);
}




END_NAMESPACE_XEVOL3D
