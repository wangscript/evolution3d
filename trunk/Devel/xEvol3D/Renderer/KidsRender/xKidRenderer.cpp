#include "xKidRenderer.h"
#include "xKidRTech.h"
#include "../xRendererQueue.h"
#include "../xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xKidRenderer , IBaseRenderer);


xKidRenderer::xKidRenderer(IRenderApi* pRenderApi) : IBaseRenderer(pRenderApi)
{
      m_pTech = new xKidBasicRenderTech(pRenderApi);
	  m_pTech->setRenderer(this);
}

xKidRenderer::~xKidRenderer()
{

}

bool xKidRenderer::loadConfig(const wchar_t* xmlName , const int8* xmlData , int len)
{
	return IBaseRenderer::loadConfig(xmlName,xmlData , len);
}

bool xKidRenderer::drawPass(int idQueue, xRenderPass* pObject)
{
     xRendererQueue* pQueue = this->RenderQueueByName(idQueue);
	 if(pQueue == NULL)
		 return false;
	 pQueue->insert(pObject);
     return true;
}

bool xKidRenderer::setRenderTech(const wchar_t* techName)
{
	xKidBasicRenderTech* pRenderTech = xKidRTechFactoryMgr::singleton()->createInstance(techName , m_pRenderApi , this);
	if(pRenderTech == NULL)
		return false;
	if(m_pTech) 
	{
		m_pTech->ReleaseObject();

	}
	m_pTech = pRenderTech;
    return true;
}

IRenderTech* xKidRenderer::getRenderTech()
{
      return m_pTech;
}     

bool xKidRenderer::begin(unsigned int passedTime)
{

	return true;
}
bool xKidRenderer::end(unsigned int passedTime)
{
	m_pTech->commitRenderer(passedTime);
	return true;
}


xRenderPass* xKidRenderer::createPass(const wchar_t* passName)
{
    return new xRenderPass(m_pRenderApi);
}

xMaterial* xKidRenderer::createMaterial(const wchar_t* matName)
{
    return NULL;
}
xRenderEffect* xKidRenderer::createRenderEffect(const wchar_t* effectName)
{
    return NULL;
}




END_NAMESPACE_XEVOL3D
