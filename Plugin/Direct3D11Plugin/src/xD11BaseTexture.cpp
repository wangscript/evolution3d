#include "stdafx.h"
#include "xDirect3D11API.h"
#include "xD11BaseTexture.h"
#include "BaseLib/xBaseMath.h"
BEGIN_NAMESPACE_XEVOL3D


xD11TextureBuffer::xD11TextureBuffer()
{
	m_pitch = 0;
	m_pixel  = NULL;
	m_dataLen = 0;
	m_destBuffer = 0;
}
xD11TextureBuffer::~xD11TextureBuffer()
{
	XSAFE_DELETE_ARRAY(m_pixel);
}

void  xD11TextureBuffer::setDestBuffer(void* destBuffer)
{
	m_destBuffer = destBuffer;
}

bool  xD11TextureBuffer::create(int h , int pitch)
{
	m_dataLen = h * pitch;
	m_pitch = pitch;
	m_pixel = new char[m_dataLen];
    memset(m_pixel , 0 , m_dataLen);
	return true;
}

bool  xD11TextureBuffer::unload()
{
	XSAFE_DELETE_ARRAY(m_pixel);
	m_pitch = 0;
	m_dataLen = 0;
	m_destBuffer = 0;
	return true;
}

bool  xD11TextureBuffer::flush(int h , int dest_pitch , int src_pitch )
{
	if(src_pitch == 0) src_pitch = m_pitch;
	memcpy(m_destBuffer , m_pixel , dest_pitch * h);
	m_destBuffer = NULL;
	return true;
}


xD11BaseTexture::xD11BaseTexture(xD3D11RenderApi* pD11Api):IBaseTexture(pD11Api) 
{
	m_SampleDesc.Count = 1;
	m_SampleDesc.Quality = 0;
	m_RefCount = 1;
	m_pD11RenderApi = pD11Api ; 
}


xD11BaseTexture::~xD11BaseTexture()
{

}


unsigned long xD11BaseTexture::memUsage()
{
	return m_TexInfo.m_MemSize ; 
}

eResourceType xD11BaseTexture::res_type()
{
	return m_TexInfo.m_Type ; 
}

 bool xD11BaseTexture::desc(xTextureDesc& _desc)
 {
	 _desc.m_depth      = (int)m_TexInfo.m_TexDepth;
	 _desc.m_fmt        =      m_TexInfo.m_xfmt;
	 _desc.m_nArraySize = (int)m_TexInfo.m_ArraySize;
	 _desc.m_width      = (int)m_TexInfo.m_TexWidth;
	 _desc.m_height     = (int)m_TexInfo.m_TexHeight;
	 return true;
 }

ID3D11Resource*  xD11BaseTexture::GetD11Resource()
{
	ID3D11Resource* pRes = NULL;
	if(m_pTextureView == NULL)
		return NULL;
	m_pTextureView->GetResource(&pRes);
	return pRes;
}

bool  xD11BaseTexture::saveToFile(const wchar_t* fileName)
{
	D3DX11_IMAGE_FILE_FORMAT fFormat = xD11ConstLexer::GetDX11ImageFileFormat(fileName);
	if((int)fFormat == -1)
		return false;

	ID3D11Resource* pResource = this->GetD11Resource();
	if(pResource == NULL)
		return false;

	HRESULT hr = D3DX11SaveTextureToFileW(m_pD11RenderApi->d11DeviceContext() ,  pResource ,  fFormat , fileName);
	pResource->Release();
	return SUCCEEDED(hr);
}

