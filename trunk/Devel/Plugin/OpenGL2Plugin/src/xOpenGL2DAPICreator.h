#ifndef __XEVOL_OPENGL20_API_CREATOR_H__
#define __XEVOL_OPENGL20_API_CREATOR_H__
#include <RenderApi/xRenderApiManager.h>
#include "xOpenGL2API.h"
BEGIN_NAMESPACE_XEVOL3D

class xGL2RenderApiCreator : public IRenderApiCreator
{
public:
	xGL2RenderApiCreator()
	{
		xRendererAPIManager::registeAPICreator(this);
	}

	~xGL2RenderApiCreator()
	{
		xRendererAPIManager::unregisteAPICreator(this);
	}

	const wchar_t* renderName() {return RENDER_NAME ; }
	//创建一个RenderApi.
	//     Win32Platform::createWindow ->hDevice = NULL- Direc3D::init
	IRenderApi*    createRenderApi(IWindow* pWindow , unsigned long hDevice , int w  = 0, int h = 0 );
	IRenderApi*    createRenderApi(int _hWnd        , unsigned long hDevice , int w  = 0, int h = 0 );
};

END_NAMESPACE_XEVOL3D

#endif

