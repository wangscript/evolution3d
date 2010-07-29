#include "stdafx.h"
#include "xD11LockableTexture.h"
#include "xD11ConstLexer.h"
#include "xDirect3D11API.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD11LockableTexture  , xD11BaseTexture);


xD11LockableTexture::xD11LockableTexture(xD3D11RenderApi* pAPI , bool bReadable)
:xD11UnkwonTexture(pAPI)
{
	m_bReadable = bReadable;
	m_pTextureView      = NULL;
	m_pTexture          = NULL;
	m_2DTexture = NULL;
	m_3DTexture = NULL;
	memset(&m_TexInfo, 0 ,sizeof(m_TexInfo));
}

xD11LockableTexture::~xD11LockableTexture()
{
	unload();
}

void* xD11LockableTexture::handle()
{
	return m_pTextureView;
}

bool xD11LockableTexture::isLoaded()
{
	return m_pTexture != NULL && m_pTextureView != NULL;
}

bool xD11LockableTexture::unload()
{
	m_texBuffer.unload();
	XSAFE_RELEASE(m_pTextureView);
	XSAFE_RELEASE(m_pTexture);
	XSAFE_RELEASE(m_2DTexture);
	XSAFE_RELEASE(m_3DTexture);
	return !isLoaded();
}


bool xD11LockableTexture::load(const wchar_t* fileName , unsigned long  arg)
{
	unload();
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	fillLoadInfo(loadInfo, true ,true);
	ID3D11Resource* pTexture = NULL;
	D3DX11CreateTextureFromFileW( m_pD11RenderApi->d11Device() , fileName ,  &loadInfo , NULL, &pTexture, NULL );
	if(pTexture)
    {
        m_TexInfo.m_ShaderViewFmt    = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_ResFmt        = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_RTViewFmt     = DXGI_FORMAT_UNKNOWN;
		return _load(pTexture);
    }
	return false;
}

bool xD11LockableTexture::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();

	if(bufLen == 0 || buf == NULL)
		return load(fileName , arg);

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	fillLoadInfo( loadInfo, true ,true);
	ID3D11Resource* pTexture = NULL;
	D3DX11CreateTextureFromMemory(m_pD11RenderApi->d11Device() , buf , bufLen , &loadInfo, NULL, &pTexture, NULL );
	if(pTexture)
    {
        m_TexInfo.m_ShaderViewFmt    = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_ResFmt        = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_RTViewFmt     = DXGI_FORMAT_UNKNOWN;
		return _load(pTexture);
    }

	return false;
}

bool  xD11LockableTexture::create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData) 
{ 
    unload();

    xD11GIFormatInfo* pFmtInfo = xD11ConstLexer::singleton()->GetPixelFormat(initDesc.m_TextureDesc.m_fmt);
    m_TexInfo.m_ArraySize = abs(initDesc.m_TextureDesc.m_nArraySize);
    m_TexInfo.m_ShaderViewFmt    = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ResFmt        = pFmtInfo->m_dxfmt;
    m_TexInfo.m_RTViewFmt     = pFmtInfo->m_dxfmt;
    m_TexInfo.m_MemSize   = 0;
    m_TexInfo.m_MipmapLevel = initDesc.m_nMipmap;
    m_TexInfo.m_TexDepth    = 1;
    m_TexInfo.m_TexHeight   = initDesc.m_TextureDesc.m_height;
    m_TexInfo.m_TexWidth    = initDesc.m_TextureDesc.m_width;
    m_TexInfo.m_Type        = initDesc.m_TextureDesc.m_depth == 1 ? RESOURCE_TEXTURE2D : RESOURCE_TEXTURE3D;
    m_TexInfo.m_xfmt        = initDesc.m_TextureDesc.m_fmt;
    m_TexInfo.m_nBytePerPixel = pFmtInfo->m_compont * pFmtInfo->m_bytePerComponent;
    if(initDesc.m_TextureDesc.m_depth == 1)
        return _create2DTexture(initDesc , pInitData , nInitData);
    return _create3DTexture(initDesc , pInitData , nInitData);
}


