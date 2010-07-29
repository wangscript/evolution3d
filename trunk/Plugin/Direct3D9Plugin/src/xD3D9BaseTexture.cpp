#include "stdafx.h"
#include "xDirect3D9API.h"
#include "xD3D9BaseTexture.h"
#include "BaseLib/xBaseMath.h"
BEGIN_NAMESPACE_XEVOL3D


xD3D9TextureBuffer::xD3D9TextureBuffer()
{
	m_pitch = 0;
	m_pixel  = NULL;
	m_dataLen = 0;
	m_destBuffer = 0;
}
xD3D9TextureBuffer::~xD3D9TextureBuffer()
{
	XSAFE_DELETE_ARRAY(m_pixel);
}

void  xD3D9TextureBuffer::setDestBuffer(void* destBuffer)
{
	m_destBuffer = destBuffer;
}

bool  xD3D9TextureBuffer::create(int h , int pitch)
{
	m_dataLen = h * pitch;
	m_pitch = pitch;
	m_pixel = new char[m_dataLen];
    memset(m_pixel , 0 , m_dataLen);
	return true;
}

bool  xD3D9TextureBuffer::unload()
{
	XSAFE_DELETE_ARRAY(m_pixel);
	m_pitch = 0;
	m_dataLen = 0;
	m_destBuffer = 0;
	return true;
}

bool  xD3D9TextureBuffer::flush(int h , int dest_pitch , int src_pitch )
{
	if(src_pitch == 0) src_pitch = m_pitch;
	memcpy(m_destBuffer , m_pixel , dest_pitch * h);
	m_destBuffer = NULL;
	return true;
}


xD3D9BaseTexture::xD3D9BaseTexture(xD3D9RenderApi* pD11Api):IBaseTexture(pD11Api) 
{
	m_pTexture  = NULL;
	m_RefCount = 1;
	m_pD3D9RenderApi = pD11Api ; 
    memset(&m_TexInfo, 0 ,sizeof(m_TexInfo));
    REGISTER_DEVICE_LOST_RESOURCE(m_pD3D9RenderApi);
}


xD3D9BaseTexture::~xD3D9BaseTexture()
{
    REMOVE_DEVICE_LOST_RESOURCE(m_pD3D9RenderApi);
    unload();
}


unsigned long xD3D9BaseTexture::memUsage()
{
	return m_TexInfo.m_MemSize ; 
}

eResourceType xD3D9BaseTexture::res_type()
{
	return m_TexInfo.m_Type ; 
}

 bool xD3D9BaseTexture::desc(xTextureDesc& _desc)
 {
	 _desc.m_depth      = (int)m_TexInfo.m_TexDepth;
	 _desc.m_fmt        =      m_TexInfo.m_xfmt;
	 _desc.m_nArraySize = (int)m_TexInfo.m_ArraySize;
	 _desc.m_width      = (int)m_TexInfo.m_TexWidth;
	 _desc.m_height     = (int)m_TexInfo.m_TexHeight;
	 return true;
 }

bool  xD3D9BaseTexture::saveToFile(const wchar_t* fileName)
{
	D3DXIMAGE_FILEFORMAT fFormat = xD3D9ConstLexer::GetDXImageFileFormat(fileName);
	if((int)fFormat == -1)
		return false;

	if(m_pTexture == NULL)
		return false;
 
	HRESULT hr = D3DXSaveTextureToFileW(fileName , fFormat , m_pTexture , NULL);//  pResource ,  fFormat , fileName);
	return SUCCEEDED(hr);
}

bool xD3D9BaseTexture::unload()
{
     XSAFE_RELEASE(m_pTexture);
    return true;
}

