#include "stdafx.h"
#include "xD10LockableTexture.h"
#include "xD10ConstLexer.h"
#include "xDirect3D10API.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD10LockableTexture  , xD10BaseTexture);


xD10LockableTexture::xD10LockableTexture(xD3D10RenderApi* pAPI , bool bReadable)
:xD10UnkwonTexture(pAPI)
{
    m_bReadable = bReadable;
    m_pTextureView      = NULL;
    m_pTexture          = NULL;
    m_2DTexture = NULL;
    m_3DTexture = NULL;
    memset(&m_TexInfo, 0 ,sizeof(m_TexInfo));
}

xD10LockableTexture::~xD10LockableTexture()
{
    unload();
}

void* xD10LockableTexture::handle()
{
    return m_pTextureView;
}

bool xD10LockableTexture::isLoaded()
{
    return m_pTexture != NULL && m_pTextureView != NULL;
}

bool xD10LockableTexture::unload()
{
    m_texBuffer.unload();
    XSAFE_RELEASE(m_pTextureView);
    XSAFE_RELEASE(m_pTexture);
    XSAFE_RELEASE(m_2DTexture);
    XSAFE_RELEASE(m_3DTexture);
    return !isLoaded();
}


bool xD10LockableTexture::load(const wchar_t* fileName , unsigned long  arg)
{
    unload();
    D3DX10_IMAGE_LOAD_INFO loadInfo;
    fillLoadInfo(loadInfo, true ,true);
    ID3D10Resource* pTexture = NULL;
    D3DX10CreateTextureFromFileW( m_pD10Api->d10Device() , fileName ,  &loadInfo , NULL, &pTexture, NULL );
    if(pTexture)
    {
        m_TexInfo.m_ShaderViewFmt    = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_ResFmt        = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_RTViewFmt     = DXGI_FORMAT_UNKNOWN;
        return _load(pTexture);
    }
    return false;
}

bool xD10LockableTexture::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
    unload();

    if(bufLen == 0 || buf == NULL)
        return load(fileName , arg);

    D3DX10_IMAGE_LOAD_INFO loadInfo;
    fillLoadInfo( loadInfo, true ,true);
    ID3D10Resource* pTexture = NULL;
    D3DX10CreateTextureFromMemory(m_pD10Api->d10Device() , buf , bufLen , &loadInfo, NULL, &pTexture, NULL );
    if(pTexture)
    {
        m_TexInfo.m_ShaderViewFmt    = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_ResFmt           = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_RTViewFmt        = DXGI_FORMAT_UNKNOWN;
        return _load(pTexture);
    }

    return false;
}

