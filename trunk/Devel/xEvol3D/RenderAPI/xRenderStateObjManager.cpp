#include "xRenderStateObjManager.h"
#include "xRenderAPI.h"
BEGIN_NAMESPACE_XEVOL3D


xRenderStateObjectLoader::xRenderStateObjectLoader()
{
	m_pRenderApi = NULL;
}
void xRenderStateObjectLoader::setRenderApi(IRenderApi* pRenderApi)
{
	m_pRenderApi = pRenderApi;
}

IRenderState*  xRenderStateObjectLoader::newInstance(const wchar_t* strResName, unsigned long arg)
{
	return m_pRenderApi->createRenderState( (eRenderStateType)arg );
}
void xRenderStateObjectLoader::deleteInstance(IRenderState*& pRes)
{
	pRes->KillObject();

}


END_NAMESPACE_XEVOL3D
