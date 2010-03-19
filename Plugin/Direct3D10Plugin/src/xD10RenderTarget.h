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
public:
	int           RefCount();
	int           AddRef();
	int           ReleaseObject();
	int           KillObject();
public:
	xD10RenderTarget(xD3D10RenderApi* pAPI , int arraySlice , int mipmapLevel);
	virtual       ~xD10RenderTarget();
	void*         handle(){ return m_pRenderTargetView ;}
	bool          destory();
	bool          create(ID3D10Resource* pTexture , xD10TexInfo& TexInfo , DXGI_SAMPLE_DESC& sampleDesc , int iSlice = 0, int mipLevel = 0);
	bool          saveToFile(const wchar_t* fileName);
};

class xD10DynamicRenderTarget : public xD10RenderTarget
{
public:
    xD10DynamicRenderTarget(xD3D10RenderApi* pAPI, int arraySlice , int mipmapLevel):xD10RenderTarget(pAPI , arraySlice , mipmapLevel){}
    int           ReleaseObject();
    int           KillObject();
};

class xD10RenderTexture : public xD10UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10RenderTexture);
	xD10RenderTarget m_RenderTarget;
	bool             m_bLockable;
	bool             m_bCanUseTexture;
	ID3D10Resource*  m_pSysTexture;
protected:
	bool           __createSysTexture();
    bool           grabRenderTagetDataTexture2D(void* pData, int x , int y , int w , int h , int arraySlice , int mipmapLevel);
public:
	xD10RenderTexture(bool lockAble ,  bool canUseTexture , xD3D10RenderApi* pRenderApi , DXGI_SAMPLE_DESC SampleDesc);
	~xD10RenderTexture();
	bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0);
	bool           unlock(xTextureLockArea& lockInfo);
	bool           create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0);
	IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
    bool           isSameInstance(IRenderTarget* pRenderTarget);
	bool           grabRenderTagetData(void* pData, int x , int y , int w , int h , int arraySlice , int mipmapLevel);
    bool           update(void* data  , int dateLen ,int rowPitch , int depthPicth =0 , int mipmapLevel = 0 , int arraySlice = 0);
private:
	bool           load(const wchar_t* fileName , unsigned long  arg){ return false ; }
	bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg){ return false ; }
    bool           unload();
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
	xD10DepthBuffer(xD3D10RenderApi* pAPI , int arraySlice , int mipmapLevel);
	virtual ~xD10DepthBuffer();
	void*         handle(){ return m_pDepthView ;}
	bool          destory();
	bool          create(ID3D10Resource* pTexture , xD10TexInfo& TexInfo , DXGI_SAMPLE_DESC& sampleDesc);
	bool          saveToFile(const wchar_t* fileName);
};

class xD10DepthTexture : public xD10UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10DepthTexture);
	xD10DepthBuffer m_RenderTarget;
	bool            m_bLockable;
	bool            m_bCanUseTexute;
public:
	xD10DepthTexture(bool lockAble ,  bool canUseTexture , xD3D10RenderApi* pRenderApi , DXGI_SAMPLE_DESC SampleDesc);
	~xD10DepthTexture();
	bool           create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0);
	IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
    bool           unload();
    bool           isSameInstance(IRenderTarget* pRenderTarget);
private:
	bool           load(const wchar_t* fileName , unsigned long  arg){ return false ; }
	bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg){ return false ; }

};

END_NAMESPACE_XEVOL3D