bool xD11BaseTexture::unload()
{
    return true;
}
 //////////////////////////////////////////////////////////////////////////
 xD11UnkwonTexture::xD11UnkwonTexture(xD3D11RenderApi* pD11Api) : xD11BaseTexture(pD11Api) 
 {
	 m_pTextureView = NULL;
	 m_pTexture  = NULL;
	 memset(&m_TexInfo, 0 ,sizeof(m_TexInfo));
 }
 bool xD11UnkwonTexture::isLoaded()
 {
	 return m_pTexture != NULL && m_pTextureView != NULL;
 }

 bool xD11UnkwonTexture::unload()
 {
	 XSAFE_RELEASE(m_pTextureView);
	 XSAFE_RELEASE(m_pTexture);
     xD11BaseTexture::unload();
	 return !isLoaded();
 }

 xD11UnkwonTexture::~xD11UnkwonTexture()
 {
     unload();
 }

 void* xD11UnkwonTexture::handle()
 {
	 return m_pTextureView;
 }

 bool xD11UnkwonTexture::_load(ID3D11Resource* pTexture , bool bCreateTextureView )
 {
	 m_pTexture = pTexture;
	 D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	 D3D11_RESOURCE_DIMENSION type;
	 m_pTexture->GetType( &type );

	 m_TexInfo.m_Pitch = 0;
	 m_TexInfo.m_SlicePitch = 0;

	 switch( type )
	 {
	 case D3D11_RESOURCE_DIMENSION_BUFFER:
		 //...
		 break;
	 case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		 {
			 D3D11_TEXTURE1D_DESC desc;
			 ID3D11Texture1D *pTexture1D = (ID3D11Texture1D*)m_pTexture;
			 pTexture1D->GetDesc( &desc );
             if(m_TexInfo.m_ShaderViewFmt == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ShaderViewFmt = desc.Format;
             if(m_TexInfo.m_RTViewFmt  == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_RTViewFmt  = desc.Format;
             if(m_TexInfo.m_ResFmt     == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ResFmt     = desc.Format;
			 if(bCreateTextureView)
			 {
        		 srvDesc.Format = m_TexInfo.m_ShaderViewFmt;
				 srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				 srvDesc.Texture2D.MostDetailedMip = 0;
				 srvDesc.Texture2D.MostDetailedMip = desc.MipLevels;
				 ID3D11ShaderResourceView *pSRView = NULL;
				 m_pD11RenderApi->d11Device()->CreateShaderResourceView( m_pTexture, &srvDesc, &pSRView );
				 if(pSRView == NULL)
				 {
					 unload();
					 return false;
				 }
				 m_pTextureView = pSRView;
			 }			 

			 //野割
			 m_TexInfo.m_ArraySize   = desc.ArraySize;
			 m_TexInfo.m_TexWidth    = desc.Width;
			 m_TexInfo.m_TexHeight   = 1;
			 m_TexInfo.m_TexDepth    = 1;
			 m_TexInfo.m_MipmapLevel = desc.MipLevels;
			 m_TexInfo.m_Type        = RESOURCE_TEXTURE1D;
		 }
		 break;
	 case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		 {
			 XD3D11_TEXTURE2D_DESC desc;
			 ID3D11Texture2D *pTexture2D = (ID3D11Texture2D*)m_pTexture;
			 pTexture2D->GetDesc( &desc );
             if(m_TexInfo.m_ShaderViewFmt == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ShaderViewFmt = desc.Format;
             if(m_TexInfo.m_RTViewFmt  == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_RTViewFmt  = desc.Format;
             if(m_TexInfo.m_ResFmt     == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ResFmt     = desc.Format;
			 if(bCreateTextureView)
			 {
				 srvDesc.Format = m_TexInfo.m_ShaderViewFmt;
				 srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				 srvDesc.Texture2D.MostDetailedMip = 0;
				 srvDesc.Texture2D.MipLevels       = desc.MipLevels;
				 ID3D11ShaderResourceView *pSRView = NULL;
				 m_pD11RenderApi->d11Device()->CreateShaderResourceView( m_pTexture, &srvDesc, &pSRView );
				 if(pSRView == NULL)
				 {
					 unload();
					 return false;
				 }
				 m_pTextureView = pSRView;
			 }

			 //野割
			 m_TexInfo.m_ArraySize   = desc.ArraySize;
			 m_TexInfo.m_TexWidth    = desc.Width;
			 m_TexInfo.m_TexHeight   = desc.Height;
			 m_TexInfo.m_TexDepth    = 1;
			 m_TexInfo.m_MipmapLevel = desc.MipLevels;
			 m_TexInfo.m_Type        = RESOURCE_TEXTURE2D;
		 }
		 break;

	 case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		 {
			 D3D11_TEXTURE3D_DESC desc;
			 ID3D11Texture3D *pTexture3D = (ID3D11Texture3D*)m_pTexture;
			 pTexture3D->GetDesc( &desc );
             if(m_TexInfo.m_ShaderViewFmt == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ShaderViewFmt = desc.Format;
             if(m_TexInfo.m_RTViewFmt  == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_RTViewFmt  = desc.Format;
             if(m_TexInfo.m_ResFmt     == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ResFmt     = desc.Format;
			 if(bCreateTextureView)
			 {
				 srvDesc.Format = m_TexInfo.m_ShaderViewFmt;
				 srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
				 srvDesc.Texture2D.MostDetailedMip = 0;
				 srvDesc.Texture2D.MostDetailedMip = desc.MipLevels;
				 ID3D11ShaderResourceView *pSRView = NULL;
				 m_pD11RenderApi->d11Device()->CreateShaderResourceView( m_pTexture, &srvDesc, &pSRView );
				 if(pSRView == NULL)
				 {
					 unload();
					 return false;
				 }
				 m_pTextureView = pSRView;
			 }

			 //野割
			 m_TexInfo.m_ArraySize   = 1;
			 m_TexInfo.m_TexWidth    = desc.Width;
			 m_TexInfo.m_TexHeight   = desc.Height;
			 m_TexInfo.m_TexDepth    = desc.Depth;
			 m_TexInfo.m_MipmapLevel = desc.MipLevels;
			 m_TexInfo.m_Type        = RESOURCE_TEXTURE3D;
		 }
		 break;
	 default:
		 return false;
		 break;
	 }

	 //===================================
	 xD11GIFormatInfo* pFormat = xD11ConstLexer::singleton()->GetPixelFormat( m_TexInfo.m_ShaderViewFmt );
	 m_TexInfo.m_nBytePerPixel = pFormat->m_compont * pFormat->m_bytePerComponent;
	 m_TexInfo.m_xfmt = pFormat->m_fmt;
	 unsigned int iPitch = (unsigned int) (m_TexInfo.m_TexWidth  * pFormat->m_compont * pFormat->m_bytePerComponent );
	 m_TexInfo.m_Pitch = xFloorToPower2( iPitch );
	 m_TexInfo.m_SlicePitch = xFloorToPower2(m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight );
	 m_TexInfo.m_MemSize = int( m_TexInfo.m_Pitch * m_TexInfo.m_ArraySize * m_TexInfo.m_TexDepth * m_TexInfo.m_TexHeight );
	 return true;
 }
END_NAMESPACE_XEVOL3D


