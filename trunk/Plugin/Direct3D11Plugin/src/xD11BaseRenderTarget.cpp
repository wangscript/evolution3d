#include "stdafx.h"
#include "xDirect3D11API.h"
#include "xD11BaseRenderTarget.h"
#include "xD11BaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D

xD11BaseRenderTarget::xD11BaseRenderTarget(xD3D11RenderApi* pD11Api , int arraySlice , int mipmapLevel):IRenderTarget(pD11Api) 
{
	m_pD11RenderApi = pD11Api ; 
	m_pBaseTexture = NULL;
	m_RefCount  = 1;
    m_arraySlice  = arraySlice;
    m_mipmapLevel = mipmapLevel;

}

bool xD11BaseRenderTarget::grabRenderTagetData(void* pData , int x , int y , int w , int h )
{
	xD11BaseTexture* pD10Texture = (xD11BaseTexture*)m_pBaseTexture;
	return pD10Texture->grabRenderTagetData(pData , x , y , w , h , m_arraySlice , m_mipmapLevel);
}

void xD11BaseRenderTarget::setTexture(IBaseTexture* pTexture)
{
	m_pBaseTexture = pTexture;
}

bool xD11BaseRenderTarget::saveResourceToFile(const wchar_t* fileName , ID3D11Resource* pResource)
{
	D3DX11_IMAGE_FILE_FORMAT fFormat = xD11ConstLexer::GetDX11ImageFileFormat(fileName);
	if((int)fFormat == -1)
		return false;

	if(pResource == NULL)
		return false;

	HRESULT hr = D3DX11SaveTextureToFileW(m_pD11RenderApi->d11DeviceContext() , pResource ,  fFormat , fileName);
    return SUCCEEDED(hr);
}

xD11BaseRenderTarget::~xD11BaseRenderTarget()
{

}


END_NAMESPACE_XEVOL3D


