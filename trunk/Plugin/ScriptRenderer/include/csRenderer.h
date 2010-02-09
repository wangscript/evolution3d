#ifndef _BASICRENDERER_H_
#define _BASICRENDERER_H_
#include "Application/xPluginMgr.h"
#include "Renderer/xRenderer.h"
#include "csRendererInc.h"
BEGIN_NAMESPACE_XEVOL3D
#define _CSCRIPTRENDER_NAME_ L"csRenderer"

class _XEVOL_BASRD_API_ csMaterial;
class _XEVOL_BASRD_API_ csRenderTech ; 
class _XEVOL_BASRD_API_ csRenderPass  ;
class _XEVOL_BASRD_API_ csRenderEffect;
class _XEVOL_BASRD_API_ csRenderer : public IBaseRenderer
{
     typedef std::map<int , xRendererQueue*>    xRendererQueueMaps;
     typedef std::vector<xRendererQueue*>       xRendererQueueVector;
    
public:
    DECL_BASE_OBJECT_DLLSAFE(csRenderer);
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
    virtual IRenderEffect*    createRenderEffect(const wchar_t* effectName);
public:
    csRenderer(IRenderApi* pRenderApi);
    ~csRenderer();	
protected:
    IRenderApi*              m_pRenderApi;
    xRendererQueueMaps       m_MapQueues;
    xRendererQueueVector     m_vQueues;
};

//²ÄÖÊ´´½¨Æ÷
class _XEVOL_BASRD_API_ csMaterialCreator
{
public:
    virtual const wchar_t*   name() const = 0;
    virtual csMaterial*  createInstance(IRenderApi* pRenderApi) const = 0;
};

class _XEVOL_BASRD_API_ csRenderTechCreator
{
public:
    virtual const wchar_t*   name() const = 0;
    virtual csRenderTech*  createInstance(IRenderApi* pRenderApi) const = 0;
};

class _XEVOL_BASRD_API_ csRenderPassCreator
{
public:
    virtual const wchar_t*   name() const = 0;
    virtual csRenderPass*  createInstance(IRenderApi* pRenderApi) const = 0;
};

class _XEVOL_BASRD_API_ csRenderEffectCreator
{
public:
    virtual const wchar_t*       name() const = 0;
    virtual csRenderEffect*  createInstance(IRenderApi* pRenderApi) const = 0;
};

class _XEVOL_BASRD_API_ xBasicMatCreatorMgr;
class _XEVOL_BASRD_API_ csRenderTechCreatorMgr ; 
class _XEVOL_BASRD_API_ csRenderPassCreatorMgr  ;
class _XEVOL_BASRD_API_ csRenderEffectCreatorMgr;
DECL_CREATOR_MANAGER(xBasicMatCreatorMgr          , _XEVOL_BASRD_API_ , csMaterialCreator     , csMaterial     , IRenderApi);
DECL_CREATOR_MANAGER(csRenderTechCreatorMgr   , _XEVOL_BASRD_API_ , csRenderTechCreator   , csRenderTech   , IRenderApi);
DECL_CREATOR_MANAGER(csRenderPassCreatorMgr   , _XEVOL_BASRD_API_ , csRenderPassCreator   , csRenderPass   , IRenderApi);
DECL_CREATOR_MANAGER(csRenderEffectCreatorMgr , _XEVOL_BASRD_API_ , csRenderEffectCreator , csRenderEffect , IRenderApi);

END_NAMESPACE_XEVOL3D

#endif