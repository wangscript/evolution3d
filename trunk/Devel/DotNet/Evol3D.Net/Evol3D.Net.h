// Evol3D.Net.h

#pragma once

#include "BaseLib/xCfgParser.h"
#include "BaseLib/xLogger.h"
#include "OperationSys/xOperationSys.h"
#include "Application/xPluginMgr.h"
#include "Application/xPlatform.h"
#include "Application/xApplication.h"
#include "RenderApi/xRenderApi.h"
#include "RenderAPI/xBaseShader.h"
#include "RenderAPI/xShaderNode.h"
#include "RenderAPI/xShaderManager.h"
#include <tchar.h>
#include "XMathLib/xMathlib.h"
#include "Image/xILImage.h"
#include "RenderAPI/xBaseTexture.h"
#include "RenderAPI/xRenderAPI.h"
#include "xDotNetUlti.h"

#include "BaseLib/xStringHash.h"
using namespace XEvol3D;
using namespace System;

namespace xEvol3D 
{
	ref class xRenderApi;
    ref class xHelperDrawable;
	public ref class   xColorF
	{
	internal:
		xColor_4f*  m_cl;
	public:
		xColorF(float r , float g , float b , float a ) {m_cl = new xColor_4f(r , g , b , a ) ; };
		xColorF() {m_cl = new xColor_4f(1.0f , 1.0f , 1.0f , 1.0f ) ; };
		~xColorF() { delete m_cl ; }

		PS_ReadWrite(float , r   , m_cl->r , ,) ;
		PS_ReadWrite(float , g   , m_cl->g , ,) ;
		PS_ReadWrite(float , b   , m_cl->b , ,) ;
		PS_ReadWrite(float , a   , m_cl->a , ,) ;
		PF_ReadOnly (float , lum , m_cl , abs , ,) ;
	};

	class xEvol3DEnvWindowMsgHandler: public IMsgListener
	{
		HWND            m_hParentWnd;
		HWND            m_hRenderWnd;
	public:
		xEvol3DEnvWindowMsgHandler(HWND hWnd , HWND hParentWnd);
		bool onMessage(xWindowMsg& msg);
	};
	public ref class xEvol3DEnv
	{
	internal:
		xCfgDocument*   m_cfgDocument;
		IPlatform*      m_platform;
		IRenderApi*     m_pRenderApi;
        HWND            m_hParentWnd;
		HWND            m_hRenderWnd;
		IWindow*        m_pWindow;
		xEvol3DEnvWindowMsgHandler* m_MsgHandler;
	public:
		Boolean             ResizeRenderWindow(int w , int h);
		Boolean             Init(String^ _xmlCfg , int hWnd , int hParentWnd , String^ _plugDir);
		xRenderApi^         createRenderApi();
		unsigned int        stringHash(String^ hashValue) {return xStringHash(ToPtr(hashValue) ) ; };
		String^             absPath(String^ _Path) { return gcnew String( _ABSPATH( ToPtr(_Path) ) ) ; }
		xHelperDrawable^    createHelperDrawable(String^ objName);
		// TODO: Add your methods for this class here.
	};
}
