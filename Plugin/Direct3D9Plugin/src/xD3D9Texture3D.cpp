#include "stdafx.h"
#include "xD3D9Texture3D.h"
#include "xD3D9ConstLexer.h"
#include "xDIRECT3D9API.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD3D9Texture3D  , xD3D9BaseTexture);


xD3D9Texture3D::xD3D9Texture3D(xD3D9RenderApi* pAPI , bool bReadable)
:xD3D9BaseTexture(pAPI)
{
	m_bReadable = bReadable;
	m_pTexture          = NULL;
	m_p3DTexture = NULL;
	memset(&m_TexInfo, 0 ,sizeof(m_TexInfo));
    m_pLockedVolume = NULL;
}

xD3D9Texture3D::~xD3D9Texture3D()
{
	unload();
}

void* xD3D9Texture3D::handle()
{
	return m_p3DTexture;
}

bool xD3D9Texture3D::isLoaded()
{
	return m_p3DTexture != NULL ;
}

bool xD3D9Texture3D::unload()
{
	m_texBuffer.unload();
    XSAFE_RELEASE(m_pSys3DTexture);
	XSAFE_RELEASE(m_p3DTexture);
	XSAFE_RELEASE(m_pTexture);
	return !isLoaded();
}


bool xD3D9Texture3D::load(const wchar_t* fileName , unsigned long  arg)
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

bool xD3D9Texture3D::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();
	return false;
}
void  xD3D9Texture3D::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    if( !bNewDevice && m_TexInfo.m_D3DPool != D3DPOOL_DEFAULT)
        return ;

    if(_Action == ID3D9DeviceLostListener::eReset_Begin)
    {
        XSAFE_RELEASE(m_pTexture);
        XSAFE_RELEASE(m_p3DTexture);
    }

    if(_Action == ID3D9DeviceLostListener::eReset_Finish)
    {
        //新创建的纹理是不会有内容的
        create(m_InitDesc , NULL , 0);
    }
}
bool  xD3D9Texture3D::create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData) 
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

    return _create3DTexture(initDesc , pInitData , nInitData);
}


