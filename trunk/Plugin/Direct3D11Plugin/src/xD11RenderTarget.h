#pragma once
#include "xD11BaseRenderTarget.h"
#include "xD11BaseTexture.h"
#include "../xDirect3D11DllApi.h"
BEGIN_NAMESPACE_XEVOL3D
class xD11RenderTexture;
class xD11RenderTarget;
class xD11RenderTexture;

class xD11RenderTarget : public xD11BaseRenderTarget
{
	ID3D11RenderTargetView*   m_pRenderTargetView;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11RenderTarget);
	int           m_mipLevel;
	int           m_arraySlice;
public:
	int           RefCount();
	int           AddRef();
	int           ReleaseObject();
	int           KillObject();
public:
	xD11RenderTarget(xD3D11RenderApi* pAPI);
	virtual       ~xD11RenderTarget();
	void*         handle(){ return m_pRenderTargetView ;}
	bool          destory();
	bool          create(ID3D11Resource* pTexture , xD11TexInfo& TexInfo , DXGI_SAMPLE_DESC& sampleDesc , int iSlice = 0, int mipLevel = 0);
	bool          saveToFile(const wchar_t* fileName);
};

class xD11DynamicRenderTarget : public xD11RenderTarget
{
public:
    xD11DynamicRenderTarget(xD3D11RenderApi* pAPI):xD11RenderTarget(pAPI){}
    int           ReleaseObject();
    int           KillObject();
};

class xD11RenderTexture : public xD11UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11RenderTexture);
	xD11RenderTarget m_RenderTarget;
	bool             m_bLockable;
	bool             m_bCanUseTexute;
	ID3D11Texture2D* m_pSysTexture;
protected:
	bool           __createSysTexture(int w , int h , DXGI_FORMAT fmt);
public:
	xD11RenderTexture(bool lockAble ,  bool canUseTexture , xD3D11RenderApi* pRenderApi , DXGI_SAMPLE_DESC SampleDesc);
	~xD11RenderTexture();
	bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0);
	bool           unlock(xTextureLockArea& lockInfo);
	bool           create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1);
	bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1);
	IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
    bool           isSameInstance(IRenderTarget* pRenderTarget);
	bool           grabRenderTagetData(int x , int y , int w , int h , void* pData);
private:
	bool           load(const wchar_t* fileName , unsigned long  arg){ return false ; }
	bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg){ return false ; }
    bool           unload();
};



class xD11DepthBuffer : public xD11BaseRenderTarget
{
	ID3D11DepthStencilView*   m_pDepthView;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11DepthBuffer);
public:
	int      RefCount();
	int      AddRef();
	int      ReleaseObject();
	int      KillObject();
public:
	xD11DepthBuffer(xD3D11RenderApi* pAPI);
	virtual ~xD11DepthBuffer();
	void*         handle(){ return m_pDepthView ;}
	bool          destory();
	bool          create(ID3D11Resource* pTexture , xD11TexInfo& TexInfo , DXGI_SAMPLE_DESC& sampleDesc);
	bool          saveToFile(const wchar_t* fileName);
};

class xD11DepthTexture : public xD11UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11DepthTexture);
	xD11DepthBuffer  m_RenderTarget;
	bool             m_bLockable;
	bool             m_bCanUseTexute;
public:
	xD11DepthTexture(bool lockAble ,  bool canUseTexture , xD3D11RenderApi* pRenderApi ,  DXGI_SAMPLE_DESC SampleDesc);
	~xD11DepthTexture();
	bool           create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1);
	IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0);
    bool           unload();
    bool           isSameInstance(IRenderTarget* pRenderTarget);
private:
	bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1){return false;}
	bool           load(const wchar_t* fileName , unsigned long  arg){ return false ; }
	bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg){ return false ; }

};

END_NAMESPACE_XEVOL3D

