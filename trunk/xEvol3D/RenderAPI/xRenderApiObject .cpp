#include "../xStdPch.h"
#include "xRenderApiObject.h"
#include "xRenderAPI.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(IRenderApiObject   , IBaseObject);
IMPL_BASE_OBJECT_CLASSID(IRenderApiState    , IRenderApiObject );
IMPL_BASE_OBJECT_CLASSID(ISamplerState      , IRenderApiState );
IMPL_BASE_OBJECT_CLASSID(IBlenderState      , IRenderApiState );
IMPL_BASE_OBJECT_CLASSID(IRasterizerState   , IRenderApiState );
IMPL_BASE_OBJECT_CLASSID(IDepthStencilState , IRenderApiState );
IMPL_BASE_OBJECT_CLASSID(IRenderApiResource , IBaseObject);

IRenderApiResource::IRenderApiResource(IRenderApi* pOwner)
:IRenderApiObject(pOwner)
{
    m_pRenderApi = pOwner;
}

IRenderApiResource::~IRenderApiResource()
{

}


IRenderApiObject::IRenderApiObject(IRenderApi* pOwner)
{
    m_pRenderApi = pOwner;
    if(m_pRenderApi)
    {
          m_pRenderApi->addRenderObject(this);
    }
}

IRenderApiObject::~IRenderApiObject()
{
    if(m_pRenderApi)
        m_pRenderApi->removeRenderObject(this);
}

IRenderApiState::IRenderApiState(IRenderApi* pOwner):IRenderApiObject(pOwner) 
{

}

IRenderApiState::~IRenderApiState()
{

}

END_NAMESPACE_XEVOL3D
