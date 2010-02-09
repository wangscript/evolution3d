#ifndef _XEVOL_DIRECT3D10_BASE_RENDERTARGET_H_
#define _XEVOL_DIRECT3D10_BASE_RENDERTARGET_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <D3DX10.h>
#include <D3DX10tex.h>
#include <string>
#include <vector>
#include <RenderAPI/xBaseTexture.h>
#include <RenderAPI/xRenderView.h>
BEGIN_NAMESPACE_XEVOL3D
class xD3D10RenderApi;
class xD10BaseRenderTarget : public IRenderTarget
{
protected:
	xD3D10RenderApi*          m_pD10Api;
	IBaseTexture*             m_pBaseTexture;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD10BaseRenderTarget);
public:
	xD10BaseRenderTarget(xD3D10RenderApi* pD10Api);
	virtual ~xD10BaseRenderTarget();
	IBaseTexture* toTexture(){return m_pBaseTexture ; }
	bool          desc(xTextureDesc& desc){return m_pBaseTexture->desc(desc) ; }
	bool          grabRenderTagetData(int x , int y , int w , int h , void* pData);
	void          setTexture(IBaseTexture* pTexture);
	bool          saveResourceToFile(const wchar_t* fileName , ID3D10Resource* pRes); 
};


END_NAMESPACE_XEVOL3D

#endif
