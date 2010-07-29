#include "../xStdPch.h"
#include "xSceneNode.h"
#include "xSceneDrawable.h"
#include "xSceneGraph.h"
#include "xScenePropertyTool.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_REFCOUNT_OBJECT_FUNCTION(ISceneNodePropertySet);
IMPL_BASE_OBJECT_DLLSAFE(ISceneNode                ,  IBaseObject);
IMPL_OBJECT_FACTORY_MGR(ISceneNode                 ,  xSceneNodeCreator , xSceneNodeFactoryMgr )
xMathLib::xmat4  ISceneNode::m_idTransMat(1.0f);
ISceneNode::ISceneNode(ISceneGraph*  pScene , ISceneNode* pParent)
{
	m_bInTheScene     = true;
	m_pParent         = pParent;
	m_pScene          = pScene;
	m_bVisible        = true;
	m_bPropertySetInited = false;
	m_pPropertySet    = NULL;
	m_TypeName = L"";
}

bool ISceneNode::bInScene()
{
	return m_bInTheScene;
}

bool ISceneNode::isVisible()
{ 
	return m_bVisible;
}

void ISceneNode::setVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

ISceneNodePropertySet*  ISceneNode::getPropertySet()
{
	return m_pPropertySet;
}

bool ISceneNode::initPropertySet()
{
	 //{ 
	 //	 ISceneNodePropertySet* pPropertySet = getPropertySet();
	 //	 xPropertyItem* pPropertyItem = NULL; 
	 //  pPropertyItem = getPropertySet()->add_property(L"可见性" , new TPropertyValueRef<bool>(m_bVisible , 0) );
	 //	 pPropertyItem->add_attribute(L"data_type"    , L"bool");
	 //	 pPropertyItem->add_attribute(L"control_type" , L"checkbox");
	 //	 pPropertyItem = NULL; 
	 // }

	class xPlacementChangeHandler : public xPropertyItem::xEventHanlder
	{
	public:
		bool onChange(void* pUserData)
		{
			ISceneNode* pNode = (ISceneNode*)pUserData;
			pNode->invalidatePlacement();
			return true;
		}
	};
	static xPlacementChangeHandler g_PlacementChangeHandler;

	DECL_ADD_SCENE_PROPERTY_REF_STRING(L"名字" , std::ds_wstring , m_Name);
	ADD_DESCRIPTOR(L"data_type"    , L"string");
	ADD_DESCRIPTOR(L"control_type" , L"edit");
	END_ADD_SCENE_PROPERTY(L"名字");

	DECL_ADD_SCENE_PROPERTY_REF(L"可见性" , bool , m_bVisible);
	ADD_DESCRIPTOR(L"data_type"    , L"bool");
	ADD_DESCRIPTOR(L"control_type" , L"checkbox");
	END_ADD_SCENE_PROPERTY(L"可见性");


	DECL_ADD_SCENE_PROPERTY_REF(L"位置" , xPlacement , m_Placement);
	ADD_DESCRIPTOR(L"data_type"    , L"placement");
	ADD_EVENTHANLDER(&g_PlacementChangeHandler , (void*)this);
	ADD_DESCRIPTOR(L"control_type" , L"placement");
	END_ADD_SCENE_PROPERTY(L"位置");

	return true;
}

void ISceneNode::setInScene(bool _bInScene)
{
	m_bInTheScene = _bInScene;
}

ISceneNode::~ISceneNode()
{
	XSAFE_RELEASEOBJECT(m_pPropertySet);
}

void ISceneNode::attachToScene(ISceneGraph* pScene)
{
	m_pScene = pScene;
}

IBaseRenderer* ISceneNode::renderer()
{
	return m_pScene->renderer();
}

const wchar_t* ISceneNode::name()
{
	return m_Name.c_str();
}

void ISceneNode::setName(const wchar_t* _name)
{
	if(_name == NULL)
	{
		XEVOL_LOG(eXL_DEBUG_TIPS , L"ISceneNode::setName(NULL)\n");
		return ;
	}
	m_Name = _name;
}

xMathLib::xPlacement* ISceneNode::placement()
{
	return &m_Placement;
}

