#pragma once
#include "xD3D9BaseRenderTarget.h"
#include "xD3D9BaseTexture.h"

BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderTarget;

class xD3D9RenderTarget : public xD3D9BaseRenderTarget
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD3D9RenderTarget);
public:
	int           RefCount();
	int           AddRef();
	int           ReleaseObject();
	int           KillObject();
public:
	xD3D9RenderTarget(xD3D9RenderApi* pAPI , int arraySlice , int mipmapLevel);
	virtual       ~xD3D9RenderTarget();
	void*         handle();
	bool          destory();
	bool          create(xD3D9BaseTexture* pTexture , int iSlice = 0, int mipLevel = 0);
	bool          saveToFile(const wchar_t* fileName);
    void          onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);
protected:
    bool              m_bResourceLosted;
    xD3D9BaseTexture* m_pD3D9BaseTexture;
};

class xD3D9DynamicRenderTarget : public xD3D9RenderTarget
{
public:
    xD3D9DynamicRenderTarget(xD3D9RenderApi* pAPI , int arraySlice , int mipmapLevel):xD3D9RenderTarget(pAPI , arraySlice , mipmapLevel){}
    void          onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);
    void*         handle();
public:
    IMPL_BASE_OBJECT_INTERFACE(xD3D9DynamicRenderTarget);
};


class xD3D9DepthBuffer : public xD3D9BaseRenderTarget
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD3D9DepthBuffer);
public:
	int           RefCount();
	int           AddRef();
	int           ReleaseObject();
	int           KillObject();
public:
	xD3D9DepthBuffer(xD3D9RenderApi* pAPI);
	virtual ~xD3D9DepthBuffer();
    void*         handle(); 
	bool          destory();
	bool          create(int w , int h , D3DFORMAT fmt );
	bool          saveToFile(const wchar_t* fileName);
    bool          desc(xTextureDesc& desc);
    void          onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);
protected:
    int       m_Width;
    int       m_Height;
    D3DFORMAT m_Format;
};

END_NAMESPACE_XEVOL3D

