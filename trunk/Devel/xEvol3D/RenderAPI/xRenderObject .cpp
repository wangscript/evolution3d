#include "xRenderObject.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(IRenderObject   , IBaseObject);
IMPL_BASE_OBJECT_CLASSID(IRenderState       , IRenderObject );
IMPL_BASE_OBJECT_CLASSID(ISamplerState      , IRenderState );
IMPL_BASE_OBJECT_CLASSID(IBlenderState        , IRenderState );
IMPL_BASE_OBJECT_CLASSID(IRasterizerState   , IRenderState );
IMPL_BASE_OBJECT_CLASSID(IDepthStencilState , IRenderState );
IMPL_BASE_OBJECT_CLASSID(IRenderResource    , IBaseObject);

IRenderResource::IRenderResource(IRenderApi* pOwner)
:IRenderObject(pOwner)
{
    m_pRenderApi = pOwner;
}

IRenderResource::~IRenderResource()
{

}


IRenderObject::IRenderObject(IRenderApi* pOwner)
{
	m_pRenderApi = pOwner;
}

IRenderObject::~IRenderObject()
{

}

IRenderState::IRenderState(IRenderApi* pOwner):IRenderObject(pOwner) 
{

}

IRenderState::~IRenderState()
{

}

END_NAMESPACE_XEVOL3D