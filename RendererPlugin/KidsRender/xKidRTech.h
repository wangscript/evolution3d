#ifndef _KID_RENDERER_RENDER_TECH_
#define _KID_RENDERER_RENDER_TECH_
#include "Renderer/xRenderer.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_RENDERER_API_ xKidRenderer;
class _XEVOL_RENDERER_API_ xKidBasicRenderTech : public IRenderTech
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

DECL_OBJECT_FACTORY_AND_MGR_DLL(xKidBasicRenderTech,  xKidRTechFactory , xKidRTechFactoryMgr , IRenderApi* , xKidRenderer* , _XEVOL_RENDERER_API_);

/*
class  xKidRTechFactory                                                                                        
{                                                                                                             
public:																					                       
	virtual const wchar_t*           name() = 0;													                   
	virtual xKidBasicRenderTech*     createInstance(ArgType1 param1 , ArgType2 param2) = 0;					           
	virtual int                      type()= 0;                                                                        
	virtual const wchar_t *          desc() = 0;                                                                       
};

class  _XEVOL_BASE_API_ I_OBJECT_Factory_Manager : public TFactoryManager<xKidRTechFactory , xKidBasicRenderTech , IRenderApi* , xKidRenderer*>	    
{																						  
	static I_OBJECT_Factory_Manager* g_singleton; 
public:	 
	static I_OBJECT_Factory_Manager* singleton();  
	static void                      deleteSinglton(); 
};		
*/

END_NAMESPACE_XEVOL3D
#endif
