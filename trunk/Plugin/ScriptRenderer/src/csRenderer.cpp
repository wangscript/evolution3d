#include "../include/csRenderer.h"
#include "../include/csMaterial.h"
#include "BaseLib/xLogger.h"


BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(csRenderer , IBaseRenderer);

csRenderer::csRenderer(IRenderApi* pRenderApi)
{
    m_pRenderApi = pRenderApi;
}

csRenderer::~csRenderer()
{

}

bool csRenderer::loadConfig(const wchar_t* xmlName , const int8* xmlData , int len)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

int csRenderer::nRenderQueue()
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return 0;
}

xRendererQueue* csRenderer::RenderQueueByName(int QueueName)
{
    int _id = QueueName;
    xRendererQueueMaps::iterator pos = m_MapQueues.find(_id);
    if(pos == m_MapQueues.end() ) return NULL;
    return pos->second;
    return NULL;
}

xRendererQueue* csRenderer::RenderQueueByName(const wchar_t* QueueName)
{
    int hsName = xStringHash(QueueName);
    return RenderQueueByName(hsName);
}

xRendererQueue* csRenderer::RenderQueue(int iQueue)
{
    return m_vQueues[iQueue];
}

bool csRenderer::drawPass(xRenderPass* pObject , bool drawImm )
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

bool csRenderer::setRenderTech(const wchar_t* techName)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

const wchar_t* csRenderer::getRenderTech()
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return NULL;
}

bool csRenderer::begin()
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

bool csRenderer::end()
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

xRenderPass* csRenderer::createPass(const wchar_t* passName)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return NULL;
}

xMaterial* csRenderer::createMaterial(const wchar_t* matName)
{
    xBasicMatCreatorMgr* pMatCreatorMgr = xBasicMatCreatorMgr::singleton();
    if(pMatCreatorMgr == NULL)
        return NULL;
    xMaterial* pMat = pMatCreatorMgr->createInstance(matName , m_pRenderApi);
    if(pMat)
    {
        return pMat;
    }

    pMat = new csMaterial(m_pRenderApi);
    if(pMat->load(matName , NULL , 0) == false)
    {
        pMat->KillObject();
        pMat = NULL;
    }
    return pMat;
}

IRenderEffect* csRenderer::createRenderEffect(const wchar_t* effectName)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return NULL;
}


//===²ÄÖÊ´´½¨Æ÷==========================
IMPL_CREATOR_MANAGER(xBasicMatCreatorMgr , csMaterialCreator , csMaterial , IRenderApi);
IMPL_CREATOR_MANAGER(csRenderTechCreatorMgr   , csRenderTechCreator   , csRenderTech   , IRenderApi);
IMPL_CREATOR_MANAGER(csRenderPassCreatorMgr   , csRenderPassCreator   , csRenderPass   , IRenderApi);
IMPL_CREATOR_MANAGER(csRenderEffectCreatorMgr , csRenderEffectCreator , csRenderEffect , IRenderApi);

END_NAMESPACE_XEVOL3D
