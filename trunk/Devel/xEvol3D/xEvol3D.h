#ifndef _XEVOLUTION3D_H_
#define _XEVOLUTION3D_H_
#include "./BaseLib/xEvol3DBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ IPlatform;
class _XEVOL_BASE_API_ IWindow;
class _XEVOL_BASE_API_ IRenderApi;
class _XEVOL_BASE_API_ xXmlDocument;
class _XEVOL_BASE_API_ IApplication;
class _XEVOL_BASE_API_ xEvol3DEngine
{
protected:
	xXmlDocument*    m_cfgDocument;
	IPlatform*       m_platform;
	IWindow*         m_pWindow ;
	IRenderApi*      m_pRenderApi;
public:
	xEvol3DEngine();
	~xEvol3DEngine();
	IRenderApi*   renderApi();
	bool          exit();
    bool          init(IApplication* pApp , void* hWnd  , const wchar_t* cfgFile, const wchar_t* projectName ,  const wchar_t* argv , int argc);
	bool          messageLoop();
};
END_NAMESPACE_XEVOL3D
#endif