bool  xD11LockableTexture::_create2DTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData)
{
	XD3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)m_TexInfo.m_TexWidth;
	desc.Height           = (UINT)m_TexInfo.m_TexHeight;
	desc.MipLevels        = (UINT)m_TexInfo.m_MipmapLevel;
	desc.ArraySize        = (UINT)m_TexInfo.m_ArraySize;
	desc.Format           = m_TexInfo.m_ShaderViewFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc       = m_SampleDesc;
   
    desc.BindFlags        = xD11ConstLexer::BindFlag(initDesc.m_bindType)  ;//| D3D11_BIND_RENDER_TARGET; 
    desc.Usage            = xD11ConstLexer::Usage(initDesc.m_usage);
    desc.CPUAccessFlags   = xD11ConstLexer::AccessFlag(initDesc.m_access);
    m_TexInfo.m_Usage     = desc.Usage;

    if(initDesc.m_TextureDesc.m_nArraySize == eTextureArray_CubeMap)
    {
        desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
    }

    D3D11_SUBRESOURCE_DATA* pDxInitData = NULL;
    if(pInitData )
    {
        pDxInitData = new D3D11_SUBRESOURCE_DATA [ nInitData ] ;
        for(int i = 0 ; i < nInitData ; i ++)
        {
            pDxInitData[i].pSysMem          = pInitData[i].m_pData;
            pDxInitData[i].SysMemPitch      = pInitData[i].m_Pitch;
            pDxInitData[i].SysMemSlicePitch = 0;
        }   
    }
	m_pD11RenderApi->d11Device()->CreateTexture2D( &desc, NULL, &m_2DTexture );

    if(pDxInitData) delete [] pDxInitData;


	if(m_2DTexture == NULL)
		return false;
	m_2DTexture->AddRef();

	bool bRet =  _load(m_2DTexture);

	D3D11_MAPPED_SUBRESOURCE mappedText;
    mappedText.pData = NULL;
    if(m_TexInfo.m_Usage != D3D11_USAGE_DEFAULT)
    {
        m_pD11RenderApi->d11DeviceContext()->Map(m_2DTexture , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedText);
        m_pD11RenderApi->d11DeviceContext()->Unmap(m_2DTexture , 0);
    }

    m_TexInfo.m_Pitch = xRoundToByte( (int)m_TexInfo.m_TexWidth * m_TexInfo.m_nBytePerPixel , 4);
    m_TexInfo.m_SlicePitch =  m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight;
    if(mappedText.pData)
    {
	   m_TexInfo.m_Pitch = mappedText.RowPitch;
       m_TexInfo.m_SlicePitch = mappedText.DepthPitch;
    }

	
	if(m_bReadable  || m_TexInfo.m_Usage == D3D10_USAGE_DEFAULT )
	{
		m_texBuffer.create(m_TexInfo.m_Pitch , (int)m_TexInfo.m_TexHeight) ;
	}
	return bRet ;
}



