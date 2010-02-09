// This is the main DLL file.

#include "stdafx.h"
#include <vcclr.h>
#include "Evol3D.Net.h"
#include "xnRenderApi.h"
#include "xnRenderer.h"
#pragma comment(lib,"user32.lib")
namespace xEvol3DNet 
{

		xEvol3DEnvWindowMsgHandler::xEvol3DEnvWindowMsgHandler(HWND hWnd , HWND hParentWnd)
		{
			m_hParentWnd = hParentWnd;
			m_hRenderWnd = hWnd;
		}
		bool xEvol3DEnvWindowMsgHandler::onMessage(xWindowMsg& msg)
		{
			if(m_hParentWnd == NULL)
				return false;
			if(msg.MsgID >= WIN_MOUSE_MESSAGE_BEGIN && msg.MsgID <= WIN_MOUSE_MESSAGE_END )
				PostMessage(m_hParentWnd , msg.sSysMsg.nMsg , msg.sSysMsg.wParam , msg.sSysMsg.lParam);
			if(msg.MsgID >= WIN_KEYBOARD_MESSAGE_BEGIN && msg.MsgID <= WIN_KEYBOARD_MESSAGE_END )
				PostMessage(m_hParentWnd , msg.sSysMsg.nMsg , msg.sSysMsg.wParam , msg.sSysMsg.lParam);
			return true;
		}

	Boolean xEvol3DEnv::ResizeRenderWindow(int w , int h)
	{
		x2DRect _rect;
		m_pWindow->getWindRect(_rect);
		if(_rect.w == w && _rect.h == h)
			return true;
        m_pWindow->setWindowSize(w , h);
		return true;
	}
	Boolean xEvol3DEnv::Init(String^ _xmlCfg , int hWnd , int hParentWnd , String^ _plugDir)
	{
		pin_ptr<const wchar_t> xmlCfg   = PtrToStringChars(_xmlCfg);
		pin_ptr<const wchar_t> plugDir  = PtrToStringChars(_plugDir);

		m_cfgDocument = new xXmlDocument;
		wchar_t appName[512] = {0};
		::GetModuleFileName(NULL, appName , 512);
		xOperationSys::singleton()->init(appName);
		XEVOL_LOG_SET_LEVEL( eXR_LOG_LEVEL(eXL_DEBUG_TIPS - 1) );
		m_cfgDocument->load( _XEVOL_ABSPATH_(xmlCfg) );
		xPluginMgr::singleton()->loadPluginDir( _XEVOL_ABSPATH_( plugDir) );
		xR_CreatePlatform(L"Win32");
		m_platform = IPlatform::singleton();
		XEVOL_LOG_REDIR( _XEVOL_ABSPATH_(L".\\XEvolLogger.txt") );

		WIN_CREATE_STRUCT cs;
		wcsncpy(cs.m_Caption , L"我的D3D10演示程序",64);
		wcsncpy(cs.m_WinName , L"HelloWindow",64);

		cs.m_Style = WIN_STYLE_COMMON;
		cs.m_WinRect.x = 0;
		cs.m_WinRect.y = 0;
		cs.m_WinRect.w = 1024;
		cs.m_WinRect.h = 768;
		cs.m_BackGrdColor = xColor_4ub( (unsigned char)0,0,0,255);

		//IRenderApi* pRenderApi = NULL;
		if(hWnd == NULL)
		{

			
			XEvol3D::xXmlNode node;
			if(hParentWnd) node.setValue(L"parentWindow" , hParentWnd);
			XEvol3D::IRenderApi * pRenderApi = NULL;
			RECT rect;
			GetClientRect((HWND)hParentWnd , &rect);
			cs.m_WinRect.w = rect.right  - rect.left;
			cs.m_WinRect.h = rect.bottom - rect.top;
			m_pWindow = m_platform->createRenderWindow(&pRenderApi , cs , L"Direct3D10" , &node);
			m_pWindow->show(true);
			m_pRenderApi = pRenderApi;
			m_hParentWnd = (HWND)hParentWnd;
			m_hRenderWnd = (HWND)m_pWindow->getHandle();
			m_MsgHandler = new xEvol3DEnvWindowMsgHandler(m_hRenderWnd , m_hParentWnd);
			m_pWindow->addMsgListener(m_MsgHandler);
		}
		else
		{
			XEvol3D::xXmlNode node;
			node.setValue(L"externWindow" , hWnd);
			XEvol3D::IRenderApi * pRenderApi = NULL;
			m_pWindow = m_platform->createRenderWindow(&pRenderApi , cs , L"Direct3D10" , &node);
			m_pRenderApi = pRenderApi;
			m_hRenderWnd = (HWND)hWnd;
			m_hParentWnd = NULL;
		}
		
		if(m_pRenderApi )
		{
			m_pRenderApi->init( m_cfgDocument->root() ) ;
		}
		return m_pRenderApi != NULL;
	}

	xRenderApi^ xEvol3DEnv::createRenderApi()
	{
		return gcnew xRenderApi(m_pRenderApi);
	}

	xHelperDrawable^  xEvol3DEnv::createHelperDrawable(String^ objName)
	{
          return gcnew xHelperDrawable( (IDrawableHelper *)xPluginMgr::singleton()->createObject( ToPtr(objName),NULL) );
	}
}


