#include "vrcSceneNode.h"
#include "vrcSceneManager.h"
#include "vrcModelNode.h"
#include "vrcDummyNode.h"

class xVR_SceneNodeFactoryMgr
{
	vector<xVR_SceneNodeFactory*> m_vFactorys;
public:
	xVR_SceneNode* createNodes(const wchar_t* _typeName, const wchar_t* _Name,  XVR_SceneManager* pSceneMgr)
	{
		for(size_t  i = 0 ; i < m_vFactorys.size() ; i ++)
		{
			xVR_SceneNodeFactory* pFactory = m_vFactorys[i];
			if(pFactory->isNodeTypeSupport(_typeName) )
				return pFactory->createNode(_typeName , _Name , pSceneMgr);
		}
		return NULL;
	}
	bool           addFactory(xVR_SceneNodeFactory* _p)
	{
		m_vFactorys.push_back(_p);
		return true;
	}

	bool           removeFactory(xVR_SceneNodeFactory* _p)
	{
		for(vector<xVR_SceneNodeFactory*>::iterator pos =m_vFactorys.begin() ; pos != m_vFactorys.end() ; pos ++)
		{
			if(*pos == _p){ m_vFactorys.erase(pos) ; return true; }
		}
		return false;
	}

};
IMPL_BASE_OBJECT_CLASSID(XVR_SceneManager , xVR_SceneNode);

xVR_SceneNodeFactoryMgr* g_FactoryMgr = NULL;
bool XVR_SceneManager::registeNodeFactory(xVR_SceneNodeFactory* pFactry)
{
    if(g_FactoryMgr == NULL) g_FactoryMgr = new xVR_SceneNodeFactoryMgr;
	return g_FactoryMgr->addFactory(pFactry);
}

bool XVR_SceneManager::unregisteNodeFactory(xVR_SceneNodeFactory* pFactory)
{
   if(g_FactoryMgr) 
	   return g_FactoryMgr->removeFactory(pFactory);
   return false;
}

XVR_SceneManager*   XVR_SceneManager::createSceneManager(const wchar_t* scenManer , IRenderApi* pRenderApi)
{
    return new XVR_SceneManager(pRenderApi);
}

XVR_SceneManager::XVR_SceneManager(IRenderApi* pRenderApi) : xVR_SceneNode(this)
{
	m_pRenderApi = pRenderApi;
	m_pTextureManager = m_pRenderApi->createTextureManager(L"XVRSceneTexMgr");
	m_pModelMager = xBaseModelMgr::createInstance(pRenderApi , m_pTextureManager , L"XVRSceneModelMgr");  
	m_ZStencil   = m_pRenderApi->createDepthStencilState(L"Default");
}

XVR_SceneManager::~XVR_SceneManager()
{
}

void XVR_SceneManager::destory()
{

}

xVR_SceneNode* XVR_SceneManager::createNode(const wchar_t* _typeName  , const wchar_t* _Name , xXmlNode* xmlNode , xVR_SceneNode* pParentNode)
{
	xVR_SceneNode* pNode = g_FactoryMgr->createNodes(_typeName , _Name , this);
	if(pNode && pNode->load(xmlNode) )
	{
		if(pParentNode)
		{
			pNode->m_pParent = pParentNode;
			pParentNode->insertChild(pNode);
		}
		else
		{
			m_Children.push_back(pNode);
			pNode->m_pParent = NULL;
		}
		
	}
	return pNode;
}


void XVR_SceneManager::deleteNode(xVR_SceneNode* pNode)
{
	xVR_SceneNode* pParentNode = pNode->getParent();
	pParentNode->removeChild(pNode);
	pNode->ReleaseObject();	
}

bool XVR_SceneManager::update(unsigned long passedTime)
{
	for(int i = 0 ; i < (int)m_Children.size() ; i++)
	{
		m_Children[i]->update(passedTime);
	}
	return true;
}

bool        XVR_SceneManager::render(unsigned long passedTime)
{
	IDepthStencilState* pOldStencil = m_pRenderApi->getDepthStencilState();
	m_pRenderApi->setDepthStencilState(m_ZStencil);
	for(int i = 0 ;  i < (int)m_Children.size() ; i++)
	{
		m_Children[i]->render(passedTime);
	}
	m_pRenderApi->setDepthStencilState(pOldStencil);
	return true;
}

xVR_SceneNode* XVR_SceneManager::getSelected(int x , int y )
{
	xvec2i _id;
	m_pRenderApi->beginSelectMode();
	m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.0f,0.0f));
	m_pRenderApi->beginScene();
	render(0);
	m_pRenderApi->endScene();
	m_pRenderApi->swapBuffer();
	m_pRenderApi->endSelectMode(x,y , &_id.x);
	xVR_SceneNode* ret= (xVR_SceneNode*)_id.x;
	return ret;
}

HBaseModel XVR_SceneManager::addModel(const wchar_t * file)
{
    return m_pModelMager->add(file,0,true);
}
bool XVR_SceneManager::load(xXmlNode* pNode)
{
	return xVR_SceneNode::load(pNode);
}
bool XVR_SceneManager::save(xXmlNode* pXMLNode)
{
	return xVR_SceneNode::save(pXMLNode);
}

bool XVR_SceneManager::load(const wchar_t* sceneFile)
{
     xXmlDocument doc;
	 doc.load(sceneFile,true);
	 xXmlNode* pRootNode = doc.root();
	 if(pRootNode == NULL)
		 return NULL;
     return load(pRootNode);
}

bool XVR_SceneManager::save(const wchar_t* sceneFile)
{
	xXmlDocument doc;
	xXmlNode* pRootNode = doc.root( type() );
	if(pRootNode == NULL)
		return false;
	save(pRootNode);
	doc.save(sceneFile);
	return true;
}

class xVR_DefualtNodeFactory : public xVR_SceneNodeFactory
{
public:
    xVR_DefualtNodeFactory()
	{
		XVR_SceneManager::registeNodeFactory(this);
	}
	~xVR_DefualtNodeFactory()
	{
		XVR_SceneManager::unregisteNodeFactory(this);
	}

	xVR_SceneNode* createNode(const wchar_t* _type , const wchar_t* _name , XVR_SceneManager* pSceneMgr)
	{
		if(_type == wstring(L"ModelNode"))
		{
			xVR_SceneNode* pNode = new xVR_ModelNode(pSceneMgr);
			pNode->setname(_name);
			return pNode;
		}
		else if(_type == wstring(L"ModelNode"))
		{
			xVR_SceneNode* pNode = new xVR_ModelNode(pSceneMgr);
			pNode->setname(_name);
			return pNode;
		}
		return NULL;		
	}

	bool isNodeTypeSupport(const wchar_t* _type)
	{
		if(_type == wstring(L"ModelNode"))
		{
			return true;
		}
		else if(_type == wstring(L"ModelNode"))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

static xVR_DefualtNodeFactory g_DefualtFactory;