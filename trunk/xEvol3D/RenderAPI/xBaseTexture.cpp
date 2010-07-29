#include "../xStdPch.h"
#include "xBaseTexture.h"

BEGIN_NAMESPACE_XEVOL3D




IBaseTexture::IBaseTexture(IRenderApi* pRenderApi)
:IRenderApiResource(pRenderApi)
{

}

IBaseTexture:: ~IBaseTexture()
{

}


IMPL_BASE_OBJECT_DLLSAFE(xBaseTextureArray , IBaseTexture);

xBaseTextureArray::xBaseTextureArray(IRenderApi* pRenderApi)
	:IBaseTexture(pRenderApi)
{

}
xBaseTextureArray::~xBaseTextureArray()
{
}

int xBaseTextureArray::nSubTexture() 
{
	return (int)m_vTextureArray.size() ; 
}

IBaseTexture* xBaseTextureArray::subTexture(int index)
{ 
	return m_vTextureArray[index];
}

IBaseTexture*& xBaseTextureArray::operator[] (int index)
{
	return m_vTextureArray[index];
}

void xBaseTextureArray::setArraySize(int _size)
{
	m_vTextureArray.resize(_size);
}

void xBaseTextureArray::setSubTexture(int index , IBaseTexture* pTexture)
{
	m_vTextureArray[index] = pTexture;
}

END_NAMESPACE_XEVOL3D