void xD3D9BaseTexture::uploadSurfaceData(IDirect3DSurface9* pSurface , const char* pPixel, int src_picth ,  bool bFlip )
{
    IDirect3DDevice9*  pDevice = m_pD3D9RenderApi->getDevice();
    IDirect3DSurface9* pSurfLock = pSurface; 
    IDirect3DSurface9* pSysSurf  = NULL; 

    D3DSURFACE_DESC sDesc;
    pSurface->GetDesc(&sDesc);
	IDirect3DTexture9* pSysTexture;

    int w = sDesc.Width ;
    int h = sDesc.Height; 
    if(m_TexInfo.m_D3DPool == D3DPOOL_DEFAULT)
    {
        //pDevice->CreateOffscreenPlainSurface(sDesc.Width , sDesc.Height , sDesc.Format , D3DPOOL_SYSTEMMEM , &pSysSurf , NULL);
		if(pSysSurf == NULL)
		{
			pDevice->CreateTexture( sDesc.Width , sDesc.Height , 0 , 0 ,  sDesc.Format , D3DPOOL_SYSTEMMEM , &pSysTexture , NULL);
			pSysTexture->GetSurfaceLevel(0, &pSysSurf);
		}
		pSurfLock = pSysSurf;
    }


    D3DLOCKED_RECT LockedRect;
    pSurfLock->LockRect(&LockedRect,NULL,0);
    long dst_picth = LockedRect.Pitch;

    if(bFlip == false)
    {
        if(dst_picth == src_picth)
        {
            memcpy(LockedRect.pBits,pPixel,src_picth * h);
        }
        else
        {
            long final_picth = min(dst_picth,src_picth);
            char* pSrcLine = (char*)pPixel;
            char* pDstLine = (char*)LockedRect.pBits;
            for(int y = 0 ; y < h ; ++y)
            {
                memcpy(pDstLine , pSrcLine , src_picth);
                pSrcLine += src_picth;
                pDstLine += dst_picth;
            }
        }
    }
    else
    {
        long final_picth = min(dst_picth,src_picth);
        char* pSrcLine = (char*)pPixel  + src_picth * (h - 1);
        char* pDstLine = (char*)LockedRect.pBits;
        for(int y = 0 ; y < h ; ++y)
        {
            memcpy(pDstLine , pSrcLine , src_picth);
            pSrcLine -= src_picth;
            pDstLine += dst_picth;
        }
    }
    pSurfLock->UnlockRect();


    if(pSysSurf)
    {
        pDevice->UpdateSurface(pSysSurf , NULL , pSurface , NULL);
    }
	
	if(pSysTexture)
	{
		pSysTexture->Release();
		pSysTexture = NULL;
	}

    XSAFE_RELEASE(pSysSurf);
    pSurface->Release();
}

void xD3D9BaseTexture::uploadVolumeData(IDirect3DVolume9* pVolume , const char* pPixel, int RowPitch ,  int SlicePitch, bool bFlip )
{
    IDirect3DDevice9*  pDevice = m_pD3D9RenderApi->getDevice();
    D3DVOLUME_DESC vDesc;
    pVolume->GetDesc(&vDesc);
    int w = vDesc.Width;
    int h = vDesc.Height ;
    int depth = vDesc.Depth;

    
    D3DLOCKED_BOX LockedBox;
    pVolume->LockBox(&LockedBox,NULL,D3DLOCK_DISCARD);
    long dst_rowpicth   = LockedBox.RowPitch;
    long dst_slicePitch = LockedBox.SlicePitch;

    if(bFlip == false)
    {
        if(dst_rowpicth == RowPitch && dst_slicePitch == SlicePitch)
        {
            memcpy(LockedBox.pBits, pPixel, depth * SlicePitch );
        }
        else
        {
            char* pSrcSlice = (char*)pPixel;
            char* pDstSlice = (char*)LockedBox.pBits;

            for(int d = 0 ; d < depth ;  ++ d)
            {
 
                char* pSrcLine = (char*)pSrcSlice;
                char* pDstLine = (char*)pDstSlice;
                for(int y = 0 ; y < h ; ++y)
                {
                    memcpy(pDstLine , pSrcLine , RowPitch);
                    pSrcLine += RowPitch;
                    pDstLine += dst_rowpicth;
                }
                pSrcSlice += SlicePitch;
                pDstSlice += dst_slicePitch;
            }
        }
    }
    else
    {
        //鍬廬Slice
        char* pSrcSlice = (char*)pPixel  + (depth - 1) *  SlicePitch;
        char* pDstSlice = (char*)LockedBox.pBits;
        for(int d = 0 ; d < depth ;  ++ d)
        {

            char* pSrcLine = (char*)pSrcSlice;
            char* pDstLine = (char*)pDstSlice;
            for(int y = 0 ; y < h ; ++y)
            {
                memcpy(pDstLine , pSrcLine , RowPitch);
                pSrcLine += RowPitch;
                pDstLine += dst_rowpicth;
            }
            pSrcSlice -= SlicePitch;
            pDstSlice += dst_slicePitch;
        }
    }
    pVolume->UnlockBox();
    pVolume->Release();
}


