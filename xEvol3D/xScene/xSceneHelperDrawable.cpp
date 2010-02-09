#include "../xStdPch.h"
#include "xSceneHelperDrawable.h"
#include "../Renderer/xRenderer.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xSceneHelperDrawable , xSceneEffectObject);
IMPL_SCENEOBJECT_FACTORY(xSceneHelperDrawable , L"xSceneHelperDrawable");
bool     xSceneHelperDrawable::draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamera)
{
	IDrawElement*   pDrawElement = drawElement(0);
	IRenderEffect*  pEffect      = effect(0);

	if(pDrawElement && pCamera && false == pDrawElement->isVisible(pCamera) )
		return false; 

	if(pEffect )
	{
		pEffect->draw(pDrawElement);
		return true;
	}
	else
	{
		m_Pass.setRenderer( pRenderer  );
		m_Pass.setDrawable(pDrawElement);
		pRenderer->drawPass( xStringHash(L"overlay") , &m_Pass);	
	}
	return false;
}
bool  xSceneHelperDrawable::draw(IBaseRenderer* pRenderer, unsigned int passedTime, xGeomLib::xCamera* pCamera)
{
	IDrawElement*   pDrawElement = drawElement(0);
	IRenderEffect*  pEffect      = effect(0);

	if(pDrawElement && pCamera && false == pDrawElement->isVisible(pCamera) )
		return false; 

	if(pEffect )
	{
		pEffect->draw(pDrawElement , passedTime );
		return true;
	}
	else
	{
		m_Pass.setRenderer( pRenderer );
		m_Pass.setDrawable(pDrawElement);
		pRenderer->drawPass(  &m_Pass , passedTime , true);	
	}
	return false;
}

IDrawElement*    xSceneHelperDrawable::drawElement(size_t idx)
{
	return m_pElement;
}

bool             xSceneHelperDrawable::setDrawElement(IDrawElement* pDrawElement , size_t idx)
{
	if(m_pElement) 
	{
		m_pElement->ReleaseObject();
	}
	m_pElement = pDrawElement ; 
	m_pElement->AddRef();
	return true ;
}

bool     xSceneHelperDrawable::updateFrame(unsigned long passedTime)
{
	return true ;
}

bool     xSceneHelperDrawable::load(xXmlNode* pXml)
{
	return true ;
}

bool     xSceneHelperDrawable::save(xXmlNode* pNode)
{
	return true ;
}
xSceneHelperDrawable::xSceneHelperDrawable(ISceneGraph*  pScene , int arg) : xSceneEffectObject(pScene , arg) ,m_Pass( NULL )
{
	m_pElement = NULL;
}

xSceneHelperDrawable::~xSceneHelperDrawable()
{
	if(m_pElement) 
	{
		m_pElement->ReleaseObject();
		m_pElement = NULL;
	}
}


////=
IMPL_SCENENODE_FACTORY(xSceneHelperDrawableNode);
IMPL_BASE_OBJECT_DLLSAFE(xSceneHelperDrawableNode , xSceneBasicNode);
xSceneHelperDrawableNode::xSceneHelperDrawableNode(ISceneGraph*  pScene , ISceneNode* pParent)
:xSceneBasicNode(pScene , pParent)
{

}

xSceneHelperDrawableNode::~xSceneHelperDrawableNode()
{
	ISceneObject::vSceneObjects::iterator pos = m_vObjects.begin();
	for( ; pos != m_vObjects.end() ; pos ++)
	{
		ISceneObject* pObject = *pos;
		delete pObject;
	}
	m_vObjects.clear();
}

bool xSceneHelperDrawableNode::attachDrawElement(IDrawElement* pDrawElement)
{
	if(pDrawElement == NULL)
		return false;
     xSceneHelperDrawable* pObject = new xSceneHelperDrawable(m_pScene , 0);
	 pObject->setDrawElement(pDrawElement,0);
	 return attachObject(pObject);
}

bool xSceneHelperDrawableNode::detachDrawElement(IDrawElement* pDrawElement)
{
   ISceneObject::vSceneObjects::iterator pos = m_vObjects.begin();
   for( ; pos != m_vObjects.end() ; pos ++)
   {
	   ISceneObject* pObject = *pos;
	   xSceneHelperDrawable* pDrawable = dynamic_cast<xSceneHelperDrawable*>(pObject);
	   if(pDrawable->drawElement(0) == pDrawElement)
	   {
		   m_vObjects.erase(pos);
		   delete pDrawable;
		   return true;		   
	   }
   }
   return false;
}

bool  xSceneHelperDrawableNode::load(xXmlNode* pNode)
{
	return true;
}

bool  xSceneHelperDrawableNode::save(xXmlNode* pNode)
{
	return true;
}
END_NAMESPACE_XEVOL3D