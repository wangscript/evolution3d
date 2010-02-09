#pragma once
#include "xGL2BaseRenderTarget.h"
#include "xGL2BaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderTexture;
class xGL2RenderTarget;
class xGL2RenderTexture;

class xGL2RenderTarget : public xGL2BaseRenderTarget
{
	ID3D10RenderTargetView*   m_pRenderTargetView;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2RenderTarget);
	int           m_mipLevel;
	int           m_arraySlice;
public:
	int           RefCount();
	int           AddRef();
	int           ReleaseObject();
	int           KillObject();
public:
	xGL2RenderTarget(xGL2RenderApi* pAPI);
	virtual       ~xGL2RenderTarget();
	void*         handle(){ return m_pRenderTargetView ;}
	bool          destory();
	bool          create(ID3D10Resource* pTexture , xGL2TexInfo& TexInfo , int iSlice = 0, int mipLevel = 0);
};

class xGL2DynamicRenderTarget : public xGL2RenderTarget
{
public:
    xGL2DynamicRenderTarget(xGL2RenderApi* pAPI):xGL2RenderTarget(pAPI){}
    int           ReleaseObject();
    int           KillObject();
};

class xGL2RenderTexture : public xGL2UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2RenderTexture);
	xGL2RenderTarget m_RenderTarget;
	bool             m_bLockable;
	bool             m_bCanUseTexute;
	ID3D10Texture2D* m_pSysTexture;
protected:
	bool           __createSysTexture(int w , int h , DXGI_FORMAT fmt);
public:
	xGL2RenderTexture(bool lockAble ,  bool canUseTexture , xGL2RenderApi* pRenderApi);
	~xGL2RenderTexture();
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



class xGL2DepthBuffer : public xGL2BaseRenderTarget
{
	ID3D10DepthStencilView*   m_pDepthView;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2DepthBuffer);
public:
	int      RefCount();
	int      AddRef();
	int      ReleaseObject();
	int      KillObject();
public:
	xGL2DepthBuffer(xGL2RenderApi* pAPI);
	virtual ~xGL2DepthBuffer();
	void*         handle(){ return m_pDepthView ;}
	bool          destory();
	bool          create(ID3D10Resource* pTexture , xGL2TexInfo& TexInfo);
};

class xGL2DepthTexture : public xGL2UnkwonTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2DepthTexture);
	xGL2DepthBuffer m_RenderTarget;
	bool            m_bLockable;
	bool            m_bCanUseTexute;
public:
	xGL2DepthTexture(bool lockAble ,  bool canUseTexture , xGL2RenderApi* pRenderApi);
	~xGL2DepthTexture();
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

