#include "stdafx.h"
#include "xD3D9Texture2D.h"
#include "xD3D9ConstLexer.h"
#include "xD3D9RenderTarget.h"
#include "xDIRECT3D9API.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD3D9Texture2D  , xD3D9BaseTexture);


xD3D9Texture2D::xD3D9Texture2D(xD3D9RenderApi* pAPI , bool bReadable)
:xD3D9BaseTexture(pAPI)
{
	m_bReadable = bReadable;
	m_pTexture          = NULL;
	m_p2DTexture = NULL;
	memset(&m_TexInfo, 0 ,sizeof(m_TexInfo));
    m_pLockedSurface = NULL;
}

xD3D9Texture2D::~xD3D9Texture2D()
{
	unload();
}

void* xD3D9Texture2D::handle()
{
	return m_p2DTexture;
}

bool xD3D9Texture2D::isLoaded()
{
	return m_p2DTexture != NULL ;
}

bool xD3D9Texture2D::unload()
{
	m_texBuffer.unload();
	XSAFE_RELEASE(m_p2DTexture);
	XSAFE_RELEASE(m_pTexture);
	return !isLoaded();
}

void  xD3D9Texture2D::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    if( !bNewDevice && m_TexInfo.m_D3DPool != D3DPOOL_DEFAULT)
        return ;

    if(_Action == ID3D9DeviceLostListener::eReset_Begin)
    {
        XSAFE_RELEASE(m_pTexture);
        XSAFE_RELEASE(m_p2DTexture);
    }

    if(_Action == ID3D9DeviceLostListener::eReset_Finish)
    {
        //新创建的纹理是不会有内容的
        create(m_InitDesc , NULL , 0);
    }
}


bool xD3D9Texture2D::load(const wchar_t* fileName , unsigned long  arg)
{
    FILE* fp = _wfopen(fileName , L"rb");
    if(fp == NULL)
        return false;

    fseek(fp , 0 , SEEK_END);
    int len = ftell(fp);
    fseek(fp , 0 , SEEK_SET);
    unsigned char* fBuf = new unsigned char[len];
    fread(fBuf , 1 , len , fp);
    fclose(fp);
    return load(fileName , fBuf , len , arg);
}

bool xD3D9Texture2D::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();
	return false;
}

bool  xD3D9Texture2D::create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData) 
{ 
    unload();
    m_InitDesc = initDesc;
    xD3D9GIFormatInfo* pFmtInfo = xD3D9ConstLexer::singleton()->GetPixelFormat(initDesc.m_TextureDesc.m_fmt);
    m_TexInfo.m_ArraySize = abs(initDesc.m_TextureDesc.m_nArraySize);
    m_TexInfo.m_ResFmt        = pFmtInfo->m_dxfmt;
    m_TexInfo.m_MemSize   = 0;
    m_TexInfo.m_MipmapLevel = initDesc.m_nMipmap;
    m_TexInfo.m_TexDepth    = 1;
    m_TexInfo.m_TexHeight   = initDesc.m_TextureDesc.m_height;
    m_TexInfo.m_TexWidth    = initDesc.m_TextureDesc.m_width;
    m_TexInfo.m_Type        = initDesc.m_TextureDesc.m_depth == 1 ? RESOURCE_TEXTURE2D : RESOURCE_TEXTURE3D;
    m_TexInfo.m_xfmt        = initDesc.m_TextureDesc.m_fmt;
    m_TexInfo.m_nBytePerPixel = pFmtInfo->m_compont * pFmtInfo->m_bytePerComponent;
    if(initDesc.m_TextureDesc.m_depth != 1)
        return false;

    return _create2DTexture(initDesc , pInitData , nInitData);
}


