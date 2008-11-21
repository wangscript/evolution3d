#ifndef _XEVOL_RENDER_API_OBJECT_H_
#define _XEVOL_RENDER_API_OBJECT_H_
#include "xRenderApiDef.h"
#include "xRenderApiFwDecl.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_   IRenderObject : public IBaseObject
{
protected:
	IRenderApi* m_pRenderApi;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderObject);
public:
	virtual void* handle(){return 0 ; }
	IRenderObject(IRenderApi* pOwner);
	virtual ~IRenderObject();
};

class _XEVOL_BASE_API_  IRenderResource : public IRenderObject
{
protected:
	IRenderApi* m_pRenderApi;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderResource);
public:
	IRenderApi* renderApi(){return m_pRenderApi ; }
	IRenderResource(IRenderApi* pOwner);
	virtual ~IRenderResource();
	virtual void*         handle(){return 0 ; }
	virtual eResourceType res_type() = 0 ;
	virtual bool          load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	virtual bool          isLoaded()  = 0;
	virtual bool          unload() = 0;
	virtual unsigned long memUsage() = 0;
};

class _XEVOL_BASE_API_  IRenderState : public IRenderObject
{
    IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderState);
public:
	IRenderState(IRenderApi* pOwner);
	virtual ~IRenderState();
    //virtual unsigned int  hash_name() = 0;
	virtual eRenderStateType state_type() = 0;
	virtual bool             load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool             load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	virtual bool             isLoaded()  = 0;
	virtual bool             unload() = 0;
	virtual unsigned long    memUsage() = 0;


};

class _XEVOL_BASE_API_  ISamplerState : public IRenderState
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(ISamplerState);
public:
	eRenderStateType state_type() {return eRenderState_Sampler ; }
	ISamplerState(IRenderApi* pOwner):IRenderState(pOwner) {};
	virtual ~ISamplerState(){};
};

class _XEVOL_BASE_API_  IBlenderState : public IRenderState
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IBlenderState);
public:
	eRenderStateType state_type() {return eRenderState_Blend ; }
	IBlenderState(IRenderApi* pOwner):IRenderState(pOwner) {};
	virtual ~IBlenderState(){};
	virtual float*   factor() = 0;
	virtual void     set_factor(xColor_4f& cl) = 0;
	virtual void     set_factor(float r , float g , float b , float a) = 0;
	virtual void     set_samplemask(unsigned int mask) = 0;
	virtual unsigned int samplemask() = 0 ;
};


class _XEVOL_BASE_API_  IRasterizerState : public IRenderState
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRasterizerState);
public:
	eRenderStateType state_type() {return eRenderState_Rasterizer ; }
	IRasterizerState(IRenderApi* pOwner):IRenderState(pOwner) {};
	virtual ~IRasterizerState(){}
};

class _XEVOL_BASE_API_  IDepthStencilState : public IRenderState
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IDepthStencilState);
public:
	eRenderStateType state_type() {return eRenderState_DepthStencil ; }
	IDepthStencilState(IRenderApi* pOwner):IRenderState(pOwner) {};
	virtual  unsigned int stencil_ref() = 0;    
	virtual  void         stencil_ref(unsigned int ref) = 0;
	virtual ~IDepthStencilState(){};
};


class _XEVOL_BASE_API_ I2DRectObject : public IRefCountObject
{
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(I2DRectObject);
	I2DRectObject(){};
	virtual ~I2DRectObject(){};
	virtual void     setUVChanel(int iUVChanel) = 0;
	virtual int      nUVChanel() = 0;
	virtual void     setColor(xColor_4f& cl) = 0;
	virtual void     setRect(float destRect[4]) = 0;
	virtual void     setTexture(int iStage , IBaseTexture* pTexture) = 0;
	virtual void     setTexture(int iStage , IBaseTexture* pTexture , float srcRect[4]) = 0;
	virtual void     commit() = 0;
	virtual bool     draw() = 0;
};

END_NAMESPACE_XEVOL3D
#endif
