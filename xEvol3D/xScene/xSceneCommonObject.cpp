#include "../xStdPch.h"
#include "xSceneCommonObject.h"
#include "../Renderer/xRenderer.h"
#include "xSceneGraph.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xSceneSerializeObject , ISceneDrawable);
IMPL_SCENEOBJECT_FACTORY(xSceneSerializeObject , L"xSceneSerializeObject");

bool     xSceneSerializeObject::draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamera)
{
	int _nElement = this->nDrawElement();
	IRenderEffect* pEffect = this->effect();
	for(int i = 0 ; i  < _nElement ; i ++ )
	{
		IDrawElement*   pDrawElement = drawElement(i);

		if(pDrawElement && pCamera && false == pDrawElement->isVisible( pCamera , m_trans ) )
			continue ;
		if(pEffect)
		{
			pEffect->draw(pDrawElement , this);
		}
		else
		{
			m_Pass.setRenderer( pRenderer  );
			m_Pass.setDrawable( pDrawElement );
			m_Pass.setRenderPassArg( this );
			pRenderer->drawPass( xStringHash(L"default") , &m_Pass);
		}
	}
	return true;
}

bool  xSceneSerializeObject::drawImm(IBaseRenderer* pRenderer, unsigned int passedTime, xGeomLib::xCamera* pCamera)
{
	int _nElement = this->nDrawElement();
	IRenderEffect* pEffect = this->effect();
	for(int i = 0 ; i  < _nElement ; i ++ )
	{
		IDrawElement*   pDrawElement = drawElement(i);

		if(pDrawElement && pCamera && false == pDrawElement->isVisible( pCamera , m_trans ) )
			continue ;
		if(pEffect)
		{
			pEffect->drawImm(pDrawElement  , this , passedTime);
		}
		else
		{
			m_Pass.setRenderer(pRenderer );
			m_Pass.setDrawable(pDrawElement);
			m_Pass.setRenderPassArg( this );
			pRenderer->drawPass( &m_Pass , passedTime , true);	
		}
	}
	return false;
}

IDrawElement*    xSceneSerializeObject::drawElement(size_t idx)
{
	return m_pElement;
}

bool             xSceneSerializeObject::setDrawElement(IDrawElement* pDrawElement , size_t idx)
{
	xSerializeDrawElement* pSceneElement = type_cast<xSerializeDrawElement*>(pDrawElement);
	if(pSceneElement == NULL)
		return false;

	XSAFE_RELEASEOBJECT(m_pElement);
	m_pElement = pSceneElement ; 
	m_pElement->AddRef();
	return true ;
}

bool     xSceneSerializeObject::updateFrame(unsigned long passedTime, IRenderCamera* pCamera)
{
	return true ;
}

bool     xSceneSerializeObject::load(xXmlNode* pXml)
{
	IDrawElement* pElement = xDrawElementFactoryMgr::singleton()->createInstance(pXml->value(L"drawElement") , this->m_pSceneGraph->renderer() , 0 );
	xSerializeDrawElement* pSceneElement = type_cast<xSerializeDrawElement*>(pElement);

	if(pSceneElement == NULL)
		return false;

	XSAFE_RELEASEOBJECT(m_pElement);
	m_pElement = type_cast<xSerializeDrawElement*>(pElement);
	return m_pElement->load(pXml) ;
}

bool     xSceneSerializeObject::save(xXmlNode* pNode)
{ 
	pNode->setValue(L"type" , L"xSceneSerializeObject");
	if(m_pElement) return m_pElement->save(pNode);
	return true ;
}

xSceneSerializeObject::xSceneSerializeObject(ISceneGraph*  pScene , int arg) : ISceneDrawable(pScene , arg) ,m_Pass( NULL )
{
	m_pElement = NULL;
}

xSceneSerializeObject::~xSceneSerializeObject()
{
	if(m_pElement) 
	{
		m_pElement->ReleaseObject();
		m_pElement = NULL;
	}
}

bool xSceneSerializeObject::unload()
{
	return true;
}
END_NAMESPACE_XEVOL3D