#include "xDirect3D10API.h"
#include "xD10BaseTexture.h"
#include "BaseLib/xBaseMath.h"
BEGIN_NAMESPACE_XEVOL3D


xD10TextureBuffer::xD10TextureBuffer()
{
	m_pitch = 0;
	m_pixel  = NULL;
	m_dataLen = 0;
	m_destBuffer = 0;
}
xD10TextureBuffer::~xD10TextureBuffer()
{
	XSAFE_DELETE_ARRAY(m_pixel);
}

void  xD10TextureBuffer::setDestBuffer(void* destBuffer)
{
	m_destBuffer = destBuffer;
}

bool  xD10TextureBuffer::create(int h , int pitch)
{
	m_dataLen = h * pitch;
	m_pitch = pitch;
	m_pixel = new char[m_dataLen];
    memset(m_pixel , 0 , m_dataLen);
	return true;
}

bool  xD10TextureBuffer::unload()
{
	XSAFE_DELETE_ARRAY(m_pixel);
	m_pitch = 0;
	m_dataLen = 0;
	m_destBuffer = 0;
	return true;
}

bool  xD10TextureBuffer::flush(int h , int dest_pitch , int src_pitch )
{
	if(src_pitch == 0) src_pitch = m_pitch;
	memcpy(m_destBuffer , m_pixel , dest_pitch * h);
	m_destBuffer = NULL;
	return true;
}


xD10BaseTexture::xD10BaseTexture(xD3D10RenderApi* pD10Api):IBaseTexture(pD10Api) 
{
	m_RefCount = 1;
	m_pD10Api = pD10Api ; 
}


xD10BaseTexture::~xD10BaseTexture()
{

}


unsigned long xD10BaseTexture::memUsage()
{
	return m_TexInfo.m_MemSize ; 
}

eResourceType xD10BaseTexture::res_type()
{
	return m_TexInfo.m_Type ; 
}

 bool xD10BaseTexture::desc(xTextureDesc& _desc)
 {
	 _desc.m_depth      = (int)m_TexInfo.m_TexDepth;
	 _desc.m_fmt        =      m_TexInfo.m_xfmt;
	 _desc.m_nArraySize = (int)m_TexInfo.m_ArraySize;
	 _desc.m_width      = (int)m_TexInfo.m_TexWidth;
	 _desc.m_height     = (int)m_TexInfo.m_TexHeight;
	 return true;
 }
