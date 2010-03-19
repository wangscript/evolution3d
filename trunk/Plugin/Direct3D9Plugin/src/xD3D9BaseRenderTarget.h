#ifndef _XEVOL_DIRECT3D9_BASE_RENDERTARGET_H_
#define _XEVOL_DIRECT3D9_BASE_RENDERTARGET_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
#include <RenderAPI/xBaseTexture.h>
#include <RenderAPI/xRenderView.h>

#include "xDirect3D9APIResource.h"
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;

class xD3D9BaseRenderTarget : public IRenderTarget
{
protected:
	xD3D9RenderApi*           m_pD3D9RenderApi;
	IBaseTexture*             m_pBaseTexture;
    int                       m_ArraySlice;
    int                       m_MipmapLevel;
    IDirect3DSurface9*        m_pRenderTargetView;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD3D9BaseRenderTarget);
    DECLARE_DEVICE_LOST(xD3D9BaseRenderTarget);
    virtual void onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action) = 0;
public:
	xD3D9BaseRenderTarget(xD3D9RenderApi* pD11Api , int arraySlice , int mipmapLevel);
	virtual ~xD3D9BaseRenderTarget();
	IBaseTexture*   toTexture(){return m_pBaseTexture ; }
	bool            desc(xTextureDesc& desc){return m_pBaseTexture->desc(desc) ; }
	bool            grabRenderTagetData(void* pData , int x , int y , int w , int h);
	void            setTexture(IBaseTexture* pTexture);
	bool            saveResourceToFile(const wchar_t* fileName , IDirect3DSurface9* pRes); 
};


END_NAMESPACE_XEVOL3D

#endif
