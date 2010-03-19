#ifndef _XEVOL_DIRECT3D11_BASE_TEXTURE_H_
#define _XEVOL_DIRECT3D11_BASE_TEXTURE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
#include <RenderAPI/xBaseTexture.h>
#include "xD11ConstLexer.h"
#include "../xDirect3D11DllApi.h"

BEGIN_NAMESPACE_XEVOL3D
class xD3D11RenderApi;
struct xD11TexInfo
{
	DXGI_FORMAT   m_ShaderViewFmt;
    DXGI_FORMAT   m_ResFmt;
    DXGI_FORMAT   m_RTViewFmt;
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
	D3D11_USAGE   m_Usage;
};

class xD11TextureBuffer
{
public:
	char* m_pixel;
	int   m_pitch;
	int   m_depth;
	int   m_dataLen;
	void* m_destBuffer;
public:
	xD11TextureBuffer();
	~xD11TextureBuffer();
	void  setDestBuffer(void* destBuffer);
	bool  create(int pitch , int h);
	bool  unload();
	bool  flush(int h ,  int dest_pitch , int src_pitch = 0);
};

class xD11BaseTexture : public IBaseTexture
{
protected:
	xD3D11RenderApi*          m_pD11RenderApi;
	ID3D11ShaderResourceView* m_pTextureView;
	xD11TexInfo               m_TexInfo;
	DXGI_SAMPLE_DESC          m_SampleDesc;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD11BaseTexture);
public:
	xD11BaseTexture(xD3D11RenderApi* pD11Api);
	virtual ~xD11BaseTexture();
	virtual unsigned long   memUsage();
	virtual eResourceType   res_type();
	virtual IRenderTarget*  toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0){return NULL ; }
	virtual ePIXEL_FORMAT   format(){ return m_TexInfo.m_xfmt ; }
	virtual bool            validate(){return true; }
	virtual bool            desc(xTextureDesc& _desc);
	virtual bool            grabRenderTagetData(void* pData , int x , int y , int w , int h , int arraySlice , int mipmapLevel) { return false ; }
    virtual bool            unload();
	virtual bool            saveToFile(const wchar_t* fileName);
	virtual ID3D11Resource* GetD11Resource();
};

class xD11UnkwonTexture : public xD11BaseTexture
{
protected:
	ID3D11Resource*           m_pTexture;
public:
	void*                 handle();
	virtual bool          isLoaded();
	virtual bool          unload();
protected:
	virtual bool          load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;

	virtual bool          update(void* data  , int dateLen , int rowPitch , int depthPicth , int mipmapLevel = 0 , int arraySlice = 0){ return NULL ;}
	virtual bool          lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0){ return false ;}
	virtual bool          unlock(xTextureLockArea& lockInfo){return false;}
protected:
	virtual bool          _load(ID3D11Resource* pTexture , bool bCreateTextureView = true);
	xD11UnkwonTexture(xD3D11RenderApi* pD11Api);
	virtual ~xD11UnkwonTexture();
};
bool fillLoadInfo(D3DX11_IMAGE_LOAD_INFO& loadInfo , bool lockAble , bool bAsRenderTarget);
END_NAMESPACE_XEVOL3D

#endif