bool  xD3D9Texture2D::_create2DTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData)
{
	
    m_p2DTexture = NULL;
    DWORD   usage = xD3D9ConstLexer::singleton()->GetD3DUsage(initDesc.m_access , initDesc.m_bindType , m_InitDesc.m_usage );
    D3DPOOL pool  = xD3D9ConstLexer::singleton()->GetD3DPool( initDesc.m_usage ); 
    if( (usage & D3DUSAGE_RENDERTARGET) == 0)
        RESET_D3DPOOL(pool);

    if(FAILED(m_pD3D9RenderApi->getDevice()->CreateTexture((UINT)m_TexInfo.m_TexWidth, (UINT)m_TexInfo.m_TexHeight, m_TexInfo.m_MipmapLevel , usage , m_TexInfo.m_ResFmt , pool , &m_p2DTexture,0)) )
        return false;
    m_pTexture = m_p2DTexture;


	bool bRet =  _load(m_pTexture);
    m_TexInfo.m_D3DPool = pool;



    m_TexInfo.m_Pitch = xRoundToByte(m_TexInfo.m_TexWidth * m_TexInfo.m_nBytePerPixel , 4);
    m_TexInfo.m_SlicePitch = (int)(m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight);

    //只有不是POOL_DEFAULT才是可以锁定的
    if(m_TexInfo.m_D3DPool != D3DPOOL_DEFAULT)
    {
        IDirect3DSurface9* pSurf = NULL;
        m_p2DTexture->GetSurfaceLevel(0 , &pSurf);
        D3DLOCKED_RECT LockedRect;
        pSurf->LockRect(&LockedRect,NULL,0);
        pSurf->UnlockRect();
        pSurf->Release();
        if(LockedRect.pBits)
        {
            m_TexInfo.m_Pitch = LockedRect.Pitch;
            m_TexInfo.m_SlicePitch = (int)(m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight);
        }
    }


    
    if(m_bReadable || pool == D3DPOOL_DEFAULT )
	{
		m_texBuffer.create(m_TexInfo.m_Pitch , (int)m_TexInfo.m_TexHeight) ;
	}

    if(pInitData == NULL )
        return bRet;

    xTextureInitData* _initData = pInitData;
    for(int iLevel = 0 ; iLevel < (int)m_TexInfo.m_MipmapLevel ; iLevel ++)
    {
        update( _initData->m_pData , _initData->m_DataLen , _initData->m_Pitch , _initData->m_DepthPitch , iLevel , 0  );
        _initData ++ ;
    }

	return bRet ;
}


bool  xD3D9Texture2D::update(void* data  , int dateLen , int rowPitch , int depthPicth , int mipmapLevel , int arraySlice)
{
	if(m_pTexture == NULL)
		return false;
    IDirect3DDevice9*  pDevice = m_pD3D9RenderApi->getDevice();
    IDirect3DSurface9* pDstSurf = NULL;
    m_p2DTexture->GetSurfaceLevel(mipmapLevel , &pDstSurf);
    if(rowPitch == 0) rowPitch = m_TexInfo.m_Pitch;
    uploadSurfaceData(pDstSurf , (const char*)data , rowPitch , false );
	return true;
}


bool xD3D9Texture2D::lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo  , int mipmapLevel , int arraySlice)
{
	if(m_pTexture == NULL)
		return false;

    UINT  mapFlag = 0;
	switch(lockPolicy)
	{
	case eLock_WriteDiscard:      mapFlag = D3DLOCK_DISCARD; break;
	case eLock_Read:    		  mapFlag = D3DLOCK_READONLY; break;
	case eLock_Write:    		  mapFlag = D3DLOCK_NOOVERWRITE; break;
	case eLock_ReadWrite:    	  mapFlag = 0; break;
	case eLock_WriteNoOverwrite:  mapFlag = D3DLOCK_NOOVERWRITE; break;
	}
    mapFlag = 0;
    D3DSURFACE_DESC SurfDesc;
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    m_p2DTexture->GetSurfaceLevel(mipmapLevel , &m_pLockedSurface);    
    m_p2DTexture->GetLevelDesc(mipmapLevel , &SurfDesc);
    
    lockInfo.m_width        = (int)SurfDesc.Width;
    lockInfo.m_height       = (int)SurfDesc.Height;
    lockInfo.m_depth        = (int)1;
    lockInfo.m_lockResource = mipmapLevel;
    lockInfo.m_pixels       = NULL;
    lockInfo.m_picth        = xGetMipLevelSize(m_TexInfo.m_Pitch       , 2 * mipmapLevel);


    if(m_TexInfo.m_D3DPool != D3DPOOL_DEFAULT)
    {
        D3DLOCKED_RECT LockRect;
        m_pLockedSurface->LockRect(&LockRect , NULL , mapFlag);
        lockInfo.m_pixels    = (char*)LockRect.pBits;
        if(lockInfo.m_pixels == NULL && m_texBuffer.m_pixel == NULL)
            return false;
        if(LockRect.pBits)
        {
            lockInfo.m_picth        = LockRect.Pitch;
            lockInfo.m_slice_pitch  = LockRect.Pitch * lockInfo.m_height;
        }
    }

    //可读和非可读的。
    if(m_bReadable  || m_TexInfo.m_D3DPool == D3DPOOL_DEFAULT)
    {
        m_texBuffer.setDestBuffer(lockInfo.m_pixels);
        lockInfo.m_pixels      = m_texBuffer.m_pixel;        
    }
	return true;
}


