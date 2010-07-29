#include "../xStdPch.h"
#include "xDrawableHelper.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(IDrawableHelper ,  xSerializeDrawElement);
IDrawableHelper::IDrawableHelper(IBaseRenderer* pRenderer):xSerializeDrawElement(pRenderer , 0)
{
}
bool IDrawableHelper::load(xXmlNode* pXml)
{
    return true;
}
bool IDrawableHelper::save(xXmlNode* pNode)
{
    return true;
}

END_NAMESPACE_XEVOL3D