bool  xD3D9Texture3D::_create3DTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData)
{
	
    m_p3DTexture = NULL;
    DWORD   usage = xD3D9ConstLexer::singleton()->GetD3DUsage(initDesc.m_access , initDesc.m_bindType , m_InitDesc.m_usage );
    D3DPOOL pool  = xD3D9ConstLexer::singleton()->GetD3DPool( initDesc.m_usage ); 
    if( (usage & D3DUSAGE_RENDERTARGET) == 0)
        RESET_D3DPOOL(pool);

    m_TexInfo.m_D3DPool = pool;

    if(FAILED(m_pD3D9RenderApi->getDevice()->CreateVolumeTexture((UINT)m_TexInfo.m_TexWidth, (UINT)m_TexInfo.m_TexHeight, (UINT)m_TexInfo.m_TexDepth , (UINT)m_TexInfo.m_MipmapLevel , usage , m_TexInfo.m_ResFmt , pool , &m_p3DTexture,0)) )
        return false;
    m_pTexture = m_p3DTexture;

    if(pool == D3DPOOL_DEFAULT)
    {
        if(FAILED(m_pD3D9RenderApi->getDevice()->CreateVolumeTexture((UINT)m_TexInfo.m_TexWidth, (UINT)m_TexInfo.m_TexHeight, (UINT)m_TexInfo.m_TexDepth , (UINT)m_TexInfo.m_MipmapLevel , D3DPOOL_SYSTEMMEM , m_TexInfo.m_ResFmt , pool , &m_pSys3DTexture,0)) )
            return false;
    }

	bool bRet =  _load(m_pTexture);

    m_TexInfo.m_Pitch = xRoundToByte(m_TexInfo.m_TexWidth * m_TexInfo.m_nBytePerPixel , 4);
    m_TexInfo.m_SlicePitch = (int)(m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight);

    //只有非D3DPOOL_DEDAULT才可以锁定
    if(m_TexInfo.m_D3DPool != D3DPOOL_DEFAULT)
    {
        IDirect3DVolume9* pVolume = NULL;
        m_p3DTexture->GetVolumeLevel(0 , &pVolume);
        D3DLOCKED_BOX LockedBox;
        pVolume->LockBox(&LockedBox,NULL,0);
        pVolume->UnlockBox();
        pVolume->Release();

        if(LockedBox.pBits)
        {
            m_TexInfo.m_Pitch = LockedBox.RowPitch;
            m_TexInfo.m_SlicePitch = LockedBox.SlicePitch;
        }
    }

    
    if(m_bReadable || m_TexInfo.m_D3DPool == D3DPOOL_DEFAULT )
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


bool  xD3D9Texture3D::update(void* data  , int dateLen , int rowPitch , int depthPicth , int mipmapLevel , int arraySlice)
{
	if(m_pTexture == NULL)
		return false;
    IDirect3DDevice9*  pDevice = m_pD3D9RenderApi->getDevice();
    IDirect3DVolumeTexture9* pVolTexture = m_p3DTexture;
    if(m_pSys3DTexture) pVolTexture = m_pSys3DTexture;

    IDirect3DVolume9* pDstVolume = NULL;
    pVolTexture->GetVolumeLevel(mipmapLevel , &pDstVolume);
    if(rowPitch == 0) rowPitch = m_TexInfo.m_Pitch;
    uploadVolumeData(pDstVolume , (const char*)data , rowPitch , depthPicth , false );

    if(m_pSys3DTexture)
    {
        pDevice->UpdateTexture(pVolTexture , m_p3DTexture);
    }
	return true;
}


bool xD3D9Texture3D::lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo  , int mipmapLevel , int arraySlice)
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

    D3DVOLUME_DESC VolumeDesc;
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    IDirect3DVolumeTexture9* pVolTexture = m_p3DTexture;
    if(m_pSys3DTexture) pVolTexture = m_pSys3DTexture;

    pVolTexture->GetVolumeLevel(mipmapLevel , &m_pLockedVolume);    
    pVolTexture->GetLevelDesc(mipmapLevel , &VolumeDesc);

    lockInfo.m_width        = (int)VolumeDesc.Width;
    lockInfo.m_height       = (int)VolumeDesc.Height;
    lockInfo.m_depth        = (int)VolumeDesc.Depth;
    lockInfo.m_lockResource = mipmapLevel;
    lockInfo.m_slice_pitch  = xGetMipLevelSize(m_TexInfo.m_SlicePitch , 2 * mipmapLevel);
    lockInfo.m_picth        = xGetMipLevelSize(m_TexInfo.m_Pitch      ,     mipmapLevel);

    if(m_TexInfo.m_D3DPool != D3DPOOL_DEFAULT)
    {
        D3DLOCKED_BOX LockRect;
        m_pLockedVolume->LockBox(&LockRect , NULL , mapFlag);

        if(lockInfo.m_pixels == NULL && m_texBuffer.m_pixel == NULL)
            return false;
        if(LockRect.pBits)
        {
            lockInfo.m_slice_pitch  = LockRect.SlicePitch;
            lockInfo.m_picth        = LockRect.RowPitch;
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


bool xD3D9Texture3D::unlock(xTextureLockArea& lockInfo)
{
	IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
	if(m_texBuffer.m_destBuffer)
	{
		m_texBuffer.flush(lockInfo.m_height , lockInfo.m_picth       , lockInfo.m_picth);
	}

    if(m_TexInfo.m_D3DPool == D3DPOOL_DEFAULT)
    {
        m_pLockedVolume->AddRef();
        uploadVolumeData(m_pLockedVolume , m_texBuffer.m_pixel , lockInfo.m_picth ,lockInfo.m_slice_pitch, false);
    }
    else
    {
        m_pLockedVolume->UnlockBox();
    }
    m_pLockedVolume->Release();

    if(m_pSys3DTexture)
    {
        pDevice->UpdateTexture(m_pSys3DTexture , m_p3DTexture);
    }

	//D3DX10SaveTextureToFileW(m_pTexture,D3DX10_IMAGE_FILE_FORMAT::D3DX10_IFF_PNG , _XEVOL_ABSPATH_(L".\\Profile\\test.png") );
	return true;
}

IRenderTarget* xD3D9Texture3D::toRenderTarget(size_t iSlice  , size_t iMipMapLevel )
{
	if(iSlice != 0 || iMipMapLevel != 0)
		return NULL;
	return NULL;
}


/*


bool xD3D9Texture3D::_create3DTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData, int nInitData)
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