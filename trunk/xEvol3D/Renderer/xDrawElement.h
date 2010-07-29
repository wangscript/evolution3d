#ifndef __XEVOL3D_DRAWELEMENT_H__
#define __XEVOL3D_DRAWELEMENT_H__
#include "../RenderAPI/xRenderAPI.h"
#include "xRenderObject.h"
#include "../XMathLib/xGeomLib.h"
#include "../XMathLib/xvolume.h"
#include "xMaterial.h"
#include "../BaseLib/xObjectFactory.h"
BEGIN_NAMESPACE_XEVOL3D

//-------------------
//场景中的节点
// ISceneNode
//      ----- IRenderEffect : 记录该节点用啥Effect对象来绘制
//      ----- vector<xRenderPassArg> : 记录每一个Pass的参数。
//      ----- load() 首先load Effect. 然后load每一个xRenderPassArg. 如果xRenderPassArg里没有参数，则使用默认的。

//非场景的节点，完全demo化自绘
//Object
//        ----IRenderEffect
//        ----create: 获取每个RenderPass,然后为每个RenderPass设置参数。如果不设置，则使用默认的参数。
//-------------------

//所有的可绘制的原色都从IDrawElement派生。
struct sDrawElementInfo_t
{
      char m_queueIndex;
      int  m_queueName;
};

class _XEVOL_BASE_API_  IDrawElement : public IRenderObject
{
protected:
	IDrawElement(IBaseRenderer* pRenderer);
	virtual ~IDrawElement(){};   
public:
      DECL_BASE_OBJECT_DLLSAFE(IDrawElement);
      //几何定位和裁剪用。
      virtual void                     center(xMathLib::xvec3&   _center) = 0;
      virtual void                     aabb(xGeomLib::xaabb&     _aabb) = 0;
      virtual void                     shpere(xGeomLib::xshpere& _shpere) = 0;
      virtual IRenderApi*              renderApi();
      virtual bool                     render(unsigned long passedTime) = 0;
      virtual bool                     update(unsigned long passedTime) = 0 ;
	  virtual bool                     isVisible(xGeomLib::xCamera* pCamera ,  const xMathLib::xmat4& _mat);
	  //--------------------------------------------------------
};



class _XEVOL_BASE_API_  xSerializeDrawElement : public IDrawElement
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xSerializeDrawElement);
	xSerializeDrawElement(IBaseRenderer* pRenderer, int arg);
	virtual ~xSerializeDrawElement();
	virtual bool   load(xXmlNode* pXml) = 0;
	virtual bool   save(xXmlNode* pNode) = 0;
};

class _XEVOL_BASE_API_ IRenderPassArg : public IBaseObject
{
public:
	DECL_BASE_OBJECT_DLLSAFE(IRenderPassArg); 
	IRenderPassArg();
	virtual ~IRenderPassArg();
	virtual bool                     setMatrix(const xMathLib::xmat4& _mat);
	virtual const xMathLib::xmat4&   getMatrix();
protected:
	xMathLib::xmat4                  m_trans;
};

//真正可以投到RenderQueue里的东西
class _XEVOL_BASE_API_ xRenderPass  : public  IRenderObject
{
public:
   DECL_BASE_OBJECT_DLLSAFE(xRenderPass);   
   xRenderPass(IBaseRenderer* pRenderer);
   virtual ~xRenderPass();
public:
   XEVOL_DEFINE_PROPERTY_P(xMaterial             , Material);
   XEVOL_DEFINE_PROPERTY_P(IDrawElement          , Drawable);
   XEVOL_DEFINE_PROPERTY_P(IGpuProgramParamTable , ConstTable);
   XEVOL_DEFINE_PROPERTY_P(IRenderPassArg        , RenderPassArg);
public:
	virtual bool  render(unsigned long passedTime);
};



class  _XEVOL_BASE_API_ IDrawElementCreator;
class  _XEVOL_BASE_API_ xDrawElementFactoryMgr;
class  _XEVOL_BASE_API_ xRenderPassCreator;
class  _XEVOL_BASE_API_ xRenderPassFactoryMgr;
DECL_OBJECT_FACTORY_AND_MGR(IDrawElement ,IDrawElementCreator , xDrawElementFactoryMgr , IBaseRenderer* , int);
DECL_OBJECT_FACTORY_AND_MGR(xRenderPass  ,xRenderPassCreator  , xRenderPassFactoryMgr  , IBaseRenderer* , int);
END_NAMESPACE_XEVOL3D
#endif