bool xD11LockableTexture::_create3DTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData)
{

	D3D11_TEXTURE3D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)m_TexInfo.m_TexWidth;
	desc.Height           = (UINT)m_TexInfo.m_TexHeight;
	desc.MipLevels        = (UINT)m_TexInfo.m_MipmapLevel;
	desc.Depth            = (UINT)m_TexInfo.m_TexDepth;
	desc.Format           = m_TexInfo.m_ResFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
   
    desc.BindFlags        = xD11ConstLexer::BindFlag(initDesc.m_bindType)  ;//| D3D11_BIND_RENDER_TARGET; 
    desc.Usage            = xD11ConstLexer::Usage(initDesc.m_usage);
    desc.CPUAccessFlags   = xD11ConstLexer::AccessFlag(initDesc.m_access);
    m_TexInfo.m_Usage     = desc.Usage;

    D3D11_SUBRESOURCE_DATA* pDxInitData = NULL;
    if(pInitData )
    {
        pDxInitData = new D3D11_SUBRESOURCE_DATA [ nInitData ] ;
        for(int i = 0 ; i < nInitData ; i ++)
        {
            pDxInitData[i].pSysMem          = pInitData[i].m_pData;
            pDxInitData[i].SysMemPitch      = pInitData[i].m_Pitch;
            pDxInitData[i].SysMemSlicePitch = 0;
        }   
    }

	m_pD11RenderApi->d11Device()->CreateTexture3D( &desc, NULL, &m_3DTexture );
    if(pDxInitData) delete [] pDxInitData;

	if(m_3DTexture == NULL)
		return false;
	m_3DTexture->AddRef();
	bool bRet = _load(m_3DTexture);

	D3D11_MAPPED_SUBRESOURCE mappedText;
    mappedText.pData = NULL;
    if(m_TexInfo.m_Usage != D3D11_USAGE_DEFAULT)
    {
        m_pD11RenderApi->d11DeviceContext()->Map(m_3DTexture , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedText);
        m_pD11RenderApi->d11DeviceContext()->Unmap(m_3DTexture , 0);
    }

    m_TexInfo.m_Pitch = xRoundToByte(m_TexInfo.m_TexWidth * m_TexInfo.m_nBytePerPixel , 4);
    m_TexInfo.m_SlicePitch = m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight;
    if(mappedText.pData)
    {
        m_TexInfo.m_Pitch = mappedText.RowPitch;
        m_TexInfo.m_SlicePitch = mappedText.DepthPitch;
    }

	if(m_bReadable  || m_TexInfo.m_Usage == D3D10_USAGE_DEFAULT )
	{
		m_texBuffer.create(m_TexInfo.m_SlicePitch , (int)m_TexInfo.m_TexDepth ) ;
	}
	return bRet;
}



bool  xD11LockableTexture::update(void* data  , int dateLen , int rowPitch , int depthPicth , int mipmapLevel , int arraySlice)
{
	if(m_pTexture == NULL)
		return false;

    ID3D11Resource* pLockTexture = m_3DTexture != NULL ? (ID3D11Resource*)m_3DTexture : (ID3D11Resource*)m_2DTexture;
    return _update(pLockTexture , data , dateLen , rowPitch , depthPicth , mipmapLevel , arraySlice);
}

bool xD11LockableTexture::lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo  , int mipmapLevel , int arraySlice)
{
	if(m_pTexture == NULL)
		return false;
	UINT lockResource = D3D10CalcSubresource((UINT)mipmapLevel , (UINT)arraySlice, (UINT)m_TexInfo.m_MipmapLevel);
	D3D11_MAP  mapType = D3D11_MAP_WRITE_DISCARD;
	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();

    UINT  mapFlag = 0;
	switch(lockPolicy)
	{
	case eLock_WriteDiscard:      mapType = D3D11_MAP_WRITE_DISCARD; break;
	case eLock_Read:    		  mapType = D3D11_MAP_READ; break;
	case eLock_Write:    		  mapType = D3D11_MAP_WRITE; break;
	case eLock_ReadWrite:    	  mapType = D3D11_MAP_READ_WRITE; break;
	case eLock_WriteNoOverwrite:  mapType = D3D11_MAP_WRITE_NO_OVERWRITE; break;
	}
    lockInfo.m_lockResource = (long)lockResource;

    lockInfo.m_height      = xGetMipLevelSize(m_TexInfo.m_TexHeight   ,     mipmapLevel);
    lockInfo.m_width       = xGetMipLevelSize(m_TexInfo.m_TexWidth    ,     mipmapLevel);
    lockInfo.m_depth       = xGetMipLevelSize(m_TexInfo.m_TexDepth    ,     mipmapLevel);
    lockInfo.m_picth       = xGetMipLevelSize(m_TexInfo.m_Pitch       ,     mipmapLevel);
    lockInfo.m_slice_pitch = xGetMipLevelSize(m_TexInfo.m_SlicePitch  , 2 * mipmapLevel);

	//锁定实际的纹理
    MapTexture(pDeviceContext, lockResource, mapFlag, lockInfo);

    //==================================
    if(lockInfo.m_pixels == NULL && m_texBuffer.m_pixel == NULL)
        return false;

    //可读和非可读的。
    if(m_bReadable  || m_TexInfo.m_Usage == D3D10_USAGE_DEFAULT)
    {
        m_texBuffer.setDestBuffer(lockInfo.m_pixels);
        lockInfo.m_pixels      = m_texBuffer.m_pixel;
    }
	return true;
}

