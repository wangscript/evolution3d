#ifndef _XEVOL_DIRECT3D11_BASE_RENDERTARGET_H_
#define _XEVOL_DIRECT3D11_BASE_RENDERTARGET_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
#include <RenderAPI/xBaseTexture.h>
#include <RenderAPI/xRenderView.h>
#include "../xDirect3D11DllApi.h"
BEGIN_NAMESPACE_XEVOL3D
class xD3D11RenderApi;

class xD11BaseRenderTarget : public IRenderTarget
{
protected:
	xD3D11RenderApi*          m_pD11RenderApi;
	IBaseTexture*             m_pBaseTexture;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD11BaseRenderTarget);
public:
	xD11BaseRenderTarget(xD3D11RenderApi* pD11Api);
	virtual ~xD11BaseRenderTarget();
	IBaseTexture*   toTexture(){return m_pBaseTexture ; }
	bool            desc(xTextureDesc& desc){return m_pBaseTexture->desc(desc) ; }
	bool            grabRenderTagetData(int x , int y , int w , int h , void* pData);
	void            setTexture(IBaseTexture* pTexture);
	bool            saveResourceToFile(const wchar_t* fileName , ID3D11Resource* pRes); 
};


END_NAMESPACE_XEVOL3D

#endif
