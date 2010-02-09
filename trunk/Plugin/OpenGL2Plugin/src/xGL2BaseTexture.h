#ifndef _XEVOL_OPENGL20_BASE_TEXTURE_H_
#define _XEVOL_OPENGL20_BASE_TEXTURE_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <D3DX10.h>
#include <D3DX10tex.h>
#include <string>
#include <vector>
#include <RenderAPI/xBaseTexture.h>
#include "xGL2ConstLexer.h"


BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderApi;
struct xGL2TexInfo
{
	DXGI_FORMAT   m_ShaderViewFmt;
    DXGI_FORMAT   m_ResFmt;
    DXGI_FORMAT   m_RTViewFmt;
	size_t        m_TexWidth;
	size_t        m_TexHeight;
	size_t        m_TexDepth;
	size_t        m_MipmapLevel;
	size_t        m_ArraySize;
	eResourceType m_Type;
	ePIXEL_FORMAT m_xfmt;
	int           m_MemSize;
	int           m_Pitch;
	int           m_SlicePitch;
	int           m_nBytePerPixel;
};

class xGL2TextureBuffer
{
public:
	char* m_pixel;
	int   m_pitch;
	int   m_depth;
	int   m_dataLen;
	void* m_destBuffer;
public:
	xGL2TextureBuffer();
	~xGL2TextureBuffer();
	void  setDestBuffer(void* destBuffer);
	bool  create(int pitch , int h);
	bool  unload();
	bool  flush(int h ,  int dest_pitch , int src_pitch = 0);
};

class xGL2BaseTexture : public IBaseTexture
{
protected:
	xGL2RenderApi*          m_pD10Api;
	ID3D10ShaderResourceView* m_pTextureView;
	xGL2TexInfo               m_TexInfo;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xGL2BaseTexture);
public:
	xGL2BaseTexture(xGL2RenderApi* pD10Api);
	virtual ~xGL2BaseTexture();
	virtual unsigned long  memUsage();
	virtual eResourceType  res_type();
	virtual IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0){return NULL ; }
	virtual ePIXEL_FORMAT  format(){ return m_TexInfo.m_xfmt ; }
	virtual bool           validate(){return true; }
	virtual bool           desc(xTextureDesc& _desc);
	virtual bool           grabRenderTagetData(int x , int y , int w , int h , void* pData) { return false ; }
    virtual bool           unload();
};

class xGL2UnkwonTexture : public xGL2BaseTexture
{
protected:
	ID3D10Resource*           m_pTexture;
	ID3D10ShaderResourceView* m_pTextureView;
public:
	void*                 handle();
	virtual bool          isLoaded();
	virtual bool          unload();
protected:
	virtual bool          load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;

	virtual bool          update(void* data  , int dateLen ,int mipmapLevel = 0 , int arraySlice = 0){ return NULL ;}
	virtual bool          lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0){ return false ;}
	virtual bool          unlock(xTextureLockArea& lockInfo){return false;}
protected:
	virtual bool          _load(ID3D10Resource* pTexture , bool bCreateTextureView = true);
	xGL2UnkwonTexture(xGL2RenderApi* pD10Api);
	virtual ~xGL2UnkwonTexture();
};
bool fillLoadInfo(D3DX10_IMAGE_LOAD_INFO& loadInfo , bool lockAble);
END_NAMESPACE_XEVOL3D

#endif
