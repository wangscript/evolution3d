#ifndef _KID_RENDERER_RENDER_TECH_
#define _KID_RENDERER_RENDER_TECH_
#include "../xRenderer.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xKidRenderer;
class _XEVOL_BASE_API_ xKidBasicRenderTech : public IRenderTech
{
	xKidRenderer*          m_pRenderer;
public:
	DECL_BASE_OBJECT_DLLSAFE(xKidBasicRenderTech);
public:
	xKidBasicRenderTech(IRenderApi* pRendereApi);
	virtual ~xKidBasicRenderTech();
	virtual void           setRenderer(IBaseRenderer* pRenderer);
	virtual IBaseRenderer* getRenderer();
	virtual bool           commitRenderer(unsigned int passedTime);
	virtual bool           drawPass(int idQueue, xRenderPass* pObject);
};

DECL_OBJECT_FACTORY_AND_MGR(xKidBasicRenderTech,  xKidRTechFactory , xKidRTechFactoryMgr , IRenderApi* , xKidRenderer*);

END_NAMESPACE_XEVOL3D
#endif
