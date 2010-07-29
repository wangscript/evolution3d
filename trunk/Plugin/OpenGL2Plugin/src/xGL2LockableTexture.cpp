#include "xGL2LockableTexture.h"
#include "xGL2ConstLexer.h"
#include "xOpenGL2API.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xGL2LockableTexture  , xGL2BaseTexture);


xGL2LockableTexture::xGL2LockableTexture(xGL2RenderApi* pAPI , bool bReadable)
:xGL2UnkwonTexture(pAPI)
{
	m_bReadable = bReadable;
	m_pTextureView      = NULL;
	m_pTexture          = NULL;
	m_2DTexture = NULL;
	m_3DTexture = NULL;
	memset(&m_TexInfo, 0 ,sizeof(m_TexInfo));
}

xGL2LockableTexture::~xGL2LockableTexture()
{
	unload();
}

void* xGL2LockableTexture::handle()
{
	return m_pTextureView;
}

bool xGL2LockableTexture::isLoaded()
{
	return m_pTexture != NULL && m_pTextureView != NULL;
}

bool xGL2LockableTexture::unload()
{
	m_texBuffer.unload();
	XSAFE_RELEASE(m_pTextureView);
	XSAFE_RELEASE(m_pTexture);
	XSAFE_RELEASE(m_2DTexture);
	XSAFE_RELEASE(m_3DTexture);
	return !isLoaded();
}


bool xGL2LockableTexture::load(const wchar_t* fileName , unsigned long  arg)
{
	unload();
	D3DX10_IMAGE_LOAD_INFO loadInfo;
	fillLoadInfo(loadInfo, true );
	ID3D10Resource* pTexture = NULL;
	D3DX10CreateTextureFromFileW( m_pGL2Api->d10Device() , fileName ,  &loadInfo , NULL, &pTexture, NULL );
	if(pTexture)
    {
        m_TexInfo.m_ShaderViewFmt    = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_ResFmt        = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_RTViewFmt     = DXGI_FORMAT_UNKNOWN;
		return _load(pTexture);
    }
	return false;
}

bool xGL2LockableTexture::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();

	if(bufLen == 0 || buf == NULL)
		return load(fileName , arg);

	D3DX10_IMAGE_LOAD_INFO loadInfo;
	fillLoadInfo( loadInfo, true );
	ID3D10Resource* pTexture = NULL;
	D3DX10CreateTextureFromMemory(m_pGL2Api->d10Device() , buf , bufLen , &loadInfo, NULL, &pTexture, NULL );
	if(pTexture)
    {
        m_TexInfo.m_ShaderViewFmt    = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_ResFmt        = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_RTViewFmt     = DXGI_FORMAT_UNKNOWN;
		return _load(pTexture);
    }

	return false;
}

bool  xGL2LockableTexture::_create2DTexture()
{
	GL2_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)m_TexInfo.m_TexWidth;
	desc.Height           = (UINT)m_TexInfo.m_TexHeight;
	desc.MipLevels        = (UINT)m_TexInfo.m_MipmapLevel;
	desc.ArraySize        = (UINT)m_TexInfo.m_ArraySize;
	desc.Format           = m_TexInfo.m_ShaderViewFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.BindFlags        = GL2_BIND_SHADER_RESOURCE  ;//| GL2_BIND_RENDER_TARGET; 
	desc.Usage            = GL2_USAGE_DYNAMIC;
	desc.CPUAccessFlags   = GL2_CPU_ACCESS_WRITE;
	if(m_bReadable)
	{
		//desc.CPUAccessFlags   |= GL2_CPU_ACCESS_READ;
	}

	m_pGL2Api->d10Device()->CreateTexture2D( &desc, NULL, &m_2DTexture );
	if(m_2DTexture == NULL)
		return false;
	m_2DTexture->AddRef();

	bool bRet =  _load(m_2DTexture);

	GL2_MAPPED_TEXTURE2D mappedText;
	m_2DTexture->Map(0,GL2_MAP_WRITE_DISCARD ,0, &mappedText);
	m_2DTexture->Unmap(0);
	m_TexInfo.m_Pitch = mappedText.RowPitch;
	m_TexInfo.m_SlicePitch = m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight;
	if(m_bReadable && bRet)
	{
		m_texBuffer.create(m_TexInfo.m_Pitch , (int)m_TexInfo.m_TexHeight) ;
	}
	return bRet ;
}


