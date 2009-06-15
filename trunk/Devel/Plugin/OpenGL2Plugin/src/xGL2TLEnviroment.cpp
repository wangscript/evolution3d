#include "xGL2TLEnviroment.h"
#include "xOpenGL2API.h"
#include "xGL2InputBuffer.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xGL2LightingEnv  , ILightingEnv);
xGL2LightingEnv::~xGL2LightingEnv()
{
	xGL2RenderApi* pD10Api = (xGL2RenderApi*)m_pRenderApi;
	pD10Api->removeLightingState(m_name.c_str() );
	m_pReflection->ReleaseObject();
	m_pConstBuffer->ReleaseObject();
	m_pRenderApi->removeInputBuffer(m_pConstBuffer);
}

xGL2LightingEnv::xGL2LightingEnv(xGL2RenderApi* pD10RenderApi , const wchar_t* _name)
:ILightingEnv(pD10RenderApi)
{
    m_RefCount = 1;
	m_name = _name;
	int iBufLen = sizeof(xGL2LightEnvBuffer);
	m_pConstBuffer = pD10RenderApi->createConstBuffer( iBufLen );
	xGL2InputBuffer* pD10Buffer = (xGL2InputBuffer*)m_pConstBuffer;
	pD10RenderApi->setInputBuffer(_name , m_pConstBuffer);
	m_pReflection = (xGL2InputBufReflection *)pD10Buffer->createReflection(true);
	m_pEnvBuffer = (xGL2LightEnvBuffer*)m_pReflection->systemBuffer();
	m_pReflection->setDirty(true);
}

xLightDesc* xGL2LightingEnv::lightDesc(size_t idx)
{
	return &m_pEnvBuffer->m_Light[idx];
}

void xGL2LightingEnv::enableLight(size_t _nLight)
{
    m_pEnvBuffer->nLight[0] = (int)_nLight;
}
size_t xGL2LightingEnv::nLightEnabled()
{
    return m_pEnvBuffer->nLight[0];
}
const wchar_t* xGL2LightingEnv::name()
{
	return m_name.c_str() ; 
}
void xGL2LightingEnv::applyChange()
{
	m_pReflection->setDirty(true);
}


END_NAMESPACE_XEVOL3D