bool ISceneNode::loadPlacement(xXmlNode* pNode)
{
	const wchar_t* _pos    = pNode->value(L"pos");
	const wchar_t* _scale  = pNode->value(L"scale");
	const wchar_t* _rotate = pNode->value(L"rotate");
	if(_pos)
	{
	    xvec3 vPos ;
		swscanf(_pos , L"[%f,%f,%f]" , &vPos.x , &vPos.y , &vPos.z );
		m_Placement.setPosition(vPos);
	}

	if(_scale)
	{
		xvec3 vScale ;
		swscanf(_scale , L"[%f,%f,%f]" , &vScale.x , &vScale.y , &vScale.z );
		m_Placement.setScale(vScale);
	}

	if(_rotate)
	{
		xvec3 vRat ;
		float angle = 0.0f;
		swscanf(_rotate , L"[%f,%f,%f,%f]" , &vRat.x , &vRat.y , &vRat.z  , &angle);
		m_Placement.setRotate(vRat , angle);
	}
    
    m_Placement.validate();
	bool ret =  _rotate || _scale || _pos;
	if(ret && m_pParent == NULL)
	{
         ///Root node;
		m_trans = *m_Placement.toMatrix();
	}
	return ret;
}

bool ISceneNode::savePlacement(xXmlNode* pNode)
{
	const wchar_t* _pos    = pNode->value(L"pos");
	const wchar_t* _scale  = pNode->value(L"scale");
	const wchar_t* _rotate = pNode->value(L"rotate");
	wchar_t _buf[256] = {0};

	xvec3 vPos ;
	m_Placement.getPosition(vPos);
	swprintf_x(_buf , 256 , L"[%f,%f,%f]" , vPos.x , vPos.y , vPos.z);
	pNode->setValue(L"pos" , _buf);

	xvec3 vScale ;
	m_Placement.getScale(vScale);
	swprintf_x(_buf , 256 , L"[%f,%f,%f]" , vScale.x , vScale.y , vScale.z);
	pNode->setValue(L"scale" , _buf);

	xvec3 vRat ;
	float angle = 0.0f;
	m_Placement.getRotate(vRat , angle);
	swprintf_x(_buf , 256 , L"[%f,%f,%f,%f]" , vRat.x , vRat.y , vRat.z , angle);
	pNode->setValue(L"rotate" , _buf);

	return true;
}
ISceneNode* ISceneNode::getParent()
{
	return m_pParent;
}
void ISceneNode::setParent(ISceneNode* pNode )
{
	ISceneNode* pParent = m_pParent;
    m_pParent = pNode;
	if(pParent && m_pParent == NULL)
	{
		pParent->removeChild(this);		
	}
}


const xmat4&  ISceneNode::tansMatrix()
{
	return m_trans;
}

bool  ISceneNode::invalidatePlacement()
{
    m_Placement.invalidate();
	return this->updateChildrenTrans();
}

bool  ISceneNode::updateChildrenTrans()
{
	//计算当前节点的矩阵
	xmat4& locTrans = *m_Placement.toMatrix();
    if(m_pParent)
	{
		m_trans =  locTrans * m_pParent->m_trans;
	}
	else
	{
		m_trans = locTrans;
	}

	beginEnumChildren();
	ISceneNode* pChildNode = NULL;
	while(pChildNode = nextChild() )
	{
		if(pChildNode) pChildNode->updateChildrenTrans();
	}
	endEnumChildren(); 
	return true;
}

bool ISceneNode::updateFrame(unsigned long passedTime , IRenderCamera* pCamera , bool bRecursive)
{
	for(size_t i = 0 ; i < nObjects() ; i ++ )
	{
		getObject(i)->updateFrame(passedTime , pCamera);
	}

	if(bRecursive)
	{
		beginEnumChildren();
		ISceneNode* pChildNode = NULL;
		while(pChildNode = nextChild() )
		{
			pChildNode->updateFrame(passedTime , pCamera ,  bRecursive);
		}
		endEnumChildren();
	}
	return true ;
}

bool ISceneNode::save(xXmlNode* pXml)
{
	xXmlNode* pPlacemenetNode = pXml->insertNode(L"placement");
	savePlacement(pPlacemenetNode);
    pXml->setValue(L"name" , m_Name.c_str());
	pXml->setValue(L"visible" , m_bVisible );
	beginEnumChildren();
	ISceneNode* pChildNode = NULL;
	while(pChildNode = nextChild() )
	{
		if(pChildNode->bInScene() )
		{
			xXmlNode* pChildXml = pXml->insertNode(L"child");
			pChildNode->save(pChildXml);
		}
	}
	endEnumChildren();

	return true;
}