bool  xGL2LockableTexture::create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels , int arraySize) 
{ 
	unload();
    
	xGL2GIFormatInfo* pFmtInfo = xGL2ConstLexer::singleton()->GetPixelFormat(fmt);
	m_TexInfo.m_ArraySize = arraySize;
    m_TexInfo.m_ShaderViewFmt    = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ResFmt        = pFmtInfo->m_dxfmt;
    m_TexInfo.m_RTViewFmt     = pFmtInfo->m_dxfmt;
	m_TexInfo.m_MemSize   = 0;
	m_TexInfo.m_MipmapLevel = mipMapLevels;
	m_TexInfo.m_TexDepth    = 1;
	m_TexInfo.m_TexHeight   = h;
	m_TexInfo.m_TexWidth    = w;
	m_TexInfo.m_Type        = RESOURCE_TEXTURE2D;
	m_TexInfo.m_xfmt        = fmt;
    m_TexInfo.m_nBytePerPixel = pFmtInfo->m_compont * pFmtInfo->m_byte;
	return _create2DTexture();
}


bool xGL2LockableTexture::_create3DTexture()
{
	GL2_TEXTURE3D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)m_TexInfo.m_TexWidth;
	desc.Height           = (UINT)m_TexInfo.m_TexHeight;
	desc.MipLevels        = (UINT)m_TexInfo.m_MipmapLevel;
	desc.Depth            = (UINT)m_TexInfo.m_TexDepth;
	desc.Format           = m_TexInfo.m_ResFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.BindFlags        = GL2_BIND_SHADER_RESOURCE  ;//| GL2_BIND_RENDER_TARGET; 
	desc.Usage            = GL2_USAGE_DYNAMIC;
	desc.CPUAccessFlags   = GL2_CPU_ACCESS_WRITE;
	if(m_bReadable)
	{
		//desc.CPUAccessFlags   |= GL2_CPU_ACCESS_READ;
	}

	m_pGL2Api->d10Device()->CreateTexture3D( &desc, NULL, &m_3DTexture );
	if(m_3DTexture == NULL)
		return false;
	m_3DTexture->AddRef();
	bool bRet = _load(m_3DTexture);

	GL2_MAPPED_TEXTURE3D mappedText;
	m_3DTexture->Map(0 , GL2_MAP_WRITE_DISCARD ,0, &mappedText);
	m_3DTexture->Unmap(0);
	m_TexInfo.m_Pitch = mappedText.RowPitch;
	m_TexInfo.m_SlicePitch = mappedText.DepthPitch;
	if(m_bReadable && bRet)
	{
		m_texBuffer.create(m_TexInfo.m_SlicePitch , (int)m_TexInfo.m_TexDepth ) ;
	}
	return bRet;
}


bool  xGL2LockableTexture::create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels , int arraySize)
{
	unload();

	xGL2GIFormatInfo* pFmtInfo = xGL2ConstLexer::singleton()->GetPixelFormat(fmt);
	m_TexInfo.m_ArraySize = arraySize;
	m_TexInfo.m_ShaderViewFmt    = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ResFmt        = pFmtInfo->m_dxfmt;
    m_TexInfo.m_RTViewFmt     = pFmtInfo->m_dxfmt;
	m_TexInfo.m_MemSize   = 0;
	m_TexInfo.m_MipmapLevel = mipMapLevels;
	m_TexInfo.m_TexDepth    = depth;
	m_TexInfo.m_TexHeight   = h;
	m_TexInfo.m_TexWidth    = w;
	m_TexInfo.m_Type        = RESOURCE_TEXTURE2D;
	m_TexInfo.m_xfmt        = fmt;
	m_TexInfo.m_nBytePerPixel = pFmtInfo->m_compont * pFmtInfo->m_byte;
	if(depth == 1) return _create2DTexture();
	else return _create3DTexture();
}

bool  xGL2LockableTexture::update(void* data  , int dateLen , int mipmapLevel , int arraySlice)
{
	if(m_pTexture == NULL)
		return false;

	UINT lockResource = GL2CalcSubresource((UINT)mipmapLevel , (UINT)arraySlice, (UINT)m_TexInfo.m_MipmapLevel);
	char* pMapedData = NULL;
	int   Pitch = 0;
	int   SlicePith = 0;
	if(m_3DTexture) 
	{
		GL2_MAPPED_TEXTURE3D mappedTex;
		mappedTex.pData = NULL;
		mappedTex.RowPitch = 0;
		mappedTex.DepthPitch = 0;
		m_3DTexture->Map( lockResource  , GL2_MAP_WRITE_DISCARD, 0, &mappedTex );
		pMapedData = (char*)mappedTex.pData;
		Pitch     = mappedTex.RowPitch;
		SlicePith = mappedTex.DepthPitch;
	}
	if(m_2DTexture) 
	{
		GL2_MAPPED_TEXTURE2D mappedTex;
		mappedTex.pData = NULL;
		mappedTex.RowPitch = 0;
		m_2DTexture->Map( lockResource  , GL2_MAP_WRITE_DISCARD, 0, &mappedTex );
		pMapedData = (char*)mappedTex.pData;
		Pitch = mappedTex.RowPitch;
		SlicePith = 0;
	}
	if(pMapedData == NULL)
		return false;
	memcpy(pMapedData , data , dateLen);
	if(m_3DTexture) m_3DTexture->Unmap(lockResource);
	if(m_2DTexture) m_2DTexture->Unmap(lockResource);
	return true;
}


