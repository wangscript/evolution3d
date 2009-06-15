#include "xKidRenderer.h"

BEGIN_NAMESPACE_XEVOL3D
class xKidRenderCreator: public IRendererCreator
{
public:
	xKidRenderCreator( ) 
	{
		xRendererManager::singleton()->registeRenderer(this) ;
	}

	~xKidRenderCreator( ) 
	{
		xRendererManager::singleton()->unregisteRenderer(this) ;
	}

	const wchar_t*   name()
	{
		return KID_RENDERER;
	};	

	IBaseRenderer*   createInstance(IRenderApi* param1 , int param2)
	{
		return new xKidRenderer(param1) ;
	}	

	const wchar_t *  desc()
	{
		return L"Kid Renderer is a simple Renderer" ; 
	}   
};


xKidRenderCreator g_KidRenderer;

END_NAMESPACE_XEVOL3D

