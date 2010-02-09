#include "../xStdPch.h"
#include "xSceneDrawable.h"
#include "xSceneGraph.h"
#include "../Renderer/xRenderer.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(ISceneDrawable,  ISceneObject);
IMPL_BASE_OBJECT_DLLSAFE(xSceneEffectObject , ISceneDrawable);

ISceneDrawable::ISceneDrawable(ISceneGraph*  pScene , int arg)
:ISceneObject(pScene , arg)
{

}

ISceneDrawable::~ISceneDrawable()
{

}

bool   ISceneDrawable::isVisible(xGeomLib::xCamera* pCamera)
{
	size_t nElement = nDrawElement();
	for(size_t i = 0 ; i < nElement ; i ++)
	{
		IDrawElement*   pDrawElement = drawElement(i);
		if(pDrawElement && pDrawElement->isVisible(pCamera) )
		{
			return true;
		}
	}
	return false;
}
xSceneEffectObject::xSceneEffectObject(ISceneGraph*  pScene , int arg)
:ISceneDrawable(pScene , arg)
{
	m_pEffect   =  NULL;

}

xSceneEffectObject::~xSceneEffectObject()
{
	if(m_pEffect  ) 
		m_pEffect->ReleaseObject();

}

bool           xSceneEffectObject::load(xXmlNode* pXml)
{
	const wchar_t* _effectName = pXml->value(L"effect");
	IBaseRenderer* pRenderer = m_pSceneGraph->renderer();
	IRenderEffect* _effect = pRenderer->createRenderEffect(_effectName);
	//做一些_effect参数的加载工作
	xXmlNode* pEffectXml = pXml->findNode(L"EffectParam");
	_effect->load(pEffectXml);
	return setEffect(_effect);
}

bool xSceneEffectObject::unload()
{
	XSAFE_RELEASEOBJECT(m_pEffect);
	return true;
}

IRenderEffect* xSceneEffectObject::effect(size_t idx)
{
	return m_pEffect;
}

bool           xSceneEffectObject::setEffect(IRenderEffect* _effect)
{
	if(_effect == NULL)
		return false;

	if(m_pEffect ) m_pEffect->ReleaseObject();
	m_pEffect = _effect;
	m_pEffect->AddRef();
	return true;
}

bool     xSceneEffectObject::draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamera)
{
	size_t nElement = nDrawElement();
	for(size_t i = 0 ; i < nElement ; i ++)
	{
		IDrawElement*   pDrawElement = drawElement(i);
		IRenderEffect*  pEffect      = effect(i);
		if(pCamera && pDrawElement && pDrawElement->isVisible(pCamera) == false)
			continue;

		if(pEffect )
		{
			pEffect->draw(pDrawElement);
		}
	}
	return true;
}


bool     xSceneEffectObject::draw(IBaseRenderer* pRenderer , unsigned int passedTime  , xGeomLib::xCamera* pCamera)
{
	size_t nElement = nDrawElement();
	for(size_t i = 0 ; i < nElement ; i ++)
	{
		IDrawElement*   pDrawElement = drawElement(i);
		IRenderEffect*  pEffect      = effect(i);
		if(pCamera && pDrawElement && pDrawElement->isVisible(pCamera) == false)
			continue;

		if(pEffect )
		{
			pEffect->draw(pDrawElement , passedTime);
		}
	}
	return true;
}

END_NAMESPACE_XEVOL3D