bool xGL2LockableTexture::lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo  , int mipmapLevel , int arraySlice)
{
	if(m_pTexture == NULL)
		return false;
	UINT lockResource = GL2CalcSubresource((UINT)mipmapLevel , (UINT)arraySlice, (UINT)m_TexInfo.m_MipmapLevel);
	GL2_MAP  mapType = GL2_MAP_WRITE_DISCARD;
	switch(lockPolicy)
	{
	case eLock_WriteDiscard:      mapType = GL2_MAP_WRITE_DISCARD; break;
	case eLock_Read:    		  mapType = GL2_MAP_READ; break;
	case eLock_Write:    		  mapType = GL2_MAP_WRITE; break;
	case eLock_ReadWrite:    	  mapType = GL2_MAP_READ_WRITE; break;
	case eLock_WriteNoOverwrite:  mapType = GL2_MAP_WRITE_NO_OVERWRITE; break;
	}
    lockInfo.m_lockResource = (long)lockResource;

	if(m_2DTexture)
	{
		GL2_MAPPED_TEXTURE2D mappedTex;
		mappedTex.pData = NULL;
		mappedTex.RowPitch = 0;
		m_2DTexture->Map( lockResource  , mapType, 0, &mappedTex );
		lockInfo.m_picth       = (int)mappedTex.RowPitch ;
		lockInfo.m_pixels      = (char*)mappedTex.pData;
		lockInfo.m_slice_pitch = (int)(lockInfo.m_picth * m_TexInfo.m_TexHeight);
	}
	else
	{
		GL2_MAPPED_TEXTURE3D mappedTex;
		mappedTex.pData = NULL;
		mappedTex.RowPitch = 0;
		mappedTex.DepthPitch = 0;
		m_3DTexture->Map( lockResource  , mapType, 0, &mappedTex );
		lockInfo.m_picth       = (int)mappedTex.RowPitch ;
		lockInfo.m_pixels      = (char*)mappedTex.pData;
		lockInfo.m_slice_pitch = (int)mappedTex.DepthPitch;
	}
	
	if(lockInfo.m_pixels == NULL)
		return false;

    lockInfo.m_width       = (int)m_TexInfo.m_TexWidth;
	lockInfo.m_height      = (int)m_TexInfo.m_TexHeight;
	lockInfo.m_depth       = (int)m_TexInfo.m_TexDepth;

	//可读和非可读的。
	if(m_bReadable)
	{
		m_texBuffer.setDestBuffer(lockInfo.m_pixels);
		lockInfo.m_pixels      = m_texBuffer.m_pixel;
	}
	return true;
}


bool xGL2LockableTexture::unlock(xTextureLockArea& lockInfo)
{
	int lockResource = (int)lockInfo.m_lockResource;
	if(m_pTexture == NULL)
		return false;
	if(m_bReadable)
	{
		if(m_2DTexture)  
		{
			m_texBuffer.flush(lockInfo.m_height , lockInfo.m_picth       , lockInfo.m_picth);
		}
		else if(m_3DTexture) 
		{
			m_texBuffer.flush(lockInfo.m_depth  , lockInfo.m_slice_pitch , lockInfo.m_slice_pitch);
		}
	}
	if(m_2DTexture) m_2DTexture->Unmap( lockResource );
	if(m_3DTexture) m_3DTexture->Unmap( lockResource );
	//D3DX10SaveTextureToFileW(m_pTexture,D3DX10_IMAGE_FILE_FORMAT::D3DX10_IFF_PNG , _XEVOL_ABSPATH_(L".\\Profile\\test.png") );
	return true;
}

IRenderTarget* xGL2LockableTexture::toRenderTarget(size_t iSlice  , size_t iMipMapLevel )
{
	if(iSlice != 0 || iMipMapLevel != 0)
		return NULL;
	return NULL;
}

END_NAMESPACE_XEVOL3D