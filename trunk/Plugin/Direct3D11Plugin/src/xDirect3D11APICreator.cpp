#include "stdafx.h"
#include "xDirect3D11APICreator.h"
#include <Application/xWindow.h>
#pragma warning (disable:4311)
#pragma warning (disable:4312)

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx11d.lib")
#else
#pragma comment(lib,"d3dx11.lib")
#endif


BEGIN_NAMESPACE_XEVOL3D

IRenderApi*    xD3D11RenderApiCreator::createRenderApi(int _hWnd , unsigned long hDevice , int w , int h , const xXmlNode* pXmlNode)
{
	HWND hWnd             = (HWND) _hWnd ;
	ID3D11Device* pDevice = (ID3D11Device*)hDevice;
	xD3D11RenderApi * pRenderer = new xD3D11RenderApi(pDevice , hWnd , w , h);
	return pRenderer;
}

IRenderApi*    xD3D11RenderApiCreator::createRenderApi(IWindow* pWindow , unsigned long hDevice , int w , int h  , const xXmlNode* pXmlNode )
{
	return createRenderApi(pWindow->getHandle() , hDevice , w , h , pXmlNode);
}

END_NAMESPACE_XEVOL3D
