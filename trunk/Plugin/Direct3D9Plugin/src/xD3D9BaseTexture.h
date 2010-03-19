#ifndef _XEVOL_DIRECT3D9_BASE_TEXTURE_H_
#define _XEVOL_DIRECT3D9_BASE_TEXTURE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
#include <RenderAPI/xBaseTexture.h>
#include "xD3D9ConstLexer.h"
#include "xDirect3D9APIResource.h"

BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
struct xD3D9TexInfo
{
    D3DFORMAT     m_ResFmt;
	int           m_TexWidth;
	int           m_TexHeight;
	int           m_TexDepth;
	int           m_MipmapLevel;
	int           m_ArraySize;
	eResourceType m_Type;
	ePIXEL_FORMAT m_xfmt;
	int           m_MemSize;
	int           m_Pitch;
	int           m_SlicePitch;
	int           m_nBytePerPixel;
	D3DPOOL       m_D3DPool;
};

class xD3D9TextureBuffer
{
public:
	char* m_pixel;
	int   m_pitch;
	int   m_depth;
	int   m_dataLen;
	void* m_destBuffer;
public:
	xD3D9TextureBuffer();
	~xD3D9TextureBuffer();
	void  setDestBuffer(void* destBuffer);
	bool  create(int pitch , int h);
	bool  unload();
	bool  flush(int h ,  int dest_pitch , int src_pitch = 0);
};

class xD3D9BaseTexture : public IBaseTexture
{
protected:
	xD3D9RenderApi*          m_pD3D9RenderApi;
	xD3D9TexInfo             m_TexInfo;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD3D9BaseTexture);
public:
	xD3D9BaseTexture(xD3D9RenderApi* pD11Api);
	virtual ~xD3D9BaseTexture();
	virtual unsigned long   memUsage();
	virtual eResourceType   res_type();
	virtual IRenderTarget*  toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0){return NULL ; }
	virtual ePIXEL_FORMAT   format(){ return m_TexInfo.m_xfmt ; }
	virtual bool            validate(){return true; }
	virtual bool            desc(xTextureDesc& _desc);
	virtual bool            grabRenderTagetData(void* pData , int x , int y , int w , int h  , int arraySlice , int mipmapLevel ) { return false ; }
    virtual bool            unload();
	virtual bool            saveToFile(const wchar_t* fileName);
    virtual bool            uploadTextureData(const char* pData[], int src_picth , int slice_picth , int mipMapLevel , bool bFlip );
    virtual void            uploadSurfaceData(IDirect3DSurface9* pSurface , const char* pPixel , int src_picth ,  bool bFlip );
    virtual void            uploadVolumeData(IDirect3DVolume9* pVolume , const char* pPixel, int RowPitch ,  int SlicePitch, bool bFlip );
    virtual bool            _grabRenderTagetData(IDirect3DSurface9* pRTSurface , void* pData , int x , int y , int w , int h);

    void*                 handle();
    virtual bool          isLoaded();
    virtual bool          load(const wchar_t* fileName , unsigned long  arg) = 0;
    virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;

    virtual bool          update(void* data  , int dateLen , int rowPitch , int depthPicth , int mipmapLevel = 0 , int arraySlice = 0){ return NULL ;}
    virtual bool          lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0){ return false ;}
    virtual bool          unlock(xTextureLockArea& lockInfo){return false;}

    xD3D9TexInfo&         texInfo(){return m_TexInfo ; }
   
    virtual void onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action) = 0;

public:
    IDirect3DBaseTexture9* BaseDXTexture(){ return m_pTexture ; }
protected:
    virtual bool          _load(IDirect3DBaseTexture9* pTexture);
protected:
    IDirect3DBaseTexture9* m_pTexture;

    DECLARE_DEVICE_LOST(xD3D9BaseTexture);
};



END_NAMESPACE_XEVOL3D

#endif
