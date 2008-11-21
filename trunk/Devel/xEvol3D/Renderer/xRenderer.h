#ifndef __XEVOL3D_RENDERER_H__
#define __XEVOL3D_RENDERER_H__
#include "../RenderAPI/xRenderAPI.h"
/*********************************************

xRenderer的设计：

渲染的材料：
  类似Mesh一类的东西。 抽象成xDrawableObject

渲染的单位：
   xDrawElement. xDrawableObject只是一个物体。
   并不是一个知道如何渲染自己的物体。它包含
   一个或者多个xDrawElement.

渲染的参数：
   xSurfaceMaterial: 表面材质
   xGPUPrograme: 包含Shader本身以及shader参数.
   xTextureLayer: 一层或者多层的texture.
   
渲染的工具：
  RenderScript . RenderScript根据自身的内容，
  把xDrawableObject 解析成一个个xDrawElement.
  xDrawableObject有义务给xDrawElement设置一些
  参数。如果有需要RenderScript会改变这些参数，
  或者补充那些不完整的参数。

渲染的场所:
  Renderer: 
  Renderer保存一个渲染队列的列表。并维护着如何
  渲染这些队列的RendererScript.



Renderer 跟 RenderScript是逻辑代码。通过派生定义
不同行为的Render方式。

xSurfaceMaterial xTextureLayer xDrawableObject
这些仅仅是提供数据而已。

*********************************************/

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xRendererQueue;
class _XEVOL_BASE_API_ IDrawElement;
class _XEVOL_BASE_API_ xMaterial;

class _XEVOL_BASE_API_ IRenderer
{
public:
	virtual bool              loadConfig(const wchar_t* xmlName , const int8* xmlData , int len) = 0;
	virtual int               nRenderQueue() = 0;
	virtual xRendererQueue*   RenderQueueByName(int QueueName) = 0;
	virtual xRendererQueue*   RenderQueueByName(const wchar_t* QueueName) = 0;
	virtual xRendererQueue*   RenderQueue(int iQueue) = 0;
	virtual bool              drawObject(IDrawElement* pObject , bool drawImm = false) = 0;
    virtual bool              setRenderTech(const wchar_t* techName) = 0;
	virtual const char*       getRenderTech() = 0;
	virtual bool              begin() = 0;
	virtual bool              end() = 0;
	virtual xMaterial*        createMaterial(const wchar_t* matName) = 0 ;
	virtual void              deleteMaterial(xMaterial* pMaterial) = 0 ;
	virtual xMaterial*        loadMaterial(const wchar_t* matFileName) = 0;
};

class _XEVOL_BASE_API_ IRendererCreator
{
public:
	virtual const wchar_t* name() const = 0;
	virtual IRenderer*     createRenderer() const = 0;
};

class _XEVOL_BASE_API_ xRendererManager
{
	xRendererManager();
	IRenderer*     m_pRenderer ;
public:
	static xRendererManager* singleton();
    IRenderer*               getRenderer();
	bool                     registeRenderer(IRendererCreator* pRendererCreator);
	bool                     unregisteRenderer(IRendererCreator* pRendererCreator);
	IRenderer*               createRenderer(const wchar_t* name);
};

END_NAMESPACE_XEVOL3D

#endif