ISceneNode* XEvol_InsertSceneNode( xXmlNode* pChildXML , ISceneGraph* pScene , ISceneNode* pParentNode ) 
{
	const wchar_t* type  = pChildXML->value(L"type");
	int            param = pChildXML->int_value(L"param");
	if(NULL != type)
	{
		ISceneNode* pSceneNode = xSceneNodeFactoryMgr::singleton()->createInstance(type , pScene , pParentNode );
		if(wcslen_x(pSceneNode->type_name()) == 0) pSceneNode->type_name(type);
		if(pSceneNode == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL , L"SceneNode type not registed type=%s\n" , type);
			return NULL;
		}
		pSceneNode->setName( pChildXML->value(L"name"));
		if(pSceneNode->load(pChildXML) == true)
		{ 
			pParentNode->insertChild(pSceneNode);
		}
		pSceneNode->ReleaseObject();
		return pSceneNode;
	}
	return NULL;
}

bool ISceneNode::load(xXmlNode* pNode)
{
    //Load placement;
	xXmlNode* pPlacemenetNode = pNode->findNode(L"placement");
	if(pPlacemenetNode == NULL) 
	{
		pPlacemenetNode = pNode;
	}
	loadPlacement(pPlacemenetNode);

	if(pNode->value(L"visible")) m_bVisible = pNode->bool_value(L"visible");
	//Load children;
	xXmlNode::XmlNodes childNodes;
	pNode->findNode(L"child" , childNodes);
	size_t nChildNodes = childNodes.size() ;
	for(size_t i = 0 ;i < nChildNodes; i ++)
	{
		xXmlNode* pChildXML = childNodes[i];
        XEvol_InsertSceneNode(pChildXML,m_pScene , this);
	}
	return true;
}
/*
size_t ISceneNode::nChildren() { return }
bool ISceneNode::beginEnumChildren() = 0;
ISceneNode* ISceneNode::nextChild() = 0;
void ISceneNode::insertChild(ISceneNode* pNode) = 0;
void ISceneNode::insertChild(ISceneNode* pNode , const wchar_t* _name) = 0;
ISceneNode* ISceneNode::findChild(const wchar_t* _name) = 0;
bool ISceneNode::findChildren(ISceneNode::vSceneNodes& _ret) = 0;
bool ISceneNode::removeChild(ISceneNode* pNode , bool bRecursive = false) = 0;
*/
//--------------------------------
//
//--------------------------------
xSceneBasicNode::xSceneBasicNode(ISceneGraph*  pScene , ISceneNode* pParent )
:ISceneNode(pScene , pParent )
{
	m_iCurrentIdx = 0;
	m_pPropertySet = NULL;
	m_TypeName = L"xSceneBasicNode";
}

xSceneBasicNode::~xSceneBasicNode()
{
    unload();
}

ISceneNodePropertySet*   xSceneBasicNode::getPropertySet()
{
     if(m_pPropertySet == NULL)
	 {
		 m_pPropertySet = new ISceneNodePropertySet(L"xBasiceSceneNodeProperty",L"Scene Node Property");
		 initPropertySet();
	 }
	 return m_pPropertySet;
}

bool   xSceneBasicNode::unload()
{
	size_t nChildrens = m_Childrens.size() ;
	for(size_t i = 0 ;i < nChildrens; i ++ )
	{
		ISceneNode* pSceneNode = m_Childrens[i];
		pSceneNode->onDetached();
		pSceneNode->ReleaseObject();
	}
    m_Childrens.clear();
	size_t nObjects = m_vObjects.size() ;
	for(size_t i = 0 ; i < nObjects ; i ++)
	{
		ISceneObject* pSceneObject = m_vObjects[i];
		pSceneObject->ReleaseObject();
	}
	m_vObjects.clear();
	return true;
}
size_t xSceneBasicNode::nChildren()
{
	return m_iCurrentIdx;
}


