#include "xStdPch.h"
#include "xEvol3D.h"
#include "BaseLib/xResPackageMgrTool.h"
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

#include "xModel/xBaseModelMgr.h"
#include "xModel/xBaseModel.h"
#include "Renderer/xDrawableHelper.h"
#include "xScene/xSceneGraph.h"
#include "xModel/xBaseModelMgr.h"
#include "RenderAPI/xTextureManager.h"

#ifdef _WIN32
#ifndef _XEVOL_BUILD_STATIC_
HANDLE Global_hModuleHandle = NULL;
BOOL WINAPI DllMain(HANDLE  hDllHandle,   DWORD   dwReason,    LPVOID  lpreserved     )
{
        if (dwReason == DLL_PROCESS_ATTACH)
        {
        /*
         * The /GS security cookie must be initialized before any exception
         * handling targetting the current image is registered.  No function
         * using exception handling can be called in the current image until
         * after __security_init_cookie has been called.
         */
            Global_hModuleHandle = hDllHandle;
        }
        return TRUE;
}
#endif
#endif

BEGIN_NAMESPACE_XEVOL3D

xEvol3DEngine::xEvol3DEngine()
:m_ResPathMgr(L"defualt")
{
	m_cfgDocument = new xXmlDocument;
	m_platform = NULL;
	m_pWindow = NULL;
	m_pRenderApi = NULL;
	m_pModelMgr  = NULL;
	m_pTexMgr    = NULL;

}

bool xEvol3DEngine::loadCamera(xGeomLib::xCamera* pCamera , xXmlNode* pCameraNode)
{
    if(pCameraNode == NULL)
		return false;

	pCamera->setFOV( pCameraNode->float_value(L"fov") );
	pCamera->setNearPlan( pCameraNode->float_value(L"near") );
	pCamera->setFarPlan( pCameraNode->float_value(L"far") );


	xXmlNode* pEyePos = pCameraNode->findNode(L"pos");
	pCamera->m_Eye.m_EyePos.x = pEyePos->float_value(L"x");
	pCamera->m_Eye.m_EyePos.y = pEyePos->float_value(L"y");
	pCamera->m_Eye.m_EyePos.z = pEyePos->float_value(L"z");

	xXmlNode* pEyeTarget = pCameraNode->findNode(L"target");
	pCamera->m_Eye.m_EyeTarget.x = pEyeTarget->float_value(L"x");
	pCamera->m_Eye.m_EyeTarget.y = pEyeTarget->float_value(L"y");
	pCamera->m_Eye.m_EyeTarget.z = pEyeTarget->float_value(L"z");


	xXmlNode* pEyeUp = pCameraNode->findNode(L"up");
	pCamera->m_Eye.m_Up.x = pEyeUp->float_value(L"x");
	pCamera->m_Eye.m_Up.y = pEyeUp->float_value(L"y");
	pCamera->m_Eye.m_Up.z = pEyeUp->float_value(L"z");

	return true;
}

bool xEvol3DEngine::saveCamera(xGeomLib::xCamera* pCamera , xXmlNode* pCameraNode)
{
	if(pCameraNode == NULL)
		return false;

	pCameraNode->setValue(L"fov"  ,  pCamera->getFOV()      ) ;
	pCameraNode->setValue(L"near" ,  pCamera->getNearPlan() ) ;
	pCameraNode->setValue(L"far"  ,  pCamera->getFarPlan()  ) ;

	pCamera->setFOV( pCameraNode->float_value(L"fov") );
	pCamera->setNearPlan( pCameraNode->float_value(L"near") );
	pCamera->setFarPlan( pCameraNode->float_value(L"far") );

	xXmlNode* pEyePos = pCameraNode->findNode(L"pos"); 
	if(pEyePos == NULL) pEyePos = pCameraNode->insertNode(L"pos"); 

	pEyePos->setValue(L"x" , pCamera->m_Eye.m_EyePos.x);
	pEyePos->setValue(L"y" , pCamera->m_Eye.m_EyePos.y);
	pEyePos->setValue(L"z" , pCamera->m_Eye.m_EyePos.z);

	xXmlNode* pEyeTarget = pCameraNode->findNode(L"target");
	if(pEyeTarget == NULL) pEyeTarget = pCameraNode->insertNode(L"target"); 

	pEyeTarget->setValue(L"x" , pCamera->m_Eye.m_EyeTarget.x);
	pEyeTarget->setValue(L"y" , pCamera->m_Eye.m_EyeTarget.y);
	pEyeTarget->setValue(L"z" , pCamera->m_Eye.m_EyeTarget.z);


	xXmlNode* pEyeUp = pCameraNode->findNode(L"up");
	if(pEyeUp == NULL) pEyeUp = pCameraNode->insertNode(L"up"); 

	pEyeUp->setValue(L"x" , pCamera->m_Eye.m_Up.x);
	pEyeUp->setValue(L"y" , pCamera->m_Eye.m_Up.y);
	pEyeUp->setValue(L"z" , pCamera->m_Eye.m_Up.z);
	return true;

}

