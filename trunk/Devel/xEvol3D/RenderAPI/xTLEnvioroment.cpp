#include "xTLEnvioroment.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(ILightingEnv   , IRenderObject);
ILightingEnv::ILightingEnv(IRenderApi* pRenderApi)
:IRenderObject(pRenderApi)
{

}

ILightingEnv::~ILightingEnv()
{

}

END_NAMESPACE_XEVOL3D