bool  xD10LockableTexture::create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData) 
{ 
    unload();

    xD10GIFormatInfo* pFmtInfo = xD10ConstLexer::singleton()->GetPixelFormat(initDesc.m_TextureDesc.m_fmt);
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


bool  xD10LockableTexture::_create2DTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData)
{
    D3D10_TEXTURE2D_DESC desc;
    ZeroMemory( &desc, sizeof(desc) );
    desc.Width            = (UINT)m_TexInfo.m_TexWidth;
    desc.Height           = (UINT)m_TexInfo.m_TexHeight;
    desc.MipLevels        = (UINT)m_TexInfo.m_MipmapLevel;
    desc.ArraySize        = (UINT)m_TexInfo.m_ArraySize;
    desc.Format           = m_TexInfo.m_ShaderViewFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc       = m_SampleDesc;
    desc.BindFlags        = xD10ConstLexer::BindFlag(initDesc.m_bindType)  ;//| D3D10_BIND_RENDER_TARGET; 
    desc.Usage            = xD10ConstLexer::Usage(initDesc.m_usage);
    desc.CPUAccessFlags   = xD10ConstLexer::AccessFlag(initDesc.m_access);

    if(initDesc.m_TextureDesc.m_nArraySize == eTextureArray_CubeMap)
    {
        desc.MiscFlags |= D3D10_RESOURCE_MISC_TEXTURECUBE;
    }

    m_TexInfo.m_Usage     = desc.Usage;

    D3D10_SUBRESOURCE_DATA* pDxInitData = NULL;
    if(pInitData )
    {
        pDxInitData = new D3D10_SUBRESOURCE_DATA [ nInitData ] ;
        for(int i = 0 ; i < nInitData ; i ++)
        {
            pDxInitData[i].pSysMem          = pInitData[i].m_pData;
            pDxInitData[i].SysMemPitch      = pInitData[i].m_Pitch;
            pDxInitData[i].SysMemSlicePitch = 0;
        }   
    }

    m_pD10Api->d10Device()->CreateTexture2D( &desc , pDxInitData, &m_2DTexture );

    if(pDxInitData ) delete  [] pDxInitData;

    if(m_2DTexture == NULL)
        return false;
    m_2DTexture->AddRef();

    bool bRet =  _load(m_2DTexture);

    D3D10_MAPPED_TEXTURE2D mappedText;
    mappedText.pData = NULL;
    if(m_TexInfo.m_Usage != D3D10_USAGE_DEFAULT)
    {
        m_2DTexture->Map(0,D3D10_MAP_WRITE_DISCARD ,0, &mappedText);
        m_2DTexture->Unmap(0);
    }
    m_TexInfo.m_Pitch = xRoundToByte(m_TexInfo.m_TexWidth * m_TexInfo.m_nBytePerPixel , 4);
    
    if(mappedText.pData)
        m_TexInfo.m_Pitch = mappedText.RowPitch;
 
    m_TexInfo.m_SlicePitch = m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight;

    if(m_bReadable || desc.Usage == D3D10_USAGE_DEFAULT)
    {
        m_texBuffer.create(m_TexInfo.m_Pitch , (int)m_TexInfo.m_TexHeight) ;
    }
    return bRet ;
}



bool xD10LockableTexture::_create3DTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData)
{
    D3D10_TEXTURE3D_DESC desc;
    ZeroMemory( &desc, sizeof(desc) );
    desc.Width            = (UINT)m_TexInfo.m_TexWidth;
    desc.Height           = (UINT)m_TexInfo.m_TexHeight;
    desc.MipLevels        = (UINT)m_TexInfo.m_MipmapLevel;
    desc.Depth            = (UINT)m_TexInfo.m_TexDepth;
    desc.Format           = m_TexInfo.m_ResFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.BindFlags        = xD10ConstLexer::BindFlag(initDesc.m_bindType)  ;//| D3D10_BIND_RENDER_TARGET; 
    desc.Usage            = xD10ConstLexer::Usage(initDesc.m_usage);
    desc.CPUAccessFlags   = xD10ConstLexer::AccessFlag(initDesc.m_access);
    m_TexInfo.m_Usage     = desc.Usage;

    D3D10_SUBRESOURCE_DATA* pDxInitData = NULL;
    if(pInitData )
    {
        pDxInitData = new D3D10_SUBRESOURCE_DATA [ nInitData ] ;
        for(int i = 0 ; i < nInitData ; i ++)
        {
            pDxInitData[i].pSysMem          = pInitData[i].m_pData;
            pDxInitData[i].SysMemPitch      = pInitData[i].m_Pitch;
            pDxInitData[i].SysMemSlicePitch = 0;
        }   
    }

    m_pD10Api->d10Device()->CreateTexture3D( &desc , pDxInitData, &m_3DTexture );

    if(pDxInitData ) delete  [] pDxInitData;


    if(m_3DTexture == NULL)
        return false;
    m_3DTexture->AddRef();
    bool bRet = _load(m_3DTexture);

    D3D10_MAPPED_TEXTURE3D mappedText;
    mappedText.pData = NULL;
    if(m_TexInfo.m_Usage != D3D10_USAGE_DEFAULT)
    {
        m_3DTexture->Map(0 , D3D10_MAP_WRITE_DISCARD ,0, &mappedText);
        m_3DTexture->Unmap(0);
    }

    m_TexInfo.m_Pitch      = xRoundToByte(m_TexInfo.m_TexWidth * m_TexInfo.m_nBytePerPixel , 4);
    m_TexInfo.m_SlicePitch = m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight;
    if(mappedText.pData)
    {
        m_TexInfo.m_Pitch = mappedText.RowPitch;
        m_TexInfo.m_SlicePitch = mappedText.DepthPitch;
    }

    if(m_bReadable  || desc.Usage == D3D10_USAGE_DEFAULT )
    {
        m_texBuffer.create(m_TexInfo.m_SlicePitch , (int)m_TexInfo.m_TexDepth ) ;
    }
    return bRet;
}