xEvol3DEngine::~xEvol3DEngine()
{
	XSAFE_DELETE(m_cfgDocument);
}

xResPathManager*  xEvol3DEngine::getResPathMgr()
{
	return &m_ResPathMgr;
}

xBaseModelMgr*    xEvol3DEngine::modelMgr()
{
	return m_pModelMgr;
}
xBaseTextureMgr*  xEvol3DEngine::textureManager()
{
	return m_pTexMgr;
}

xXmlNode* xEvol3DEngine::sysConfig()
{
	return m_cfgDocument->root();
}

xXmlNode* xEvol3DEngine::sysCfgNode(const wchar_t* sector , bool bCreated)
{
	xXmlNode* pNode = m_cfgDocument->root();
	if(pNode == NULL)
	{
		pNode = m_cfgDocument->insertNode(L"root");
	}
	if(sector == NULL)
		return pNode;
	pNode =  m_cfgDocument->root()->findNode(sector);
	if(pNode == NULL && bCreated)
	{
		pNode = m_cfgDocument->root()->insertNode(sector);
	}
	return pNode;

}

int xEvol3DEngine::sysValue(const wchar_t* sector , const wchar_t* _value, int defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return defValue;
	}
	return pValue->int_value();
}

const wchar_t* xEvol3DEngine::sysValue(const wchar_t* sector , const wchar_t* _value, const wchar_t* defValue)
{
     xXmlNode* pNode = sysCfgNode(sector , true);

	 xXmlValue* pValue = pNode->findValue(_value);
	 if(pValue == NULL ) 
	 {
		 pValue = pNode->insertValue(_value);
		 pValue->setValue(defValue);
		 return defValue;
	 }
	 return pValue->value();
}

float xEvol3DEngine::sysValue(const wchar_t* sector , const wchar_t* _value, float defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return defValue;
	}
	return pValue->float_value();
}

bool xEvol3DEngine::sysValue(const wchar_t* sector , const wchar_t* _value, bool defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return defValue;
	}
	return pValue->bool_value();
}

IRenderApi* xEvol3DEngine::renderApi()
{
	return m_pRenderApi;
}

bool   xEvol3DEngine::exit()
{
	m_cfgDocument->setXMLStyle(true);
	m_cfgDocument->save( m_cfgFile.c_str() );


	if(m_pModelMgr)
	{
		m_pModelMgr->unload();
		m_pModelMgr->clear();
		m_pModelMgr->KillObject();
        m_pModelMgr = NULL;
	}

	if(m_pTexMgr)
	{
		m_pTexMgr->unload();
		m_pTexMgr->clear();
		m_pRenderApi->deleteTextureManager(m_pTexMgr);
		m_pTexMgr = NULL;
	}

	m_ResPathMgr.clearAllSearchPath();
	m_pRenderApi->uninit();
	m_pRenderApi->ReleaseObject();
	m_platform->close();
	m_platform=NULL;
	return true;
}

//ISceneGraph*   xEvol3DEngine::createScene(const wchar_t* _sceneFile , IBaseRenderer* pRenderer)
//{
//	return XEvol_CreateScene(_sceneFile , pRenderer);
//}

bool   xEvol3DEngine::initCfg(const wchar_t* cfgFile)
{
	if(cfgFile == NULL)
	{
		m_cfgFile =  _RES_ABSPATH_(L"system.xml");
	}
	else
	{
		m_cfgFile = cfgFile;
	}
	//XEVOL_LOG_SET_LEVEL( eXR_LOG_LEVEL(eXL_DEBUG_HIGH - 1) );
	return m_cfgDocument->load( m_cfgFile.c_str() );
}
bool   xEvol3DEngine::initPlatform(IApplication* pApp , const wchar_t* cfgFile, const wchar_t* pluginDir)
{
	if(cfgFile != NULL)
	{
		m_cfgFile =  cfgFile;
		m_cfgDocument->load( m_cfgFile.c_str() );
	}
	else if(m_cfgDocument->root() == NULL)
	{
		m_cfgFile =  _RES_ABSPATH_(L"system.xml");
		m_cfgDocument->load( m_cfgFile.c_str() );
	}
	//XEVOL_LOG_SET_LEVEL( eXR_LOG_LEVEL(eXL_DEBUG_HIGH - 1) );

    if(pluginDir != NULL)
    {
        xPluginMgr::singleton()->loadPluginDir( pluginDir );
    }
	xR_CreatePlatform(L"Win32");
	m_platform = IPlatform::singleton();
	XEVOL_LOG_REDIR( _XEVOL_ABSPATH_(L".\\XEvolLogger.txt") );
	m_platform->setApplication(pApp); 
	return m_platform != NULL;
}