bool  xD3D9BaseTexture::uploadTextureData(const char* pData[],int src_picth , int slice_pitch , int mipMapLevel , bool bFlip )
{
    IDirect3DTexture9* p2DTexture = NULL;
    m_pTexture->QueryInterface(IID_IDirect3DTexture9 , (void**) &p2DTexture);
    if(p2DTexture) 
    {
        IDirect3DSurface9* pSurface;
        p2DTexture->GetSurfaceLevel(mipMapLevel,&pSurface);
        uploadSurfaceData(pSurface , pData[0] , src_picth , bFlip);
        pSurface->UnlockRect();     
        p2DTexture->Release();
        return true;
    }

    IDirect3DCubeTexture9* pCubeTexture = NULL;
    m_pTexture->QueryInterface(IID_IDirect3DCubeTexture9 , (void**) &pCubeTexture);
    if(pCubeTexture )
    {
        for(int i = 0 ; i < 6 ; i ++)
        {
            D3DCUBEMAP_FACES FaceType = (D3DCUBEMAP_FACES)i;
            int mipMapLevel = 0;
            IDirect3DSurface9* pSurface;
            pCubeTexture->GetCubeMapSurface(FaceType , mipMapLevel, &pSurface);
            uploadSurfaceData(pSurface , pData[i] , src_picth , bFlip);
            pSurface->UnlockRect();  
        }
        pCubeTexture->Release();
        return true;
    }

    IDirect3DVolumeTexture9 * pVolumeTexture = NULL;
    m_pTexture->QueryInterface(IID_IDirect3DVolumeTexture9 , (void**) &pVolumeTexture);
    if(pVolumeTexture )
    {
        IDirect3DVolume9* pVolume  = NULL;
        pVolumeTexture->GetVolumeLevel(0 , &pVolume );
        uploadVolumeData(pVolume , pData[0] , src_picth , slice_pitch , bFlip);      
        pVolumeTexture->Release();
        return true;
    }

    return false;
}

 bool xD3D9BaseTexture::isLoaded()
 {
	 return m_pTexture != NULL;
 }

 void* xD3D9BaseTexture::handle()
 {
	 return m_pTexture;
 }

 bool xD3D9BaseTexture::_load(IDirect3DBaseTexture9* pTexture)
 {
     m_pTexture = pTexture;
     m_pTexture->AddRef();
     IDirect3DTexture9* p2DTexture = NULL;
     m_pTexture->QueryInterface(IID_IDirect3DTexture9 , (void**) &p2DTexture);
     if(p2DTexture) 
     {
         D3DSURFACE_DESC _LevelDesc;
         p2DTexture->GetLevelDesc(0 , &_LevelDesc);
         m_TexInfo.m_ResFmt     = _LevelDesc.Format;
         //野割
         m_TexInfo.m_ArraySize   = 1;
         m_TexInfo.m_TexWidth    = _LevelDesc.Width;
         m_TexInfo.m_TexHeight   = _LevelDesc.Height;
         m_TexInfo.m_TexDepth    = 1;
         m_TexInfo.m_MipmapLevel = p2DTexture->GetLevelCount();
         m_TexInfo.m_Type        = RESOURCE_TEXTURE2D;
         m_TexInfo.m_D3DPool    = _LevelDesc.Pool;
         p2DTexture->Release();
         return true;
     }

     IDirect3DCubeTexture9* pCubeTexture = NULL;
     m_pTexture->QueryInterface(IID_IDirect3DCubeTexture9 , (void**) &pCubeTexture);
     if(pCubeTexture )
     {
         D3DSURFACE_DESC _LevelDesc;
         pCubeTexture->GetLevelDesc(0 , &_LevelDesc);
         m_TexInfo.m_ResFmt     = _LevelDesc.Format;
         m_TexInfo.m_D3DPool    = _LevelDesc.Pool;
         //野割
         m_TexInfo.m_ArraySize   = 6;
         m_TexInfo.m_TexWidth    = _LevelDesc.Width;
         m_TexInfo.m_TexHeight   = _LevelDesc.Height;
         m_TexInfo.m_TexDepth    = 1;
         m_TexInfo.m_MipmapLevel = pCubeTexture->GetLevelCount();
         m_TexInfo.m_Type        = RESOURCE_TEXTURECube;
         pCubeTexture->Release();
         return true;
     }

     IDirect3DVolumeTexture9 * pVolumeTexture = NULL;
     m_pTexture->QueryInterface(IID_IDirect3DVolumeTexture9 , (void**) &pVolumeTexture);
     if(pVolumeTexture )
     {
         D3DVOLUME_DESC _LevelDesc ;
         pVolumeTexture->GetLevelDesc(0 , &_LevelDesc);
         m_TexInfo.m_ResFmt     = _LevelDesc.Format;
         m_TexInfo.m_D3DPool    = _LevelDesc.Pool;
         //野割
         m_TexInfo.m_ArraySize   = 1;
         m_TexInfo.m_TexWidth    = _LevelDesc.Width;
         m_TexInfo.m_TexHeight   = _LevelDesc.Height;
         m_TexInfo.m_TexDepth    = _LevelDesc.Depth;
         m_TexInfo.m_MipmapLevel = pVolumeTexture->GetLevelCount();
         m_TexInfo.m_Type        = RESOURCE_TEXTURE3D;    
         pVolumeTexture->Release();
         return true;
     }


	 m_TexInfo.m_Pitch = 0;
	 m_TexInfo.m_SlicePitch = 0;

	 //===================================
	 xD3D9GIFormatInfo* pFormat = xD3D9ConstLexer::singleton()->GetPixelFormat( m_TexInfo.m_ResFmt );
	 m_TexInfo.m_nBytePerPixel  = pFormat->m_compont * pFormat->m_bytePerComponent;
	 m_TexInfo.m_xfmt           = pFormat->m_fmt;
	 unsigned int iPitch        = (unsigned int) (m_TexInfo.m_TexWidth  * pFormat->m_compont * pFormat->m_bytePerComponent );
	 m_TexInfo.m_Pitch          = xFloorToPower2( iPitch );
	 m_TexInfo.m_SlicePitch     = (UINT)xFloorToPower2( (unsigned int)(m_TexInfo.m_Pitch * m_TexInfo.m_TexHeight) );
	 m_TexInfo.m_MemSize        = int( m_TexInfo.m_Pitch * m_TexInfo.m_ArraySize * m_TexInfo.m_TexDepth * m_TexInfo.m_TexHeight );
	 return true;
 }


 bool xD3D9BaseTexture::_grabRenderTagetData(IDirect3DSurface9* pRTSurface , void* pData , int x , int y , int w , int h)
 {
	 D3DSURFACE_DESC _SurfDesc;
	 pRTSurface->GetDesc(&_SurfDesc);

	 IDirect3DDevice9* pDevice  = m_pD3D9RenderApi->getDevice();
	 IDirect3DSurface9* pSysSurface = NULL;
	 pDevice->CreateOffscreenPlainSurface(_SurfDesc.Width , _SurfDesc.Height , _SurfDesc.Format , D3DPOOL_SYSTEMMEM , &pSysSurface , NULL);
	 HRESULT hr = pDevice->GetRenderTargetData(pRTSurface,pSysSurface); 
     if(FAILED(hr))
     {
         XSAFE_RELEASE(pSysSurface);
         return false;
     }

     D3DLOCKED_RECT LockRect;
     pSysSurface->LockRect(&LockRect , 0 , D3DLOCK_READONLY);

     const char* pSrcLine = (const char*)LockRect.pBits + x * m_TexInfo.m_nBytePerPixel;
     pSrcLine += LockRect.Pitch * y;

     char*       pDstLine = (char*)pData;
     int dstPitch = m_TexInfo.m_nBytePerPixel * w;
     int line_len =  dstPitch;
     int max_len  =  LockRect.Pitch - x * m_TexInfo.m_nBytePerPixel;
     if(dstPitch > max_len ) line_len = max_len;

	 if(h > 0)
	 {
		 for(int _y = 0 ; _y < (int)h ; _y ++ )
		 {
			 memcpy(pDstLine , pSrcLine , line_len );
			 pDstLine += dstPitch;
			 pSrcLine += LockRect.Pitch;
		 }

	 }
	 else
	 {
		 for(int _y = 0 ; _y < -(int)h ; _y ++ )
		 {
			 memcpy(pDstLine , pSrcLine , line_len );
			 pDstLine += dstPitch;
			 pSrcLine -= LockRect.Pitch;
		 }

	 }

     pSysSurface->UnlockRect();
     XSAFE_RELEASE(pSysSurface);
     return true;

 }

END_NAMESPACE_XEVOL3D


