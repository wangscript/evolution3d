#include "xMaterial.h"
#include "../BaseLib/xXmlDocument.h"
#include "xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xMaterial     ,  IRenderObject);
IMPL_BASE_OBJECT_DLLSAFE(xRenderEffect ,  IRenderObject);
xMaterial::xMaterial(IRenderApi* pRenderApi)
:IRenderObject(pRenderApi)
{

}
 xMaterial::~xMaterial()
{

}
bool xMaterial::load(const wchar_t* matName , const int8* buf , int len)
{
    xXmlDocument doc ;
	return true;
}

bool  xMaterial::isLoad()
{
	return true;
}

int  xMaterial::getResSize()
{
	return 0;
}

void  xMaterial::unload()
{

}

bool xMaterial::begin(IGpuProgramParamTable* pConstTable)
{
	return false;
}

bool xMaterial::end(IGpuProgramParamTable* pConstTable)
{
	return false;
}

bool xMaterial::render(IDrawableObject* pObject ,unsigned long passedTime)
{
     return  pObject->render(passedTime);
}


xRenderEffect::xRenderEffect(IRenderApi* pRenderApi)
:IRenderObject(pRenderApi)
{

}

xRenderEffect::~xRenderEffect()
{

}

END_NAMESPACE_XEVOL3D
