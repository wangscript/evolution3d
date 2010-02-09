#include "../xStdPch.h"
#include "xSceneModel.h"
#include "xSceneGraph.h"
#include "../Renderer/xRenderer.h"

// TEST
#include <OperationSys/xOperationSys.h>
#include <RenderAPI/xBaseTexture.h>

BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xSceneModel, xSceneEffectObject);
IMPL_BASE_OBJECT_DLLSAFE(xSceneMesh, xSceneEffectObject);

IMPL_SCENEOBJECT_FACTORY(xSceneModel,L"xSceneModel");
IMPL_SCENEOBJECT_FACTORY(xSceneMesh ,L"xCoreMesh");

//===========================================
bool xSceneModel::load(xXmlNode* pXml)
{
    if(m_pModelElement)
		m_pModelElement->ReleaseObject();
	IBaseRenderer* pRenderer = m_pSceneGraph->renderer();
	m_pModelElement = new xModelElement(pRenderer , 0);

	xBaseModelMgr* pModelMgr = m_pSceneGraph->getModelManager();
	m_strModelFile = pXml->value(L"file");
	HBaseModel hModel = pModelMgr->add( m_strModelFile.c_str() , true , true);
	m_pModelElement->setModel(hModel);

	//加载Effect;
	const wchar_t* _effectName = pXml->value(L"effect");
	IRenderEffect* _effect = pRenderer->createRenderEffect(_effectName);
    if(_effectName) m_effectName = _effectName;
	//材质
	const wchar_t* _matName = pXml->value(L"material");
	xMaterial* pMaterial = pRenderer->createMaterial(_matName);
	_effect->setMaterial(pMaterial);
	if(_matName) m_matName = _matName;

	//做一些_effect参数的加载工作
	xXmlNode* pEffectXml = pXml->findNode(L"EffectParam");
	_effect->load(pEffectXml);  
	m_pEffect = _effect;
	return true;
}

bool xSceneModel::save(xXmlNode* pXml)
{
	if(pXml == NULL)
		return false;
    pXml->setValue(L"type"       , L"xSceneModel");
	pXml->setValue(L"file"       , m_strModelFile.c_str() );
	pXml->setValue(L"material"   , m_matName.c_str() );
	pXml->setValue(L"effect"     , m_effectName.c_str() );
	if(m_pEffect)
	{
		xXmlNode* pEffectXML = pXml->insertNode(L"EffectParam");
		m_pEffect->save(pEffectXML);
	}
	return true;
}

bool xSceneModel::updateFrame(unsigned long passedTime)
{
    m_pModelElement->update(passedTime);
	return true ;
}

xSceneModel::xSceneModel(ISceneGraph*  pScene , int arg)
: xSceneEffectObject(pScene , arg)
{
   m_pModelElement = NULL;
   m_pEffect = NULL;
}

xSceneModel::~xSceneModel()
{
    unload();
}

bool xSceneModel::unload()
{
	XSAFE_RELEASEOBJECT(m_pModelElement);
	XSAFE_RELEASEOBJECT(m_pEffect);
	return xSceneEffectObject::unload();
}

IDrawElement*  xSceneModel::drawElement(size_t idx)
{
	if(idx != 0)
		return NULL;

	return m_pModelElement;
}

bool xSceneModel::setDrawElement(IDrawElement* pDrawElement , size_t idx)
{
	return false;
}

//=================
bool xSceneMesh::load(xXmlNode* pXml)
{
	return false;
}

bool xSceneMesh::unload()
{
	return true;
}
bool xSceneMesh::save(xXmlNode* pXml)
{
	return false;
}

xSceneMesh::xSceneMesh(ISceneGraph*  pScene , int arg)
: xSceneEffectObject(pScene , arg)
{

}
xSceneMesh::~xSceneMesh()
{

}

IDrawElement* xSceneMesh::drawElement(size_t idx)
{
	return NULL;
}

bool xSceneMesh::setDrawElement(IDrawElement* pDrawElement , size_t idx)
{
	return false;
}

bool xSceneMesh::updateFrame(unsigned long passedTime)
{
	return true ;
}








END_NAMESPACE_XEVOL3D