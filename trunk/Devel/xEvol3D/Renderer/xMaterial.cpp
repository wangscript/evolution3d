#include "xMaterial.h"
#include "../BaseLib/xCfgParser.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xMaterial ,  IRenderObject);
xMaterial::xMaterial(IRenderApi* pRenderApi)
:IRenderObject(pRenderApi)
{

}
 xMaterial::~xMaterial()
{

}
bool xMaterial::load(const wchar_t* matName , const int8* buf , int len)
{
    xCfgDocument doc ;
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



END_NAMESPACE_XEVOL3D