bool xD3D9Texture2D::unlock(xTextureLockArea& lockInfo)
{
	IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
	if(m_texBuffer.m_destBuffer)
	{
		m_texBuffer.flush(lockInfo.m_height , lockInfo.m_picth       , lockInfo.m_picth);
	}

    if(m_TexInfo.m_D3DPool == D3DPOOL_DEFAULT)
    {
        m_pLockedSurface->AddRef();
        uploadSurfaceData(m_pLockedSurface , m_texBuffer.m_pixel , lockInfo.m_picth , false);
    }
    else
    {
        m_pLockedSurface->UnlockRect();
    }
    m_pLockedSurface->Release();
    m_pLockedSurface = NULL;
	//D3DX10SaveTextureToFileW(m_pTexture,D3DX10_IMAGE_FILE_FORMAT::D3DX10_IFF_PNG , _XEVOL_ABSPATH_(L".\\Profile\\test.png") );
	return true;
}

IRenderTarget* xD3D9Texture2D::toRenderTarget(size_t iSlice  , size_t iMipMapLevel )
{

    xD3D9DynamicRenderTarget* pRenderTarget = new xD3D9DynamicRenderTarget(m_pD3D9RenderApi , (int)iSlice , (int)iMipMapLevel);
    pRenderTarget->setTexture(this);
    pRenderTarget->create(this,(int)iSlice , (int)iMipMapLevel);
    AddRef();
    return pRenderTarget;
}

bool xD3D9Texture2D::grabRenderTagetData(void* pData , int x , int y , int w , int h  , int arraySlice , int mipmapLevel )
{
    IDirect3DDevice9* pDevice  = m_pD3D9RenderApi->getDevice();
    IDirect3DSurface9* pRTSurface = NULL;  
    m_p2DTexture->GetSurfaceLevel(mipmapLevel , &pRTSurface);
    bool bRet = _grabRenderTagetData(pRTSurface , pData , x , y , w , h);
    XSAFE_RELEASE(pRTSurface);
    return bRet;
}
/*


bool xD3D9Texture2D::_create3DTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData)
{

D3D11_TEXTURE3D_DESC desc;
ZeroMemory( &desc, sizeof(desc) );
desc.Width            = (UINT)m_TexInfo.m_TexWidth;
desc.Height           = (UINT)m_TexInfo.m_TexHeight;
desc.MipLevels        = (UINT)m_TexInfo.m_MipmapLevel;
desc.Depth            = (UINT)m_TexInfo.m_TexDepth;
desc.Format           = m_TexInfo.m_ResFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;

desc.BindFlags        = xD3D9ConstLexer::BindFlag(initDesc.m_bindType)  ;//| D3D11_BIND_RENDER_TARGET; 
desc.Usage            = xD3D9ConstLexer::Usage(initDesc.m_usage);
desc.CPUAccessFlags   = xD3D9ConstLexer::AccessFlag(initDesc.m_access);
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

m_pD3D9RenderApi->d11Device()->CreateTexture3D( &desc, NULL, &m_3DTexture );
if(pDxInitData) delete [] pDxInitData;

if(m_3DTexture == NULL)
return false;
m_3DTexture->AddRef();
bool bRet = _load(m_3DTexture);

D3D11_MAPPED_SUBRESOURCE mappedText;
m_pD3D9RenderApi->d11DeviceContext()->Map(m_3DTexture , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedText);
m_pD3D9RenderApi->d11DeviceContext()->Unmap(m_3DTexture , 0);
m_TexInfo.m_Pitch = mappedText.RowPitch;
m_TexInfo.m_SlicePitch = mappedText.DepthPitch;

if(m_bReadable && bRet)
{
m_texBuffer.create(m_TexInfo.m_SlicePitch , (int)m_TexInfo.m_TexDepth ) ;
}
return bRet;
}




*/
END_NAMESPACE_XEVOL3D