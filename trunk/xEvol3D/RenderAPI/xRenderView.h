#ifndef __XEVOL_BASE_RENDERVIEW_H__
#define __XEVOL_BASE_RENDERVIEW_H__
#include "xRenderApiObject.h"
#include "xRenderApiFwDecl.h"
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
struct _XEVOL_BASE_API_ xRTSampleDesc
{
	xRTSampleDesc(int Count = 1 , int Qulity = 0);
public:
    int m_SampleCount;
    int m_SampleQulity;
public:
    static xRTSampleDesc Defualt;
    static xRTSampleDesc None;
};

class _XEVOL_BASE_API_ IRenderTarget : public IRenderApiObject
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderTarget);
public:
	IRenderTarget(IRenderApi* pRenderApi);
	virtual ~IRenderTarget();
	virtual IBaseTexture* toTexture() = 0;
	virtual bool          desc(xTextureDesc& desc) = 0;
	virtual bool          grabRenderTagetData(void* pData , int x , int y , int w , int h) = 0;
	virtual bool          saveToFile(const wchar_t* fileName) = 0;
};

class _XEVOL_BASE_API_ IRenderView : public IRenderApiObject
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderView);
public:
	IRenderView(IRenderApi* pRenderApi ,const xRTSampleDesc& sampleDesc);
	virtual ~IRenderView();
public:
	virtual bool              setup() = 0;
	virtual bool              desc(xTextureDesc& desc) = 0;
	virtual bool              resize(int w , int h) = 0;
	virtual bool              resize(int w , int h  , bool bDestorRT) = 0;
	virtual bool              clear(xColor_4f& bkColor) = 0;
	virtual bool              clear(xColor_4f& bkColor, int nClear) = 0;
	virtual bool              clearRenderTarget(xColor_4f& bkColor, int idx) = 0;
	virtual bool              clear(xColor_4f& bkColor , float z , unsigned int stencil) = 0;
	virtual bool              clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear) = 0;
    virtual bool              createRenderTarget(int nRenderTarget , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture) = 0;
	virtual bool              setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx , size_t iSlice = 0 , size_t iMipMapLevel = 0) = 0;
	virtual bool              setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx) = 0;
	virtual bool              setDepthBuffer(IRenderTarget* pDepthBuffer) = 0;
	virtual int               addRenderTarget(IBaseTexture*  pTexture  , size_t iSlice = 0 , size_t iMipMapLevel = 0 ) = 0;
	virtual int               addRenderTarget(IRenderTarget* pRenderTarget) = 0;
	virtual IRenderTarget*    depthBuffer() = 0;
	virtual IRenderApiObject* renderTarget(size_t rtIdx ) = 0;
	virtual void              getSampleDesc(xRTSampleDesc& desc){ desc = m_RTSampleDesc ; }
protected:
	xRTSampleDesc             m_RTSampleDesc;
};

END_NAMESPACE_XEVOL3D
#endif
