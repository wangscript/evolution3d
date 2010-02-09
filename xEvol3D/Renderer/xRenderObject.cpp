#include "../xStdPch.h"
#include "xRenderObject.h"
#include "xRenderer.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(IRenderObject   , IBaseObject);

IRenderObject::IRenderObject(IBaseRenderer* pOwner)
{
	m_pRenderer = pOwner;
}

IRenderObject::~IRenderObject()
{

}

END_NAMESPACE_XEVOL3D
