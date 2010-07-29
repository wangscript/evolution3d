#include "../xStdPch.h"
#include "xActorModel.h"
#include "xSceneGraph.h"
#include "../Renderer/xRenderer.h"
#include "../RenderAPI/xShaderName.h"
#include "../xModel/xCoreActionMgr.h"
// TEST
#include <OperationSys/xOperationSys.h>
#include <RenderAPI/xBaseTexture.h>

BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xSceneActor, xSceneEffectObject);
IMPL_SCENEOBJECT_FACTORY(xSceneActor, L"xSceneActor");


//===========================================
bool xSceneActor::load(xXmlNode* pXml)
{
    if(m_pModelElement)
		m_pModelElement->ReleaseObject();
	IBaseRenderer* pRenderer = m_pSceneGraph->renderer();
	m_pModelElement = new xActorElement(pRenderer , 0);

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


    //处理材质 //自动移掉SkinAni的修改器。
    IGpuProgram* pShader = pMaterial->gpuProgram();
    xBaseModel* pModel = hModel.getResource();
    IRenderApi* pRenderApi = pRenderer->renderApi();
    if(pShader && pModel)
    {
        xGpuProgNameParser _nameParser;
        pShader->getName( _nameParser );
        xShaderName* pVsName = _nameParser.getShaderName(eShader_VertexShader);
        bool bSkinModifier = pModel->skeleton() != NULL;
        if(pModel->skeleton())
        {
            bSkinModifier = pRenderApi->intCapsValue(L"MaxGpuBone" , 256) >= pModel->skeleton()->nBone();
        }

        //自动移掉SkinAni的修改器。
        if(pVsName && bSkinModifier == false && pVsName->removeNode(L"SkinAni"))
        {
            xGpuProgramName _name;
            _nameParser.toName(_name);
            pMaterial->setGpuProgram(_name);
        }    

    }

	//做一些_effect参数的加载工作
	xXmlNode* pEffectXml = pXml->findNode(L"EffectParam");
	_effect->load(pEffectXml);  
	m_pEffect = _effect;
	return true;
}

bool xSceneActor::save(xXmlNode* pXml)
{
	if(pXml == NULL)
		return false;
    pXml->setValue(L"type"       , L"xSceneActor");
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

bool xSceneActor::updateFrame(unsigned long passedTime, IRenderCamera* pCamera)
{
    m_pModelElement->update(passedTime);
	return true ;
}

xSceneActor::xSceneActor(ISceneGraph*  pScene , int arg)
: xSceneEffectObject(pScene , arg)
{
   m_pModelElement = NULL;
   m_pEffect = NULL;
}

xSceneActor::~xSceneActor()
{
    unload();
}

bool xSceneActor::unload()
{
	XSAFE_RELEASEOBJECT(m_pModelElement);
	XSAFE_RELEASEOBJECT(m_pEffect);
	return xSceneEffectObject::unload();
}

IDrawElement*  xSceneActor::drawElement( size_t iEl)
{
	return m_pModelElement;
}

bool xSceneActor::setDrawElement(IDrawElement* pDrawElement  , size_t iEl)
{
	return false;
}



END_NAMESPACE_XEVOL3D