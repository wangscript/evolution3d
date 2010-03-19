#include "stdafx.h"
#include "xDIRECT3D9API.h"
#include "xD3D9BaseRenderTarget.h"
#include "xD3D9BaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D

xD3D9BaseRenderTarget::xD3D9BaseRenderTarget(xD3D9RenderApi* pD11Api , int arraySlice , int mipmapLevel):IRenderTarget(pD11Api) 
{
	m_pD3D9RenderApi = pD11Api ; 
	m_pBaseTexture = NULL;
	m_RefCount  = 1;
    m_ArraySlice  = arraySlice;
    m_MipmapLevel = mipmapLevel;
    m_pRenderTargetView = NULL;
    REGISTER_DEVICE_LOST_RESOURCE(m_pD3D9RenderApi);

}

bool xD3D9BaseRenderTarget::grabRenderTagetData(void* pData , int x , int y , int w , int h  )
{
	xD3D9BaseTexture* pD10Texture = (xD3D9BaseTexture*)m_pBaseTexture;
	return pD10Texture->grabRenderTagetData(pData , x , y , w , h , m_ArraySlice , m_MipmapLevel);
}

void xD3D9BaseRenderTarget::setTexture(IBaseTexture* pTexture)
{
	m_pBaseTexture = pTexture;
}

bool xD3D9BaseRenderTarget::saveResourceToFile(const wchar_t* fileName , IDirect3DSurface9* pResource)
{
	D3DXIMAGE_FILEFORMAT fFormat = xD3D9ConstLexer::GetDXImageFileFormat(fileName);
	if((int)fFormat == -1)
		return false;

	if(pResource == NULL)
		return false;

	HRESULT hr = D3DXSaveSurfaceToFileW(fileName , fFormat , pResource , NULL , NULL );// , pResource ,  fFormat , fileName);
    return SUCCEEDED(hr);
}

xD3D9BaseRenderTarget::~xD3D9BaseRenderTarget()
{
    REMOVE_DEVICE_LOST_RESOURCE(m_pD3D9RenderApi);
    XSAFE_RELEASE(m_pRenderTargetView);
}


END_NAMESPACE_XEVOL3D


