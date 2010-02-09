#include "../xStdPch.h"
#include "xDrawableHelper.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(IDrawableHelper ,  IDrawElement);
IDrawableHelper::IDrawableHelper(IBaseRenderer* pRenderer):IDrawElement(pRenderer)
{
}
END_NAMESPACE_XEVOL3D
