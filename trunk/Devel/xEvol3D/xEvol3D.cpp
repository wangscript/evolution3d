#include "xEvol3D.h"

#include <BaseLib/xXmlDocument.h>
#include <BaseLib/xLogger.h>
#include <OperationSys/xOperationSys.h>
#include <Application/xPluginMgr.h>
#include <Application/xPlatform.h>
#include <Application/xApplication.h>
#include <RenderApi/xRenderApi.h>
#include <RenderAPI/xBaseShader.h>
#include <RenderAPI/xShaderNode.h>
#include <RenderAPI/xShaderManager.h>
#include <RenderAPI/xRenderView.h>
#include <tchar.h>
#include <XMathLib/xMathlib.h>
#include <Image/xILImage.h>
#include <RenderAPI/xBaseTexture.h>
#include "xModel/xBaseModel.h"
#include "Renderer/xDrawableHelper.h"
#include "Image/xFreeImage.h"
BEGIN_NAMESPACE_XEVOL3D

xEvol3DEngine::xEvol3DEngine()
{
	m_cfgDocument = new xXmlDocument;
	m_platform = NULL;
	m_pWindow = NULL;
	m_pRenderApi = NULL;
}
xEvol3DEngine::~xEvol3DEngine()
{
	XSAFE_DELETE(m_cfgDocument);
}

IRenderApi* xEvol3DEngine::renderApi()
{
	return m_pRenderApi;
}

bool   xEvol3DEngine::exit()
{
	m_cfgDocument->setXMLStyle(true);
	m_cfgDocument->save( _ABSPATH(L"system.xml"));
	m_pRenderApi->ReleaseObject();
	m_platform->close();
	m_platform=NULL;
	return true;
}

bool   xEvol3DEngine::init(IApplication* pApp , void* hWnd , const wchar_t* cfgFile, const wchar_t* projectName , const wchar_t* argv , int argc)
{
	//XEVOL_LOG_SET_LEVEL( eXR_LOG_LEVEL(eXL_DEBUG_HIGH - 1) );
	m_cfgDocument->load( _ABSPATH(L"system.xml"));
	xPluginMgr::singleton()->loadPluginDir( _ABSPATH(L"./") );
	xR_CreatePlatform(L"Win32");
	m_platform = IPlatform::singleton();
	XEVOL_LOG_REDIR( _ABSPATH(L".\\XEvolLogger.txt") );
	xXmlNode* pRootNode    = m_cfgDocument->root();

	xXmlNode* pPrjNode     = pRootNode->findNode(projectName);
	if(pPrjNode == NULL) pPrjNode = pRootNode->findNode(L"engine");

	const wchar_t* renderApiName = pPrjNode->value(L"renderapi");
	
    if(hWnd == NULL)
	{
		WIN_CREATE_STRUCT cs;
		const wchar_t* _captName = pPrjNode?pPrjNode->value(L"caption"):pRootNode->value(L"caption");
		if( _captName == NULL)
		{
			_captName = L"我的D3D10演示程序";
		}
		wcsncpy(cs.m_Caption , _captName    ,64);
		wcsncpy(cs.m_WinName , L"xEvolution3DWindow",64);

		cs.m_Style = WIN_STYLE_COMMON;
		cs.m_WinRect.x = 0;
		cs.m_WinRect.y = 0;
		cs.m_WinRect.w = 1024;
		cs.m_WinRect.h = 768;
		cs.m_BackGrdColor = xColor_4ub( (unsigned char)0,0,0,255);
		m_pWindow = m_platform->createRenderWindow(&m_pRenderApi , cs , renderApiName ,NULL);
		m_pWindow->show(true);
	}
	else
	{
		m_pRenderApi = m_platform->createRenderApi( (int)hWnd , L"xEvolution3DWindow" ,NULL);
	}

	if(m_pRenderApi == NULL) 
		m_pRenderApi = NULL;
	m_platform->setApplication(pApp); 
	m_pRenderApi->init( pRootNode ) ;
	return true;
}
bool xEvol3DEngine::messageLoop()
{
	m_platform->messageLoop();
	return true;
}
END_NAMESPACE_XEVOL3D