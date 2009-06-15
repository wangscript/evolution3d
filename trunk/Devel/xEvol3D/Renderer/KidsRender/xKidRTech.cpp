#include "xKidRenderer.h"
#include "xKidRTech.h"
#include "../xRendererQueue.h"
#include "../xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xKidBasicRenderTech , IRenderTech);

xKidBasicRenderTech::xKidBasicRenderTech(IRenderApi* pRendereApi):IRenderTech(pRendereApi)
{
	m_pRenderer = NULL;
};

xKidBasicRenderTech::~xKidBasicRenderTech()
{

};
void xKidBasicRenderTech::setRenderer(IBaseRenderer* pRenderer) 
{
	m_pRenderer = dynamic_cast<xKidRenderer*>(pRenderer);
}

IBaseRenderer* xKidBasicRenderTech::getRenderer()
{
	return m_pRenderer;
}

bool xKidBasicRenderTech::commitRenderer(unsigned int passedTime)
{
    size_t nQueue = m_pRenderer->nRenderQueue();
	for(size_t i = 0 ; i < nQueue ; i ++)
	{
		xRendererQueue* pQueue = m_pRenderer->RenderQueueByIndex((int)i);
		size_t nPass = pQueue->size();
		for(size_t idxObj = 0 ; idxObj < nPass ; idxObj ++ )
		{
			xRenderPass* pass =  (*pQueue)[idxObj];
			pass->render(passedTime);
		}
	}
	return true;
}


IMPL_OBJECT_FACTORY_MGR(xKidBasicRenderTech,  xKidRTechFactory , xKidRTechFactoryMgr);

class xBasicRTechFactory : public  xKidRTechFactory                                                                                         
{                                                                                                               
public:	
	xBasicRTechFactory()
	{
		xKidRTechFactoryMgr::singleton()->registe(this);

	}

	~xBasicRTechFactory()
	{
		xKidRTechFactoryMgr::singleton()->unregiste(this);
		
	}
	const wchar_t*            name()
	{
		return L"Basic";
	}

	xKidBasicRenderTech*      createInstance(IRenderApi* param1 , xKidRenderer* param2)
	{
		xKidBasicRenderTech* pTech = new xKidBasicRenderTech(param1);
		pTech->setRenderer(param2);
		return pTech;
	}

	int                       type()
	{
		return 0;
	}

	const wchar_t *           desc()
	{
		return L"Basic render tech for Kid Renderer";
	}
};																						                        
static xBasicRTechFactory g_BasicRenderTechKid;

END_NAMESPACE_XEVOL3D