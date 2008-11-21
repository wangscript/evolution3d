#pragma once
#include "xD10BaseRenderTarget.h"
#include "xD10BaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D
class xD10RenderTexture;
class xD10RenderTarget;
class xD10RenderTexture;

class xD10RenderTarget : public xD10BaseRenderTarget
{
	ID3D10RenderTargetView*   m_pRenderTargetView;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10RenderTarget);
	int m_mipLevel;
	int m_arraySlice;
public:
	int      RefCount();
	int      AddRef();
	int      ReleaseObject();
	int      KillObject();
public:
	xD10RenderTarget(xD3D10RenderApi* pAPI);
	virtual       ~xD10RenderTarget();
	void*         handle(){ return m_pRenderTargetView ;}
	bool          destory();
	bool          create(ID3D10Resource* pTexture , xD10TexInfo& TexInfo , int iSlice = 0, int mipLevel = 0);
};

class xD10RenderTexture : public xD10UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10RenderTexture);
	xD10RenderTarget m_RenderTarget;
	bool             m_bLockable;
	bool             m_bCanUseTexute;
	ID3D10Texture2D* m_pSysTexture;
protected:
	bool           __createSysTexture(int w , int h , DXGI_FORMAT fmt);
public:
	xD10RenderTexture(bool lockAble ,  bool canUseTexture , xD3D10RenderApi* pRenderApi);
	~xD10RenderTexture();
	bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0);
	bool           unlock(xTextureLockArea& lockInfo);
	bool           create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1);
	bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1);
	IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
	bool           grabRenderTagetData(int x , int y , int w , int h , void* pData);
private:
	bool           load(const wchar_t* fileName , unsigned long  arg){ return false ; }
	bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg){ return false ; }
};



class xD10DepthBuffer : public xD10BaseRenderTarget
{
	ID3D10DepthStencilView*   m_pDepthView;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10DepthBuffer);
public:
	int      RefCount();
	int      AddRef();
	int      ReleaseObject();
	int      KillObject();
public:
	xD10DepthBuffer(xD3D10RenderApi* pAPI);
	virtual ~xD10DepthBuffer();
	void*         handle(){ return m_pDepthView ;}
	bool          destory();
	bool          create(ID3D10Resource* pTexture , xD10TexInfo& TexInfo);
};

class xD10DepthTexture : public xD10UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10DepthTexture);
	xD10DepthBuffer m_RenderTarget;
	bool            m_bLockable;
	bool            m_bCanUseTexute;
public:
	xD10DepthTexture(bool lockAble ,  bool canUseTexture , xD3D10RenderApi* pRenderApi);
	~xD10DepthTexture();
	bool           create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1);
	IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
private:
	bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1){return false;}
	bool           load(const wchar_t* fileName , unsigned long  arg){ return false ; }
	bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg){ return false ; }
};

END_NAMESPACE_XEVOL3D