bool xSceneBasicNode::beginEnumChildren()
{
	m_iCurrentIdx = 0;
	if(m_Childrens.size() > 0)
		return false;
	return true;
}

ISceneNode* xSceneBasicNode::nextChild()
{
	if(m_iCurrentIdx >= (int)m_Childrens.size() )
		return NULL;
	ISceneNode* pNode = m_Childrens[m_iCurrentIdx];
	m_iCurrentIdx ++;
	return pNode;
}

void xSceneBasicNode::endEnumChildren()
{

}


void xSceneBasicNode::insertChild(ISceneNode* pNode)
{
	if(pNode == this)
	{
		return ;
	}
	pNode->setParent(this);
	pNode->AddRef();
	m_Childrens.push_back(pNode);

}

void xSceneBasicNode::insertChild(ISceneNode* pNode , const wchar_t* _name)
{
	pNode->setName(_name);
    return insertChild(pNode);
}

ISceneNode* xSceneBasicNode::findChild(const wchar_t* _name,  bool bRecursive)
{
   if(_name == NULL)
	   return NULL;

   std::ds_wstring strName = _name; 
   size_t nChildrenNode = m_Childrens.size() ;
   for(size_t i = 0 ; i < nChildrenNode ; i ++)
   {
	   if(m_Childrens[i]->name() == strName )
		   return m_Childrens[i];
   }

   for(size_t i = 0 ; i < nChildrenNode ; i ++)
   {
	   ISceneNode* pChildNode = m_Childrens[i];
	   ISceneNode* pFindNode = NULL;
	   if(pChildNode && (pFindNode = pChildNode->findChild(_name , bRecursive)) )
	   {
		   return pFindNode;
	   }
   }

   return NULL;
}

bool  xSceneBasicNode::findChildren(ISceneNode::vSceneNodes& _ret , const wchar_t* _name , bool bRecursive )
{
	if(_name == NULL)
		return false;

	std::ds_wstring strName = _name; 
	size_t nChildrenNode = m_Childrens.size() ;
	for(size_t i = 0 ; i < nChildrenNode ; i ++)
	{
		if(m_Childrens[i]->name() == strName )
		{
			_ret.push_back(m_Childrens[i]);
		}
	}

	for(size_t i = 0 ; i < nChildrenNode ; i ++)
	{
		ISceneNode* pChildNode = m_Childrens[i];
		ISceneNode* pFindNode = NULL;
		pChildNode->findChildren(_ret , _name , bRecursive);
	}
	return _ret.size() != 0;
}

bool xSceneBasicNode::findChildren(ISceneNode::vSceneNodes& _ret)
{
    if(m_Childrens.size() == 0 )
		return false;
	_ret.insert(_ret.end() , m_Childrens.begin(), m_Childrens.end());
	return true;
}
bool xSceneBasicNode::findChildren(ISceneNode::vSceneNodes& _ret , const xObjectClassID&  classID ,bool inclChildren)
{
	if(this->isType(classID))
	{
		_ret.push_back(this);
		if(inclChildren == false)
			return true;
	}
	size_t n =  m_Childrens.size()  ; 
	for(size_t i = 0 ; i <  n ; i ++)
	{
		m_Childrens[i]->findChildren(_ret , classID , inclChildren);
	}
	return true;
}
bool xSceneBasicNode::removeChild(ISceneNode* pNode , bool bRecursive )
{
	vSceneNodes::iterator pos = m_Childrens.begin();
	for( ; pos != m_Childrens.end() ; pos ++)
	{
		ISceneNode* pThisNode = *pos;
		if(pThisNode == pNode)
		{
			//Remove all child.
			if(bRecursive == true)
			{
				while(pNode->nChildren() )
				{
					ISceneNode* pChildNode = NULL;
					pNode->beginEnumChildren();
					pChildNode = pNode->nextChild();
					pNode->endEnumChildren();
					pNode->removeChild(pChildNode);
				}
			}

			m_Childrens.erase(pos);
			pNode->onDetached();
			pNode->ReleaseObject();
			return true;
		}


	}
	return false;
}

bool xSceneBasicNode::onDetached()
{
	m_pParent = NULL;
	return true;
}

bool xSceneBasicNode::onAttached()
{
	return true;
}

