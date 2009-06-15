#include "xKidRenderer.h"
#include "xKidMaterail.h"
#include "../xRendererQueue.h"
#include "../xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xKidBasicMaterial , xMaterial);

xKidBasicMaterial::xKidBasicMaterial(IRenderApi* pRendereApi):xMaterial(pRendereApi)
{

};

xKidBasicMaterial::~xKidBasicMaterial()
{

};

END_NAMESPACE_XEVOL3D