#include "../include/BasicRenderer.h"
#include "../include/BasicMaterial.h"
#include "BaseLib/xLogger.h"


BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xBasicRenderer , IBaseRenderer);

xBasicRenderer::xBasicRenderer(IRenderApi* pRenderApi)
{
    m_pRenderApi = pRenderApi;
}

xBasicRenderer::~xBasicRenderer()
{

}

bool xBasicRenderer::loadConfig(const wchar_t* xmlName , const int8* xmlData , int len)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

int xBasicRenderer::nRenderQueue()
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return 0;
}

xRendererQueue* xBasicRenderer::RenderQueueByName(int QueueName)
{
    int _id = QueueName;
    xRendererQueueMaps::iterator pos = m_MapQueues.find(_id);
    if(pos == m_MapQueues.end() ) return NULL;
    return pos->second;
    return NULL;
}

xRendererQueue* xBasicRenderer::RenderQueueByName(const wchar_t* QueueName)
{
    int hsName = xStringHash(QueueName);
    return RenderQueueByName(hsName);
}

xRendererQueue* xBasicRenderer::RenderQueue(int iQueue)
{
    return m_vQueues[iQueue];
}

bool xBasicRenderer::drawPass(xRenderPass* pObject , bool drawImm )
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

bool xBasicRenderer::setRenderTech(const wchar_t* techName)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

const wchar_t* xBasicRenderer::getRenderTech()
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return NULL;
}

bool xBasicRenderer::begin()
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

bool xBasicRenderer::end()
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

xRenderPass* xBasicRenderer::createPass(const wchar_t* passName)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return NULL;
}

xMaterial* xBasicRenderer::createMaterial(const wchar_t* matName)
{
    xBasicMatCreatorMgr* pMatCreatorMgr = xBasicMatCreatorMgr::singleton();
    if(pMatCreatorMgr == NULL)
        return NULL;
    xMaterial* pMat = pMatCreatorMgr->createInstance(matName , m_pRenderApi);
    if(pMat)
    {
        return pMat;
    }

    pMat = new xBasicMaterial(m_pRenderApi);
    if(pMat->load(matName , NULL , 0) == false)
    {
        pMat->KillObject();
        pMat = NULL;
    }
    return pMat;
}

xRenderEffect* xBasicRenderer::createRenderEffect(const wchar_t* effectName)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return NULL;
}


//===²ÄÖÊ´´½¨Æ÷==========================
IMPL_CREATOR_MANAGER(xBasicMatCreatorMgr , xBasicMaterialCreator , xBasicMaterial , IRenderApi);
IMPL_CREATOR_MANAGER(xBasicRenderTechCreatorMgr   , xBasicRenderTechCreator   , xBasicRenderTech   , IRenderApi);
IMPL_CREATOR_MANAGER(xBasicRenderPassCreatorMgr   , xBasicRenderPassCreator   , xBasicRenderPass   , IRenderApi);
IMPL_CREATOR_MANAGER(xBasicRenderEffectCreatorMgr , xBasicRenderEffectCreator , xBasicRenderEffect , IRenderApi);

END_NAMESPACE_XEVOL3D
