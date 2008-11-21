#ifndef __XEVOL_DIRECT3D10_API_CREATOR_H__
#define __XEVOL_DIRECT3D10_API_CREATOR_H__
#include <RenderApi/xRenderApiManager.h>
#include "xDirect3D10API.h"
BEGIN_NAMESPACE_XEVOL3D

class xD3D10RenderApiCreator : public IRenderApiCreator
{
public:
	xD3D10RenderApiCreator()
	{
		xRendererAPIManager::registeAPICreator(this);
	}

	~xD3D10RenderApiCreator()
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

