#include "xRenderView.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(IRenderView   , IRenderObject);
IMPL_BASE_OBJECT_CLASSID(IRenderTarget , IRenderObject);

IRenderTarget::IRenderTarget(IRenderApi* pRenderApi)
:IRenderObject(pRenderApi)
{

}

IRenderTarget::~IRenderTarget()
{

}


IRenderView::IRenderView(IRenderApi* pRenderApi)
:IRenderObject(pRenderApi)
{

}

IRenderView::~IRenderView()
{

}

END_NAMESPACE_XEVOL3D
