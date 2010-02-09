#ifndef _XEVOL_OPENGL20_BASE_RENDERTARGET_H_
#define _XEVOL_OPENGL20_BASE_RENDERTARGET_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <D3DX10.h>
#include <D3DX10tex.h>
#include <string>
#include <vector>
#include <RenderAPI/xBaseTexture.h>
#include <RenderAPI/xRenderView.h>
BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderApi;

class xGL2BaseRenderTarget : public IRenderTarget
{
protected:
	xGL2RenderApi*          m_pD10Api;
	IBaseTexture*             m_pBaseTexture;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xGL2BaseRenderTarget);
public:
	xGL2BaseRenderTarget(xGL2RenderApi* pD10Api);
	virtual ~xGL2BaseRenderTarget();
	IBaseTexture* toTexture(){return m_pBaseTexture ; }
	bool          desc(xTextureDesc& desc){return m_pBaseTexture->desc(desc) ; }
	bool          grabRenderTagetData(int x , int y , int w , int h , void* pData);
	void          setTexture(IBaseTexture* pTexture);
};


END_NAMESPACE_XEVOL3D

#endif