bool xSceneBasicNode::save(xXmlNode* pNode)
{
	if(pNode == NULL)
		return false;

	pNode->setValue(L"type" ,  type_name() );
	if(false == ISceneNode::save(pNode) )
		return false;

	size_t _nObjects = m_vObjects.size() ;
	for(size_t i = 0 ; i < _nObjects ; i ++)
	{
		xXmlNode* pEntityNode = pNode->insertNode(L"entity");
		ISceneObject* pObject = m_vObjects[i];
		pObject->save(pEntityNode);
		
	}
	return true;
}

bool xSceneBasicNode::load(xXmlNode* pNode)
{
    //load the content from XML node.
	xXmlNode::XmlNodes entityNodes;
	pNode->findNode(L"entity" , entityNodes);
	size_t nEntityNodes =  entityNodes.size() ;
	for(size_t i = 0 ;i < nEntityNodes ; i ++)
	{
		xXmlNode* pEntityXML = entityNodes[i];
		const wchar_t* type  = pEntityXML->value(L"type");
		if(NULL != type)
		{
			ISceneObject* pObject = xSceneObjectFactoryMgr::singleton()->createInstance(type , m_pScene , 0 );
			if(pObject)
			{
                if(true == pObject->load(pEntityXML) )
                {
                    attachObject(pObject);
                }
                pObject->ReleaseObject();
			}
			else
			{
#ifdef _DEBUG
				assert(0);
#endif
			}
		}
	}
	return ISceneNode::load(pNode);
}

bool xSceneBasicNode::updateFrame(unsigned long passedTime ,IRenderCamera* pCamera , bool bRecursive)
{
	size_t _nObj = nObjects() ;
	for(size_t i = 0 ; i < _nObj ; i ++ )
	{
		m_vObjects[i]->updateFrame(passedTime , pCamera);
	}

	if(bRecursive)
	{
        size_t _nChildrens = m_Childrens.size() ;
		for(size_t i = 0 ; i < _nChildrens ; i ++)
		{
			m_Childrens[i]->updateFrame(passedTime , pCamera , bRecursive);
		}
	}
	return true ;
}

bool  xSceneBasicNode::attachObject(ISceneObject* pObject)
{
	pObject->AddRef();
	m_vObjects.push_back( pObject );
	return true;
}

bool  xSceneBasicNode::detachObject(ISceneObject* pObject)
{
	ISceneObject::vSceneObjects::iterator pos = m_vObjects.begin() ;
	for( ; pos != m_vObjects.end() ; pos ++)
	{
		if(*pos == pObject)
		{
			pObject->ReleaseObject();
			m_vObjects.erase( pos );
			return true ;
		}
	}
	return false;
}

bool  xSceneBasicNode::detachAllObject()
{
	size_t _nObjects = m_vObjects.size() ;
	for(size_t i = 0 ; i < _nObjects ; i ++)
	{
		m_vObjects[i]->ReleaseObject();
	}
	m_vObjects.clear();
	return true;
}
size_t  xSceneBasicNode::nObjects()
{
	return m_vObjects.size();
}
ISceneObject*  xSceneBasicNode::getObject(size_t idx )
{
	if(m_vObjects.size() <= idx)
		return NULL;
	return m_vObjects[idx];
}

bool xSceneBasicNode::updateChildrenTrans()
{
	ISceneNode::updateChildrenTrans();
	size_t nObject = nObjects();
	for(size_t i = 0 ;  i < nObject ; i ++)
	{
		ISceneObject* pObject = getObject(i);
		ISceneDrawable* pDrawable = type_cast<ISceneDrawable*>(pObject);
		if(pDrawable)
		{
            pDrawable->setMatrix(m_trans);
        }
		//	size_t nElement  = pDrawable->nDrawElement();
		//	for(size_t iEl = 0 ; iEl < nElement ; iEl ++)
		//	{
		//		IDrawElement*   pDrawElement = pDrawable->drawElement(iEl);
		//		if(pDrawElement) 
		//		{
		//			pDrawElement->setMatrix(m_trans);
		//		}
		//	}
		//}
	}
	return true ;
}
IMPL_BASE_OBJECT_DLLSAFE(xSceneBasicNode , ISceneNode);
IMPL_SCENENODE_FACTORY(xSceneBasicNode);
END_NAMESPACE_XEVOL3D