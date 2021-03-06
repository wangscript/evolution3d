#include "stdafx.h"
#include "xDirect3D10API.h"
#include "xD10BaseRenderTarget.h"
#include "xD10BaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D


xD10BaseRenderTarget::xD10BaseRenderTarget(xD3D10RenderApi* pD10Api, int arraySlice , int mipmapLevel):IRenderTarget(pD10Api) 
{
	m_pD10Api = pD10Api ; 
	m_pBaseTexture = NULL;
	m_RefCount  = 1;
    m_mipmapLevel = arraySlice;
    m_arraySlice = mipmapLevel;

}

bool xD10BaseRenderTarget::grabRenderTagetData(void* pData, int x , int y , int w , int h )
{
	xD10BaseTexture* pD10Texture = (xD10BaseTexture*)m_pBaseTexture;
	return pD10Texture->grabRenderTagetData(pData , x , y , w , h , m_arraySlice , m_mipmapLevel);
}

void xD10BaseRenderTarget::setTexture(IBaseTexture* pTexture)
{
	m_pBaseTexture = pTexture;
}

bool xD10BaseRenderTarget::saveResourceToFile(const wchar_t* fileName , ID3D10Resource* pResource)
{
	D3DX10_IMAGE_FILE_FORMAT fFormat = xD10ConstLexer::GetDX10ImageFileFormat(fileName);
	if((int)fFormat == -1)
		return false;

	if(pResource == NULL)
		return false;

	HRESULT hr = D3DX10SaveTextureToFileW( pResource ,  fFormat , fileName);
	return SUCCEEDED(hr);
}

xD10BaseRenderTarget::~xD10BaseRenderTarget()
{

}


END_NAMESPACE_XEVOL3D


