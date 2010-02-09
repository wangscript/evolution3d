#include "stdafx.h"
#include "xDirect3D10APICreator.h"
#include <Application/xWindow.h>
#include <d3d10.h>
#pragma warning (disable:4311)
#pragma warning (disable:4312)
BEGIN_NAMESPACE_XEVOL3D

IRenderApi*    xD3D10RenderApiCreator::createRenderApi(int _hWnd , unsigned long hDevice , int w , int h , const xXmlNode* pXmlNode)
{
	HWND hWnd             = (HWND) _hWnd ;
	ID3D10Device* pDevice = (ID3D10Device*)hDevice;
	xD3D10RenderApi * pRenderer = new xD3D10RenderApi(pDevice , hWnd , w , h);
	return pRenderer;
}

IRenderApi*    xD3D10RenderApiCreator::createRenderApi(IWindow* pWindow , unsigned long hDevice , int w , int h , const xXmlNode* pXmlNode)
{
	return createRenderApi(pWindow->getHandle() , hDevice , w , h , pXmlNode);
}

END_NAMESPACE_XEVOL3D