bool  xD10LockableTexture::update(void* data  , int dateLen , int rowPitch , int depthPicth , int mipmapLevel , int arraySlice)
{
    if(m_pTexture == NULL)
        return false;

    UINT lockResource = D3D10CalcSubresource((UINT)mipmapLevel , (UINT)arraySlice, (UINT)m_TexInfo.m_MipmapLevel);
    char* pMapedData = NULL;
    int   Pitch = 0;
    int   SlicePith = 0;



    if(m_TexInfo.m_Usage == D3D10_USAGE_DEFAULT)
    {
        ID3D10Resource* pLockTexture = m_3DTexture != NULL ? (ID3D10Resource*)m_3DTexture : (ID3D10Resource*)m_2DTexture;
        if(rowPitch == 0 ) rowPitch = dateLen;
        if(depthPicth == 0) depthPicth = dateLen;
        m_pD10Api->d10Device()->UpdateSubresource(pLockTexture ,lockResource , NULL , data ,rowPitch , depthPicth);         
    }
    else
    {
        if(m_3DTexture) 
        {
            D3D10_MAPPED_TEXTURE3D mappedTex;
            mappedTex.pData = NULL;
            mappedTex.RowPitch = 0;
            mappedTex.DepthPitch = 0;
            m_3DTexture->Map( lockResource  , D3D10_MAP_WRITE_DISCARD, 0, &mappedTex );
            pMapedData = (char*)mappedTex.pData;
            Pitch     = mappedTex.RowPitch;
            SlicePith = mappedTex.DepthPitch;
        }
        if(m_2DTexture) 
        {
            D3D10_MAPPED_TEXTURE2D mappedTex;
            mappedTex.pData = NULL;
            mappedTex.RowPitch = 0;
            m_2DTexture->Map( lockResource  , D3D10_MAP_WRITE_DISCARD, 0, &mappedTex );
            pMapedData = (char*)mappedTex.pData;
            Pitch = mappedTex.RowPitch;
            SlicePith = 0;
        }
        if(pMapedData == NULL)
            return false;
        memcpy(pMapedData , data , dateLen);
        if(m_3DTexture) m_3DTexture->Unmap(lockResource);
        if(m_2DTexture) m_2DTexture->Unmap(lockResource);
    }
    return true;
}


