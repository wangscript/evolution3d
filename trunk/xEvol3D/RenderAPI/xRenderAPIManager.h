#ifndef __XEVOL_RENDER_API_H__
#define __XEVOL_RENDER_API_H__
#include "../BaseLib/xEvol3DBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_  IWindow;
class _XEVOL_BASE_API_  IRenderApi;
class _XEVOL_BASE_API_  xXmlNode;

class _XEVOL_BASE_API_  IRenderApiCreator
{
public:
	virtual const wchar_t* renderName() = 0;
	//创建一个RenderApi.
	//可能的情况: Win32Platform::createWindow ->hDevice = HWND- OpenGL::init
	//            Win32Platform::createWindow ->hDevice = HWND- Direc3D::init
	//            SDLOGLPlatform::createOpenGLWindow  ->hDevice = GLContext- OpenGL::init(GLContext);
	//            GLXPlatform::createOpenGLWindow     ->hDevice = GLContext- OpenGL::init(GLContext);
	virtual IRenderApi*    createRenderApi(int hWnd         , unsigned long hDevice , int w  = 0, int h = 0  , const xXmlNode* pXmlNode = NULL) = 0;
	virtual IRenderApi*    createRenderApi(IWindow* pWindow , unsigned long hDevice , int w  = 0, int h = 0  , const xXmlNode* pXmlNode = NULL) = 0;
};


class _XEVOL_BASE_API_  xRendererAPIManager
{
public:
	xRendererAPIManager();
	~xRendererAPIManager();
	static bool                    registeAPICreator(IRenderApiCreator* pRenderApiCreator);
	static bool                    unregisteAPICreator(IRenderApiCreator* pRenderApiCreator);
	static IRenderApiCreator*     findAPICreator(const wchar_t* rendererName);
	static int                     numOfAPICreator();
	static IRenderApiCreator*     findAPICreator(int index);
};


END_NAMESPACE_XEVOL3D

#endif