void  xD11LockableTexture::MapTexture( ID3D11DeviceContext* pDeviceContext, UINT lockResource, UINT mapFlag, xTextureLockArea &lockInfo ) 
{
    lockInfo.m_pixels      = NULL;
    if(mapFlag & D3D11_MAP_READ)
    {
        return ;
    }
    if(m_TexInfo.m_Usage == D3D11_USAGE_DEFAULT)
    {
        return ;
    }

    ID3D11Resource* pLockTexture = m_3DTexture != NULL ? (ID3D11Resource*)m_3DTexture : (ID3D11Resource*)m_2DTexture;

    D3D11_MAPPED_SUBRESOURCE mappedTex;
    mappedTex.pData = NULL;
    mappedTex.RowPitch = 0;
    mappedTex.DepthPitch = 0;
    pDeviceContext->Map(pLockTexture , lockResource , D3D11_MAP_WRITE_DISCARD , mapFlag , &mappedTex );
    lockInfo.m_picth       = (int)mappedTex.RowPitch ;
    lockInfo.m_pixels      = (char*)mappedTex.pData;
    lockInfo.m_slice_pitch = (int)mappedTex.DepthPitch;
}


bool xD11LockableTexture::unlock(xTextureLockArea& lockInfo)
{
	int lockResource = (int)lockInfo.m_lockResource;

	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
    ID3D11Resource* pLockTexture = m_3DTexture != NULL ? (ID3D11Resource*)m_3DTexture : (ID3D11Resource*)m_2DTexture;
    if(m_texBuffer.m_destBuffer)
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
    if(m_TexInfo.m_Usage == D3D11_USAGE_DEFAULT)
    {
        int iMip = lockResource%m_TexInfo.m_MipmapLevel;
        int iSlice = lockResource/m_TexInfo.m_MipmapLevel;
        return update(m_texBuffer.m_pixel ,m_texBuffer.m_dataLen , lockInfo.m_picth , lockInfo.m_slice_pitch , iMip , iSlice);
    }
    else
    {
        pDeviceContext->Unmap(pLockTexture , lockResource);
    }
	//D3DX10SaveTextureToFileW(m_pTexture,D3DX10_IMAGE_FILE_FORMAT::D3DX10_IFF_PNG , _XEVOL_ABSPATH_(L".\\Profile\\test.png") );
	return true;
}

IRenderTarget* xD11LockableTexture::toRenderTarget(size_t iSlice  , size_t iMipMapLevel )
{
    xD11DynamicRenderTarget* pRenderTarget = new xD11DynamicRenderTarget(m_pD11RenderApi , (int) iSlice , (int)iMipMapLevel);
    pRenderTarget->setTexture(this);                                                        
    pRenderTarget->create(m_pTexture,m_TexInfo  , m_SampleDesc , (int)iSlice , (int)iMipMapLevel);
    AddRef();                                                                               
    return pRenderTarget;                                                                   
}

END_NAMESPACE_XEVOL3D