bool xD10LockableTexture::lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo  , int mipmapLevel , int arraySlice)
{
    if(m_pTexture == NULL)
        return false;
    UINT lockResource = D3D10CalcSubresource((UINT)mipmapLevel , (UINT)arraySlice, (UINT)m_TexInfo.m_MipmapLevel);
    D3D10_MAP  mapType = D3D10_MAP_WRITE_DISCARD;
    switch(lockPolicy)
    {
    case eLock_WriteDiscard:      mapType = D3D10_MAP_WRITE_DISCARD; break;
    case eLock_Read:    		  mapType = D3D10_MAP_READ; break;
    case eLock_Write:    		  mapType = D3D10_MAP_WRITE; break;
    case eLock_ReadWrite:    	  mapType = D3D10_MAP_READ_WRITE; break;
    case eLock_WriteNoOverwrite:  mapType = D3D10_MAP_WRITE_NO_OVERWRITE; break;
    }
    lockInfo.m_lockResource = (long)lockResource;
    lockInfo.m_height      = xGetMipLevelSize(m_TexInfo.m_TexHeight   ,     mipmapLevel);
    lockInfo.m_width       = xGetMipLevelSize(m_TexInfo.m_TexWidth    ,     mipmapLevel);
    lockInfo.m_depth       = xGetMipLevelSize(m_TexInfo.m_TexDepth    ,     mipmapLevel);
    lockInfo.m_picth       = xGetMipLevelSize(m_TexInfo.m_Pitch       ,     mipmapLevel);
    lockInfo.m_slice_pitch = xGetMipLevelSize(m_TexInfo.m_SlicePitch  , 2 * mipmapLevel);
    MapTexture(lockResource, mapType, lockInfo);

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

void xD10LockableTexture::MapTexture( UINT lockResource, D3D10_MAP mapType, xTextureLockArea &lockInfo ) 
{
    lockInfo.m_pixels = NULL;
    if(mapType & D3D10_MAP_READ)
    {
        return ;
    }
    if(m_TexInfo.m_Usage == D3D10_USAGE_DEFAULT)
    {
        return ;
    }
    if(m_2DTexture)
    {
        D3D10_MAPPED_TEXTURE2D mappedTex;
        mappedTex.pData = NULL;
        mappedTex.RowPitch = 0;
        m_2DTexture->Map( lockResource  , mapType, 0, &mappedTex );
        lockInfo.m_picth       = (int)mappedTex.RowPitch ;
        lockInfo.m_pixels      = (char*)mappedTex.pData;
        lockInfo.m_slice_pitch = (int)(lockInfo.m_picth * m_TexInfo.m_TexHeight);
    }
    else
    {
        D3D10_MAPPED_TEXTURE3D mappedTex;
        mappedTex.pData = NULL;
        mappedTex.RowPitch = 0;
        mappedTex.DepthPitch = 0;
        m_3DTexture->Map( lockResource  , mapType, 0, &mappedTex );
        lockInfo.m_picth       = (int)mappedTex.RowPitch ;
        lockInfo.m_pixels      = (char*)mappedTex.pData;
        lockInfo.m_slice_pitch = (int)mappedTex.DepthPitch;
    }

}

bool xD10LockableTexture::unlock(xTextureLockArea& lockInfo)
{
    int lockResource = (int)lockInfo.m_lockResource;
    if(m_pTexture == NULL)
        return false;
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

    if(m_TexInfo.m_Usage == D3D10_USAGE_DEFAULT)
    {
        int iMip = lockResource%m_TexInfo.m_MipmapLevel;
        int iSlice = lockResource/m_TexInfo.m_MipmapLevel;
        return update(m_texBuffer.m_pixel ,m_texBuffer.m_dataLen , lockInfo.m_picth , lockInfo.m_slice_pitch , iMip , iSlice);
    }
    else
    {
        if(m_2DTexture) m_2DTexture->Unmap( lockResource );
        if(m_3DTexture) m_3DTexture->Unmap( lockResource );
    }
    //D3DX10SaveTextureToFileW(m_pTexture,D3DX10_IMAGE_FILE_FORMAT::D3DX10_IFF_PNG , _XEVOL_ABSPATH_(L".\\Profile\\test.png") );
    return true;
}

IRenderTarget* xD10LockableTexture::toRenderTarget(size_t iSlice  , size_t iMipMapLevel )
{
    xD10DynamicRenderTarget* pRenderTarget = new xD10DynamicRenderTarget(m_pD10Api , (int)iSlice , (int)iMipMapLevel);
    pRenderTarget->setTexture(this);
    pRenderTarget->create(m_pTexture,m_TexInfo  , m_SampleDesc , (int)iSlice , (int)iMipMapLevel);
    AddRef();
    return pRenderTarget;
}

END_NAMESPACE_XEVOL3D