bool xD10BaseTexture::unload()
{
    return true;
}
 //////////////////////////////////////////////////////////////////////////
 xD10UnkwonTexture::xD10UnkwonTexture(xD3D10RenderApi* pD10Api) : xD10BaseTexture(pD10Api) 
 {
	 m_pTextureView = NULL;
	 m_pTexture  = NULL;
	 memset(&m_TexInfo, 0 ,sizeof(m_TexInfo));
 }
 bool xD10UnkwonTexture::isLoaded()
 {
	 return m_pTexture != NULL && m_pTextureView != NULL;
 }

 bool xD10UnkwonTexture::unload()
 {
	 XSAFE_RELEASE(m_pTextureView);
	 XSAFE_RELEASE(m_pTexture);
     xD10BaseTexture::unload();
	 return !isLoaded();
 }

 xD10UnkwonTexture::~xD10UnkwonTexture()
 {
     unload();
 }

 void* xD10UnkwonTexture::handle()
 {
	 return m_pTextureView;
 }

 bool xD10UnkwonTexture::_load(ID3D10Resource* pTexture , bool bCreateTextureView )
 {
	 m_pTexture = pTexture;
	 D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	 D3D10_RESOURCE_DIMENSION type;
	 m_pTexture->GetType( &type );

	 m_TexInfo.m_Pitch = 0;
	 m_TexInfo.m_SlicePitch = 0;

	 switch( type )
	 {
	 case D3D10_RESOURCE_DIMENSION_BUFFER:
		 //...
		 break;
	 case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
		 {
			 D3D10_TEXTURE1D_DESC desc;
			 ID3D10Texture1D *pTexture1D = (ID3D10Texture1D*)m_pTexture;
			 pTexture1D->GetDesc( &desc );
             if(m_TexInfo.m_ShaderViewFmt == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ShaderViewFmt = desc.Format;
             if(m_TexInfo.m_RTViewFmt  == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_RTViewFmt  = desc.Format;
             if(m_TexInfo.m_ResFmt     == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ResFmt     = desc.Format;
			 if(bCreateTextureView)
			 {
        		 srvDesc.Format = m_TexInfo.m_ShaderViewFmt;
				 srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
				 srvDesc.Texture2D.MostDetailedMip = 0;
				 srvDesc.Texture2D.MostDetailedMip = desc.MipLevels;
				 ID3D10ShaderResourceView *pSRView = NULL;
				 m_pD10Api->d10Device()->CreateShaderResourceView( m_pTexture, &srvDesc, &pSRView );
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
	 case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
		 {
			 D3D10_TEXTURE2D_DESC desc;
			 ID3D10Texture2D *pTexture2D = (ID3D10Texture2D*)m_pTexture;
			 pTexture2D->GetDesc( &desc );
             if(m_TexInfo.m_ShaderViewFmt == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ShaderViewFmt = desc.Format;
             if(m_TexInfo.m_RTViewFmt  == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_RTViewFmt  = desc.Format;
             if(m_TexInfo.m_ResFmt     == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ResFmt     = desc.Format;
			 if(bCreateTextureView)
			 {
				 srvDesc.Format = m_TexInfo.m_ShaderViewFmt;
				 srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
				 srvDesc.Texture2D.MostDetailedMip = 0;
				 srvDesc.Texture2D.MipLevels       = desc.MipLevels;
				 ID3D10ShaderResourceView *pSRView = NULL;
				 m_pD10Api->d10Device()->CreateShaderResourceView( m_pTexture, &srvDesc, &pSRView );
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

	 case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
		 {
			 D3D10_TEXTURE3D_DESC desc;
			 ID3D10Texture3D *pTexture3D = (ID3D10Texture3D*)m_pTexture;
			 pTexture3D->GetDesc( &desc );
             if(m_TexInfo.m_ShaderViewFmt == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ShaderViewFmt = desc.Format;
             if(m_TexInfo.m_RTViewFmt  == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_RTViewFmt  = desc.Format;
             if(m_TexInfo.m_ResFmt     == DXGI_FORMAT_UNKNOWN) m_TexInfo.m_ResFmt     = desc.Format;
			 if(bCreateTextureView)
			 {
				 srvDesc.Format = m_TexInfo.m_ShaderViewFmt;
				 srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE3D;
				 srvDesc.Texture2D.MostDetailedMip = 0;
				 srvDesc.Texture2D.MostDetailedMip = desc.MipLevels;
				 ID3D10ShaderResourceView *pSRView = NULL;
				 m_pD10Api->d10Device()->CreateShaderResourceView( m_pTexture, &srvDesc, &pSRView );
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
	 xD10GIFormatInfo* pFormat = xD10ConstLexer::singleton()->GetPixelFormat( m_TexInfo.m_ShaderViewFmt );
	 m_TexInfo.m_nBytePerPixel = pFormat->m_compont * pFormat->m_bytePerComponent;
	 m_TexInfo.m_xfmt = pFormat->m_fmt;
	 unsigned int iPitch = (unsigned int) (m_TexInfo.m_TexWidth  * pFormat->m_compont * pFormat->m_bytePerComponent );
	 m_TexInfo.m_Pitch = xFloorToPower2( iPitch );
	 m_TexInfo.m_SlicePitch = xFloorToPower2(m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight );
	 m_TexInfo.m_MemSize = int( m_TexInfo.m_Pitch * m_TexInfo.m_ArraySize * m_TexInfo.m_TexDepth * m_TexInfo.m_TexHeight );
	 return true;
 }
END_NAMESPACE_XEVOL3D