void xEvol3DEngine::onDeviceReset(bool bNewDevice , eResetAction _Action)
{
    if(_Action == IRenderApiDeviceLostListener::eReset_Begin)
    {
        if(m_pModelMgr )
        {
            m_pModelMgr->unload();
        }

    }
}

bool xEvol3DEngine::initRenderer( const wchar_t* projectName, void* hWnd , xXmlNode* pCreateParams) 
{
	xXmlNode* pRootNode    = m_cfgDocument->root();

	xXmlNode* pPrjNode     = pRootNode->findNode(projectName);
	if(pPrjNode == NULL) pPrjNode = pRootNode->findNode(L"engine");

	const wchar_t* renderApiName = pPrjNode->value(L"renderapi");
	
	if( renderApiName == NULL)
	{
		renderApiName = L"Auto";
	}

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
		m_pWindow = m_platform->createRenderWindow(&m_pRenderApi , cs , renderApiName , pCreateParams);
		m_pWindow->show(true);
	}
	else
	{
		m_pRenderApi = m_platform->createRenderApi( (int)hWnd , renderApiName , pCreateParams);
	}

	if(m_pRenderApi == NULL) 
		m_pRenderApi = NULL;

    m_pRenderApi->AddDeviceLostListener(this);
	m_pRenderApi->init( pRootNode ) ;

	//现在来初始化所有的ResourceManager;
	m_pTexMgr = m_pRenderApi->createTextureManager(L"DefTexMgr", true);

	pPrjNode     = pRootNode->findNode(projectName);
	if(pPrjNode == NULL)
	{
		pPrjNode = pRootNode->findNode(L"ResourceManager");
	}

	if(pPrjNode)
	{

        xXmlNode* pTexMgrNode = pPrjNode->findNode(L"TextureManager");
		if(pTexMgrNode)
		{
			XEvol_AddResManagerPath(*m_pTexMgr , pTexMgrNode);
		}
		xXmlNode* pOtherResNode = pPrjNode->findNode(L"ResoursPath");
		if(pOtherResNode)
		{
			XEvol_AddResManagerPath(m_ResPathMgr , pOtherResNode);
		}

		xXmlNode::XmlNodes pathManagerNodes;
		pPrjNode->findNode(L"PathManagers" , pathManagerNodes);
		size_t nPathMgr = pathManagerNodes.size() ;

		for(size_t i = 0 ; i < nPathMgr ; i ++)
		{
			xXmlNode* pPathMgrNode = pathManagerNodes[i];
			xResPathManager* pPathMgr = new xResPathManager(pPathMgrNode->value(L"name"));
			XEvol_AddResManagerPath(*pPathMgr , pPathMgrNode);
			m_ResPathManagers[pPathMgrNode->value(L"name")] = pPathMgr;
		}

	}
	m_pModelMgr= xBaseModelMgr::createInstance(m_pRenderApi , m_pTexMgr , L"ModelMgr");
	m_pModelMgr->setPathMgr( &m_ResPathMgr );
	return true;
}
xResPathManager*    xEvol3DEngine::getResPathMgr(const wchar_t* pName)
{
	if(pName == NULL)
		return NULL;

	if(std::ds_wstring(pName) == L"default")
		return &m_ResPathMgr;

	mapResPathManagers::iterator pos = m_ResPathManagers.find(pName);
	if(pos == m_ResPathManagers.end() )
		return NULL;
	return pos->second;
}
bool   xEvol3DEngine::init(IApplication* pApp , void* hWnd , const wchar_t* cfgFile, const wchar_t* projectName , const wchar_t* argv , int argc , xXmlNode* pRenderParams)
{
    if(initPlatform(pApp , cfgFile , NULL) == false )
		return false;	
	return initRenderer(projectName, hWnd , pRenderParams);
}
bool xEvol3DEngine::messageLoop()
{
	m_platform->messageLoop();
	return true;
}
END_NAMESPACE_XEVOL3D