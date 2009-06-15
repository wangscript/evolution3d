#include "xD10TLEnviroment.h"
#include "xDirect3D10API.h"
#include "xD10InputBuffer.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD10LightingEnv  , ILightingEnv);
xD10LightingEnv::~xD10LightingEnv()
{
	xD3D10RenderApi* pD10Api = (xD3D10RenderApi*)m_pRenderApi;
	pD10Api->removeLightingState(m_name.c_str() );
	m_pReflection->ReleaseObject();
	m_pConstBuffer->ReleaseObject();
	m_pRenderApi->removeInputBuffer(m_pConstBuffer);
}

xD10LightingEnv::xD10LightingEnv(xD3D10RenderApi* pD10RenderApi , const wchar_t* _name)
:ILightingEnv(pD10RenderApi)
{
    m_RefCount = 1;
	m_name = _name;
	int iBufLen = sizeof(xD10LightEnvBuffer);
	m_pConstBuffer = pD10RenderApi->createConstBuffer( iBufLen );
	xD10InputBuffer* pD10Buffer = (xD10InputBuffer*)m_pConstBuffer;
	pD10RenderApi->setInputBuffer(_name , m_pConstBuffer);
	m_pReflection = (xD10InputBufReflection *)pD10Buffer->createReflection(true);
	m_pEnvBuffer = (xD10LightEnvBuffer*)m_pReflection->systemBuffer();
	m_pReflection->setDirty(true);
}

xLightDesc* xD10LightingEnv::lightDesc(size_t idx)
{
	return &m_pEnvBuffer->m_Light[idx];
}

void xD10LightingEnv::enableLight(size_t _nLight)
{
    m_pEnvBuffer->nLight[0] = (int)_nLight;
}
size_t xD10LightingEnv::nLightEnabled()
{
    return m_pEnvBuffer->nLight[0];
}
const wchar_t* xD10LightingEnv::name()
{
	return m_name.c_str() ; 
}
void xD10LightingEnv::applyChange()
{
	m_pReflection->setDirty(true);
}


END_NAMESPACE_XEVOL3D

