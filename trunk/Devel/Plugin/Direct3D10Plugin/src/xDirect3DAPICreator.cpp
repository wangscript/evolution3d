#include "xDirect3DAPICreator.h"
#include <Application/xWindow.h>
#include <d3d10.h>
#pragma warning (disable:4311)
#pragma warning (disable:4312)
BEGIN_NAMESPACE_XEVOL3D

IRenderApi*    xD3D10RenderApiCreator::createRenderApi(int _hWnd , unsigned long hDevice , int w , int h)
{
	HWND hWnd             = (HWND) _hWnd ;
	ID3D10Device* pDevice = (ID3D10Device*)hDevice;
	xD3D10RenderApi * pRenderer = new xD3D10RenderApi();
	if(pRenderer->create(pDevice , hWnd , w , h ))
		return pRenderer;
	delete pRenderer;
	return NULL;
}

IRenderApi*    xD3D10RenderApiCreator::createRenderApi(IWindow* pWindow , unsigned long hDevice , int w , int h  )
{
	return createRenderApi(pWindow->getHandle() , hDevice , w , h);
}

END_NAMESPACE_XEVOL3D
