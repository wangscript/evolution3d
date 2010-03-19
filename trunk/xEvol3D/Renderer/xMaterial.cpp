#include "../xStdPch.h"
#include "xMaterial.h"
#include "../BaseLib/xXmlDocument.h"
#include "xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xMaterial     ,  IRenderApiObject);
IMPL_BASE_OBJECT_DLLSAFE(IRenderEffect ,  IRenderApiObject);
xMaterial::xMaterial(IBaseRenderer* pRenderer)
:IRenderObject(pRenderer)
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

bool xMaterial::render(IDrawElement* pObject ,unsigned long passedTime)
{
     return  pObject->render(passedTime);
}

IGpuProgram*  xMaterial::gpuProgram() 
{
    return NULL;
}
bool  xMaterial::setGpuProgram(xGpuProgramName& _name)
{
    return false;
}
IRenderEffect::IRenderEffect( IBaseRenderer* pRenderer)
:IRenderObject(pRenderer)
{

}

IRenderEffect::~IRenderEffect()
{

}

END_NAMESPACE_XEVOL3D
