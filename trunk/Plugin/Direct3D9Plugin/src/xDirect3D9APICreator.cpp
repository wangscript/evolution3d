#include "stdafx.h"
#include "xDIRECT3D9APICreator.h"
#include <Application/xWindow.h>
#pragma warning (disable:4311)
#pragma warning (disable:4312)

#pragma comment(lib,"d3d9.lib")

#pragma comment(lib,"dxguid.lib")
#ifdef _USE_SUMMER2004_DXSDK_
#pragma comment(lib,"d3dx9.lib")
#else
#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif
#endif


BEGIN_NAMESPACE_XEVOL3D

IRenderApi*    xD3D9RenderApiCreator::createRenderApi(int _hWnd , unsigned long hDevice , int w , int h , const xXmlNode* pXmlNode)
{
	HWND hWnd             = (HWND) _hWnd ;
	IDirect3DDevice9* pDevice = (IDirect3DDevice9*)hDevice;
	xD3D9RenderApi * pRenderer = new xD3D9RenderApi(pDevice , hWnd , w , h);
	return pRenderer;
}

IRenderApi*    xD3D9RenderApiCreator::createRenderApi(IWindow* pWindow , unsigned long hDevice , int w , int h  , const xXmlNode* pXmlNode )
{
	return createRenderApi(pWindow->getHandle() , hDevice , w , h , pXmlNode);
}

END_NAMESPACE_XEVOL3D
