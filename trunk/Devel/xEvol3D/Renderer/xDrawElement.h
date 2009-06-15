#ifndef __XEVOL3D_DRAWELEMENT_H__
#define __XEVOL3D_DRAWELEMENT_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../XMathLib/XGeomLib.h"
#include "../XMathLib/XGVolume.h"
#include "xMaterial.h"
BEGIN_NAMESPACE_XEVOL3D

//-------------------
//场景中的节点
// ISceneNode
//      ----- xRenderEffect : 记录该节点用啥Effect对象来绘制
//      ----- vector<xRenderPassArg> : 记录每一个Pass的参数。
//      ----- load() 首先load Effect. 然后load每一个xRenderPassArg. 如果xRenderPassArg里没有参数，则使用默认的。

//非场景的节点，完全demo化自绘
//Object
//        ----xRenderEffect
//        ----create: 获取每个RenderPass,然后为每个RenderPass设置参数。如果不设置，则使用默认的参数。
//-------------------

//所有的可绘制的原色都从IDrawableObject派生。
struct sDrawElementInfo_t
{
      char m_queueIndex;
      int  m_queueName;
};

class _XEVOL_BASE_API_  IDrawableObject : public IRenderObject
{
protected:
	IDrawableObject(IRenderApi* pRenderApi);
	virtual ~IDrawableObject(){};   
public:
      DECL_BASE_OBJECT_DLLSAFE(IDrawableObject);
      //几何定位和裁剪用。
      virtual void           center(XMathLib::xvec3&   _center) = 0;
      virtual void           aabb(xGeomLib::xaabb&     _aabb) = 0;
      virtual void           shpere(xGeomLib::xshpere& _shpere) = 0;
      virtual IRenderApi*    renderApi() = 0;
      virtual bool           render(unsigned long passedTime) = 0;
      virtual bool           update(unsigned long passedTime) = 0 ;

	  //--------------------------------------------------------
};


class _XEVOL_BASE_API_ IDrawElement : public IDrawableObject
{
public:
      DECL_BASE_OBJECT_DLLSAFE(IDrawElement);
      IDrawElement(IRenderApi* pRenderApi);
      virtual ~IDrawElement() = 0{};    
};



//真正可以投到RenderQueue里的东西
class _XEVOL_BASE_API_ xRenderPass  : IRenderObject
{
public:
   DECL_BASE_OBJECT_DLLSAFE(IRenderObject);   
   xRenderPass(IRenderApi* pRenderApi);
   virtual ~xRenderPass();
public:
   XEVOL_DEFINE_PROPERTY_P(xMaterial             , Material);
   XEVOL_DEFINE_PROPERTY_P(IDrawElement          , Drawable);
   XEVOL_DEFINE_PROPERTY_P(IGpuProgramParamTable , ConstTable);

public:
	virtual bool  render(unsigned long passedTime);
};

END_NAMESPACE_XEVOL3D
#endif
