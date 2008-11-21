#ifndef __XEVOL_BASE_RENDERVIEW_H__
#define __XEVOL_BASE_RENDERVIEW_H__
#include "xRenderObject.h"
#include "xRenderApiFwDecl.h"
#include <vector>
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ IRenderTarget : public IRenderObject
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IGpuProgram);
public:
	IRenderTarget(IRenderApi* pRenderApi);
	virtual ~IRenderTarget();
	virtual IBaseTexture* toTexture() = 0;
	virtual bool          desc(xTextureDesc& desc) = 0;
	virtual bool          grabRenderTagetData(int x , int y , int w , int h , void* pData) = 0;
};

class _XEVOL_BASE_API_ IRenderView : public IRenderObject
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderView);
public:
	IRenderView(IRenderApi* pRenderApi);
	virtual ~IRenderView();
public:
	virtual bool              setup() = 0;
	virtual bool              desc(xTextureDesc& desc) = 0;
	virtual bool              resize(int w , int h) = 0;
	virtual bool              clear(xColor_4f& bkColor) = 0;
	virtual bool              clear(xColor_4f& bkColor, int nClear) = 0;
	virtual bool              clear(xColor_4f& bkColor , float z , unsigned int stencil) = 0;
	virtual bool              clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear) = 0;
	virtual bool              createRenderTarget(int nRenderTarget , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture) = 0;
	virtual bool              setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx) = 0;
	virtual bool              setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx) = 0;
	virtual bool              setDepthBuffer(IRenderTarget* pDepthBuffer) = 0;
	virtual int               addRenderTarget(IBaseTexture*  pTexture) = 0;
	virtual int               addRenderTarget(IRenderTarget* pRenderTarget) = 0;
	virtual IRenderTarget*    depthBuffer() = 0;
	virtual IRenderObject*    renderTarget(size_t rtIdx ) = 0;
};

END_NAMESPACE_XEVOL3D
#endif
