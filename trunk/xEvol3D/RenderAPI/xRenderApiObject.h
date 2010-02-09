#ifndef _XEVOL_RENDER_API_OBJECT_H_
#define _XEVOL_RENDER_API_OBJECT_H_
#include "xRenderApiDef.h"
#include "xRenderApiFwDecl.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_   IRenderApiObject : public IBaseObject
{
protected:
	IRenderApi* m_pRenderApi;
	IRenderApiObject(IRenderApi* pOwner);
	virtual ~IRenderApiObject();
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderApiObject);
	virtual void*       handle(){return 0 ; }
    virtual bool        lockRenderApiObject()  {return true ; }
    virtual bool        unlockRenderApiObject(){return true ;  }
	virtual IRenderApi* renderApi(){ return m_pRenderApi ; }
	virtual void        setRenderApi(IRenderApi* pRenderApi){  m_pRenderApi = pRenderApi ; }
};

class _XEVOL_BASE_API_  IRenderApiResource : public IRenderApiObject
{
protected:
	IRenderApiResource(IRenderApi* pOwner);
	virtual ~IRenderApiResource();
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderApiResource);
	IRenderApi* renderApi(){return m_pRenderApi ; }
	virtual void*         handle(){return 0 ; }
	virtual eResourceType res_type() = 0 ;
	virtual bool          load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	virtual bool          isLoaded()  = 0;
	virtual bool          unload() = 0;
	virtual unsigned long memUsage() = 0;
};

class _XEVOL_BASE_API_ IRenderOtherRes : public IRenderApiResource
{
protected:
    IRenderOtherRes(IRenderApi* pOwner):IRenderApiResource(pOwner){};
    virtual ~IRenderOtherRes(){};
public:
    virtual eResourceType res_type() { return RESOURCE_OTHER ; }
};

class _XEVOL_BASE_API_  IRenderApiState : public IRenderApiObject
{
protected:
	IRenderApiState(IRenderApi* pOwner);
	virtual ~IRenderApiState();
public:
   IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderApiState);
    //virtual unsigned int  hash_name() = 0;
	virtual eRenderStateType state_type() = 0;
	virtual bool             load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool             load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	virtual bool             isLoaded()  = 0;
	virtual bool             unload() = 0;
	virtual unsigned long    memUsage() = 0;


};

class _XEVOL_BASE_API_  ISamplerState : public IRenderApiState
{
protected:
	ISamplerState(IRenderApi* pOwner):IRenderApiState(pOwner) {};
	virtual ~ISamplerState(){};
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(ISamplerState);
	eRenderStateType state_type() {return eRenderState_Sampler ; }

};

class _XEVOL_BASE_API_  IBlenderState : public IRenderApiState
{
protected:
	IBlenderState(IRenderApi* pOwner):IRenderApiState(pOwner) {};
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


class _XEVOL_BASE_API_  IRasterizerState : public IRenderApiState
{
protected:
	IRasterizerState(IRenderApi* pOwner):IRenderApiState(pOwner) {};
	virtual ~IRasterizerState(){}
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRasterizerState);
	eRenderStateType state_type() {return eRenderState_Rasterizer ; }

};

class _XEVOL_BASE_API_  IDepthStencilState : public IRenderApiState
{
protected:
	virtual ~IDepthStencilState(){};
	IDepthStencilState(IRenderApi* pOwner):IRenderApiState(pOwner) {};
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
	virtual void     setUVChanel(int iUVChanel) = 0;
	virtual int      nUVChanel() = 0;
	virtual void     setColor(const xColor_4f& cl) = 0;
	virtual void     setRect(float destRect[4]) = 0;
	virtual void     setTexture(int iStage , IBaseTexture* pTexture) = 0;
	virtual void     setTexture(int iStage , IBaseTexture* pTexture , float srcRect[4]) = 0;
	virtual void     commit() = 0;
	virtual bool     draw() = 0;
};

END_NAMESPACE_XEVOL3D
#endif
