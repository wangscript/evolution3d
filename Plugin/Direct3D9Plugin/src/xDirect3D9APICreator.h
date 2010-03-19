#ifndef __XEVOL_DIRECT3D9_API_CREATOR_H__
#define __XEVOL_DIRECT3D9_API_CREATOR_H__
#include <RenderApi/xRenderApiManager.h>
#include "xDIRECT3D9API.h"
BEGIN_NAMESPACE_XEVOL3D

class xD3D9RenderApiCreator : public IRenderApiCreator
{
public:
	xD3D9RenderApiCreator()
	{
		xRendererAPIManager::registeAPICreator(this);
	}

	~xD3D9RenderApiCreator()
	{
		xRendererAPIManager::unregisteAPICreator(this);
	}

	const wchar_t* renderName() {return RENDER_NAME ; }
	//创建一个RenderApi.
	//     Win32Platform::createWindow ->hDevice = NULL- Direc3D::init
	IRenderApi*    createRenderApi(IWindow* pWindow , unsigned long hDevice , int w  = 0, int h = 0  , const xXmlNode* pXmlNode = NULL);
	IRenderApi*    createRenderApi(int _hWnd        , unsigned long hDevice , int w  = 0, int h = 0  , const xXmlNode* pXmlNode = NULL);
};

END_NAMESPACE_XEVOL3D

#endif

