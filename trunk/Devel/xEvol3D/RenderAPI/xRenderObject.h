#ifndef _XEVOL_RENDER_API_OBJECT_H_
#define _XEVOL_RENDER_API_OBJECT_H_
#include "xRenderApiDef.h"
#include "xRenderApiFwDecl.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_   IRenderObject : public IBaseObject
{
protected:
	IRenderApi* m_pRenderApi;
	IRenderObject(IRenderApi* pOwner);
	virtual ~IRenderObject();
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderObject);
	virtual void* handle(){return 0 ; }
    virtual bool lockRenderObject()  {return true ; }
    virtual bool unlockRenderObject(){return true ;  }
};

class _XEVOL_BASE_API_  IRenderResource : public IRenderObject
{
protected:
	IRenderApi* m_pRenderApi;
	IRenderResource(IRenderApi* pOwner);
	virtual ~IRenderResource();
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderResource);
	IRenderApi* renderApi(){return m_pRenderApi ; }
	virtual void*         handle(){return 0 ; }
	virtual eResourceType res_type() = 0 ;
	virtual bool          load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	virtual bool          isLoaded()  = 0;
	virtual bool          unload() = 0;
	virtual unsigned long memUsage() = 0;
};

class _XEVOL_BASE_API_ IRenderOtherRes : public IRenderResource
{
protected:
    IRenderOtherRes(IRenderApi* pOwner):IRenderResource(pOwner){};
    virtual ~IRenderOtherRes(){};
public:
    virtual eResourceType res_type() { return RESOURCE_OTHER ; }
};

class _XEVOL_BASE_API_  IRenderState : public IRenderObject
{
protected:
	IRenderState(IRenderApi* pOwner);
	virtual ~IRenderState();
public:
   IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderState);
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
protected:
	ISamplerState(IRenderApi* pOwner):IRenderState(pOwner) {};
	virtual ~ISamplerState(){};
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(ISamplerState);
	eRenderStateType state_type() {return eRenderState_Sampler ; }

};

class _XEVOL_BASE_API_  IBlenderState : public IRenderState
{
protected:
	IBlenderState(IRenderApi* pOwner):IRenderState(pOwner) {};
	virtual ~IBlenderState(){};
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IBlenderState);
	eRenderStateType state_type() {return eRenderState_Blend ; }
	virtual float*   factor() = 0;
	virtual void     set_factor(xColor_4f& cl) = 0;
	virtual void     set_factor(float r , float g , float b , float a) = 0;
	virtual void     set_samplemask(unsigned int mask) = 0;
	virtual unsigned int samplemask() = 0 ;
};


class _XEVOL_BASE_API_  IRasterizerState : public IRenderState
{
protected:
	IRasterizerState(IRenderApi* pOwner):IRenderState(pOwner) {};
	virtual ~IRasterizerState(){}
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRasterizerState);
	eRenderStateType state_type() {return eRenderState_Rasterizer ; }

};

class _XEVOL_BASE_API_  IDepthStencilState : public IRenderState
{
protected:
	virtual ~IDepthStencilState(){};
	IDepthStencilState(IRenderApi* pOwner):IRenderState(pOwner) {};
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IDepthStencilState);
	eRenderStateType state_type() {return eRenderState_DepthStencil ; }
	virtual  unsigned int stencil_ref() = 0;    
	virtual  void         stencil_ref(unsigned int ref) = 0;
	
};


class _XEVOL_BASE_API_ I2DRectObject : public IRefCountObject
{
protected:
	I2DRectObject(){};
	virtual ~I2DRectObject(){};
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(I2DRectObject);
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
