#ifndef _BASICRENDERER_H_
#define _BASICRENDERER_H_
#include "Application/xPluginMgr.h"
#include "Renderer/xRenderer.h"
#include "BasicRendereInc.h"
BEGIN_NAMESPACE_XEVOL3D
#define _BASICRENDER_NAME_ L"BasicRenderer"

class _XEVOL_BASRD_API_ xBasicMaterial;
class _XEVOL_BASRD_API_ xBasicRenderTech ; 
class _XEVOL_BASRD_API_ xBasicRenderPass  ;
class _XEVOL_BASRD_API_ xBasicRenderEffect;
class _XEVOL_BASRD_API_ xBasicRenderer : public IBaseRenderer
{
     typedef std::map<int , xRendererQueue*>    xRendererQueueMaps;
     typedef std::vector<xRendererQueue*>       xRendererQueueVector;
    
public:
    DECL_BASE_OBJECT_DLLSAFE(xBasicRenderer);
public:
    virtual bool              loadConfig(const wchar_t* xmlName , const int8* xmlData , int len);
    virtual int               nRenderQueue();
    virtual xRendererQueue*   RenderQueueByName(int QueueName);
    virtual xRendererQueue*   RenderQueueByName(const wchar_t* QueueName);
    virtual xRendererQueue*   RenderQueue(int iQueue);
    virtual bool              drawPass(xRenderPass* pObject , bool drawImm = false);
    virtual bool              setRenderTech(const wchar_t* techName);
    virtual const wchar_t*    getRenderTech();
    virtual bool              begin();
    virtual bool              end();
    virtual xRenderPass*      createPass(const wchar_t* passName);
    virtual xMaterial*        createMaterial(const wchar_t* matName);
    virtual xRenderEffect*    createRenderEffect(const wchar_t* effectName);
public:
    xBasicRenderer(IRenderApi* pRenderApi);
    ~xBasicRenderer();	
protected:
    IRenderApi*              m_pRenderApi;
    xRendererQueueMaps       m_MapQueues;
    xRendererQueueVector     m_vQueues;
};

//²ÄÖÊ´´½¨Æ÷
class _XEVOL_BASRD_API_ xBasicMaterialCreator
{
public:
    virtual const wchar_t*   name() const = 0;
    virtual xBasicMaterial*  createInstance(IRenderApi* pRenderApi) const = 0;
};

class _XEVOL_BASRD_API_ xBasicRenderTechCreator
{
public:
    virtual const wchar_t*   name() const = 0;
    virtual xBasicRenderTech*  createInstance(IRenderApi* pRenderApi) const = 0;
};

class _XEVOL_BASRD_API_ xBasicRenderPassCreator
{
public:
    virtual const wchar_t*   name() const = 0;
    virtual xBasicRenderPass*  createInstance(IRenderApi* pRenderApi) const = 0;
};

class _XEVOL_BASRD_API_ xBasicRenderEffectCreator
{
public:
    virtual const wchar_t*       name() const = 0;
    virtual xBasicRenderEffect*  createInstance(IRenderApi* pRenderApi) const = 0;
};

class _XEVOL_BASRD_API_ xBasicMatCreatorMgr;
class _XEVOL_BASRD_API_ xBasicRenderTechCreatorMgr ; 
class _XEVOL_BASRD_API_ xBasicRenderPassCreatorMgr  ;
class _XEVOL_BASRD_API_ xBasicRenderEffectCreatorMgr;
DECL_CREATOR_MANAGER(xBasicMatCreatorMgr          , _XEVOL_BASRD_API_ , xBasicMaterialCreator     , xBasicMaterial     , IRenderApi);
DECL_CREATOR_MANAGER(xBasicRenderTechCreatorMgr   , _XEVOL_BASRD_API_ , xBasicRenderTechCreator   , xBasicRenderTech   , IRenderApi);
DECL_CREATOR_MANAGER(xBasicRenderPassCreatorMgr   , _XEVOL_BASRD_API_ , xBasicRenderPassCreator   , xBasicRenderPass   , IRenderApi);
DECL_CREATOR_MANAGER(xBasicRenderEffectCreatorMgr , _XEVOL_BASRD_API_ , xBasicRenderEffectCreator , xBasicRenderEffect , IRenderApi);

END_NAMESPACE_XEVOL3D

#endif