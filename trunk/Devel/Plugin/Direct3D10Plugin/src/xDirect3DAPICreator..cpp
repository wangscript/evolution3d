#include "xDirect3DAPICreator.h"
#include <Application/xWindow.h>
#include <d3d10.h>
BEGIN_NAMESPACE_XREAL3D
IXRenderAPI*    xD3D10RenderApiCreator::createRenderAPI(IWindow* pWindow , unsigned long hDevice , int w , int h  )
{
	HWND hWnd             = (HWND) pWindow->getHandle() ;
	ID3D10Device* pDevice = (ID3D10Device*)hDevice;
	xD3D10RenderApi * pRenderer = new xD3D10RenderApi();
	if(pRenderer->create(pDevice , hWnd , w , h ))
		return pRenderer;
	delete pRenderer;
	return NULL;
}

END_NAMESPACE_XREAL3D
