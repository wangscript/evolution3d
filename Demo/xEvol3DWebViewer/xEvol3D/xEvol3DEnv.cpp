// TestBase.cpp : Defines the entry point for the console application.
//
#include "../stdafx.h"
#include "xEvol3DEnv.h"
#include "RenderAPI/xVideoTexture.h"
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")
#pragma  comment(lib,"winmm.lib")

#ifdef _XEVOL_BUILD_STATIC_PLUGIN_
extern bool InitEvolHelperObjPlugin();
extern bool InitDirect3D9Plugin();
extern bool InitKidRenderCreatorPlugin();
extern bool InitWin32PlatformPlugin();
//extern bool InitDirect3D11Plugin();
#endif

BEGIN_NAMESPACE_XEVOL3D
using namespace XEvol3D;

bool   CEvolEnviroment::onResize()
{
	x2DRect _rect;
	xTextureDesc _desc;
	m_pRenderApi->onResize(0 , 0 );
	m_pRenderApi->getRenderView()->desc(_desc);
	m_p2DCamera->setPerspective2D(_desc.m_width , _desc.m_height , 60 ,true);
	m_pCamera->setAspect( float(_desc.m_width)/(_desc.m_height) );
	return true;
}
void     CEvolEnviroment::topCamera(xGeomLib::xaabb& aabox)
{
	xvec3 cen = 0.5f * (aabox.m_Max + aabox.m_Min);
	xvec3 len =        (aabox.m_Max - aabox.m_Min);

	m_pCamera->m_Eye.m_EyePos    = xvec4(cen.x         ,cen.y , cen.z  + 10 * len.z);
	m_pCamera->m_Eye.m_EyeTarget = xvec4(cen.x         ,cen.y , cen.z);
	m_pCamera->m_Eye.m_Up        = xvec4(0.0f,1.0f,0.0f);
	m_cameraStep = len.x/5.0f;
	m_pCamera->setFarPlan(len.z * 15.0f);
	m_pCamera->setNearPlan(0.1f);
}

xvec3   CEvolEnviroment::getModelCeneter(xBaseModel* pModel)
{
	xGeomLib::xaabb& aabox = pModel->boundBox();
	xvec3 cen = 0.5f * (aabox.m_Max + aabox.m_Min);
	return cen;
}

void     CEvolEnviroment::frontCamera(xGeomLib::xaabb& aabox)
{
	xvec3 cen = 0.5f * (aabox.m_Max + aabox.m_Min);
	xvec3 len =        (aabox.m_Max - aabox.m_Min);

	m_pCamera->m_Eye.m_EyePos    = xvec4(cen.x +  m_fCameraDist * len.x ,cen.y     , cen.z + 0.28f * len.z);
	m_pCamera->m_Eye.m_EyeTarget = xvec4(cen.x                          ,cen.y     , cen.z + 0.28f * len.z );
	m_pCamera->m_Eye.m_Up        = xvec4(0.0f,0.0f,1.0f);
	m_cameraStep = len.x/10.0f;
	m_pCamera->setFarPlan(len.x * 150.0f);
	m_pCamera->setNearPlan(0.1f);
}

void     CEvolEnviroment::exit()
{
    TAutoLocker<CEvolEnviroment> aLocker(this);
	if(m_pScene)
	{
		m_pScene->unload();
		m_pScene->ReleaseObject();
	}    
	m_hProgram.setNULL();
	m_hFont.setNULL();
	m_pEngine->exit();
}

xvec2i*  CEvolEnviroment::GetSelectData( xTextureDesc& _desc , int x , int y)
{
	m_pRenderApi->lock();
	m_pRenderApi->colorSelector()->setRenderObjectID(0xcdef0a74 , 0x12345678 );
	m_pRenderApi->beginSelectMode();

	m_pRenderApi->getRenderView()->desc(_desc);
	xvec2i * pRTData = new xvec2i[_desc.m_width * _desc.m_height];

	if( x < 0  || x >= _desc.m_width || y < 0  || y >= _desc.m_height )
	{
		m_pRenderApi->endSelectMode(0 , 0 , _desc.m_width, _desc.m_height , pRTData );
		m_pRenderApi->unlock();
		delete [] pRTData ;
		return NULL;
	}

	updateFrame(0 , m_selVistor);
	m_pRenderApi->endSelectMode(0 , 0 , _desc.m_width, _desc.m_height , pRTData );
	m_pRenderApi->unlock();
	return pRTData;
}

void  CEvolEnviroment::ReleaseSelectData(xvec2i* pRTData)
{
	delete [] pRTData ;
}


bool   CEvolEnviroment::onMessage(xWindowMsg& msg)
{
	if(m_xuiWinMgr == NULL || m_pRenderApi == NULL)
		return false;

	bool bRet = m_xuiWinMgr->dispatchMsg(msg);
	if(msg.MsgID == WIN_CLOSE )
	{
		m_pEngine->platform()->quit();
	}
	if(msg.MsgID == WIN_RESIZE)
	{
		onResize();
		return true;
	}
	else if(msg.MsgID == WIN_KEYUP)
	{
		if(msg.Keyboard.cKeyChar >= '1' && msg.Keyboard.cKeyChar <= '9')
		{
			//m_Model->setAction( msg.Keyboard.cKeyChar - '1' );
		}

		if(msg.Keyboard.nKey == VKEY_F9)
		{
			bool bDrawPlane = m_pEngine->sysValue(L"defualt",L"drawPlane" , false);
		}

		if(msg.Keyboard.nKey == VKEY_DELETE)
		{
			bool bAllowOpScene = m_pEngine->sysValue(L"defualt",L"sceneEditable" , true);
			if(bAllowOpScene && GetSelection()->GetCurSelNode())
			{
				ISceneNode* pNode = GetSelection()->GetCurSelNode();
				ISceneNode* pParentNode = pNode->getParent();
				if(pParentNode == NULL)
				{
					MessageBox(GetActiveWindow() , L"节点没有父节点", L"错误" , MB_OK);
				}
				else
				{
					pParentNode->removeChild(pNode,true);
					GetSelection()->setCurSelNode(NULL);
				}
			}
		}
		return false;
	}
	else if(msg.MsgID == WIN_KEYDOWN)
	{
		if(msg.Keyboard.cKeyChar == 'a' )
		{
			m_pCamera->shift(-m_cameraStep);
		}
		else if(msg.Keyboard.cKeyChar == 'd')
		{
			m_pCamera->shift(m_cameraStep);
		}
		else if(msg.Keyboard.cKeyChar == 'w')
		{
			m_pCamera->toward(m_cameraStep);
		}
		else if(msg.Keyboard.cKeyChar == 's')
		{
			m_pCamera->toward(-m_cameraStep);
		}
		else if(msg.Keyboard.nKey == VKEY_UP)
		{
			m_pCamera->pitch(2);
		}
		else if(msg.Keyboard.nKey == VKEY_DOWN)
		{
			m_pCamera->pitch(-2);
		}
		if(msg.Keyboard.nKey == VKEY_LEFT)
		{
			m_pCamera->yaw(2);
		}
		else if(msg.Keyboard.nKey == VKEY_RIGHT)
		{
			m_pCamera->yaw(-2);
		}
		else if(msg.Keyboard.nKey == VKEY_SHIFT)
		{
			m_pCamera->upDown(m_cameraStep);
		}
		else if(msg.Keyboard.nKey == VKEY_CONTROL)
		{
			m_pCamera->upDown(-m_cameraStep);
		}
		else 
		{
			return false;
		}
		return true;
	}

	static bool inRotCamera = false;
	static xvec2i lastPos ;
	if(msg.MsgID == NS_XEVOL3D::WIN_LMOUSE_DOWN)
	{
		inRotCamera = true;
		lastPos.x = msg.Mouse.x;
		lastPos.y = msg.Mouse.y;
		SetCapture((HWND)msg.sSysMsg.hWnd);
		return true;
	}

	if(msg.MsgID == NS_XEVOL3D::WIN_MOUSE_MOVE)
	{
		if(inRotCamera)
		{
			int dx = msg.Mouse.x - lastPos.x ;
			m_pCamera->circle(dx / 10.0f);
			lastPos.x = msg.Mouse.x;
			lastPos.y = msg.Mouse.y;
		}
	}

	if(msg.MsgID == NS_XEVOL3D::WIN_LMOUSE_RELEASE)
	{
		inRotCamera = false;
		::ReleaseCapture();
		return true;
	}

	m_pScenePlacementOprator->onMessage(msg);
	return false;
}

const wchar_t*     CEvolEnviroment::sysCfgfile()
{
	return m_cfgFile.c_str() ; 
}

bool     CEvolEnviroment::createScene(const wchar_t* _SceneType)
{
	xSceneInitArg arg;
	arg.m_pModelMgr   = m_pEngine->modelMgr();
	arg.m_pRenderer   = this->m_pRenderer;
	arg.m_pTextureMgr = m_pEngine->textureManager();
	arg.m_pResPathMgr = m_pEngine->getResPathMgr();
	ISceneGraph* pScene = xSceneFactoryMgr::singleton()->createInstance( _SceneType , NULL , NULL );
	if(pScene == NULL)
		return false;

	pScene->setRenderer(arg.m_pRenderer);
	pScene->setModelManager(arg.m_pModelMgr);
	pScene->setTextureManager(arg.m_pTextureMgr);
	pScene->setPathManager(arg.m_pResPathMgr);
	return  __AfterCreateScene(pScene , L"");
}

bool     CEvolEnviroment::loadScene(const wchar_t* _Scene)
{

    TAutoLocker<CEvolEnviroment> aLocker(this);
	if(_Scene != NULL)
	{
		xSceneInitArg arg;
		arg.m_pModelMgr   = m_pEngine->modelMgr();
		arg.m_pRenderer   = this->m_pRenderer;
		arg.m_pTextureMgr = m_pEngine->textureManager();
		arg.m_pResPathMgr = m_pEngine->getResPathMgr();
		ISceneGraph* pNewScene = XEvol_CreateScene(_Scene ,arg );
		return __AfterCreateScene(pNewScene, _Scene);

	}
	return false;
}

bool CEvolEnviroment::__AfterCreateScene( ISceneGraph* pNewScene, const wchar_t* _Scene ) 
{
	if(pNewScene)
	{
		if(m_pScene)
		{
			m_pScene->unload();
			m_pScene->ReleaseObject();
		}
		m_pScene = pNewScene;
		m_sceneFile = _Scene;
		m_pScene->setPathManager( m_pEngine->getResPathMgr() );
		return true;
	}
	return false;
}

bool    CEvolEnviroment::init()
{
	m_pScene = NULL;
    wchar_t appname[1024] = {0};
    xOperationSys::singleton()->getModuleFileName( (HMODULE)Global_hModuleHandle , appname , 1024);   
    
    xOperationSys::singleton()->init( appname );
    xOperationSys::singleton()->setResourcePath( _XEVOL_ABSPATH_(L"./") );
	xStringTable::singleton()->addPath(_RES_ABSPATH_(L"Language/"));
	xStringTable::singleton()->setLanguage(NULL);
    m_pLocker = xOperationSys::singleton()->createThreadLocker();

	m_pEngine = new xEvol3DEngine;
	m_cfgFile = _RES_ABSPATH_(L"Web3D/system.xml");

#ifdef _XEVOL_BUILD_STATIC_PLUGIN_
    InitDirect3D9Plugin();
    InitKidRenderCreatorPlugin();
    InitWin32PlatformPlugin();
    //InitDirect3D11Plugin();
#endif

	m_pEngine->initPlatform(this , m_cfgFile.c_str() , NULL );


    m_fCameraDist = m_pEngine->sysValue(L"engine" , L"m_fCameraDist" , 3.0f);
	return true;
}


void     CEvolEnviroment::initRenderer(HWND hRenderWindow , HWND hParentWnd)
{
	xXmlNode RenderWindowParam;
	RenderWindowParam.setValue(L"parentWindow" , (int)hParentWnd);
	RenderWindowParam.setValue(L"useParentSize" , true );

	m_pEngine->initRenderer(L"BaseDemo" , (void*)hRenderWindow , &RenderWindowParam);
	m_pRenderApi = m_pEngine->renderApi();
	//===============

	m_pRenderer = xRendererManager::singleton()->createRenderer(L"KidRenderer",m_pRenderApi);
	m_pRenderer->loadConfig( sysCfgfile() ,NULL,0);

	m_pEngine->textureManager()->addPath( _RES_ABSPATH_(L"./texture/Arch/") );
	m_pEngine->textureManager()->addPath( _RES_ABSPATH_(L"./texture/Planet/") );

	m_pRenderApi = m_pRenderApi;

    //初始化摄像机
	m_pCamera = m_pRenderApi->createCamera(L"Default");
	m_pCamera->m_Eye.m_EyePos = xvec4(0.0f,0.0f,-500.0f);
	m_pCamera->m_Eye.m_EyeTarget = xvec4(0.0f,0.0f,0.0f);
	m_pCamera->m_Eye.m_Up = xvec4(0.0f,1.0f,0.0f);


	m_pCamera->setFarPlan(1000.0f);
	m_pCamera->setNearPlan(0.1f);
	m_pCamera->setFOV(45.0f);
	m_pCamera->setProjectType(xGeomLib::xCamera::PT_PROJECT);

	m_p2DCamera = m_pRenderApi->createCamera(L"2DCamera");
	onResize();


	m_pStencilState = m_pRenderApi->createDepthStencilState(L"Overlay");

	m_hProgram = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel(0:simple.texture,simple.fakehdr)" , NULL);;

	m_hFont    = m_pRenderApi->findFont(L"small" );


	//m_Model = m_pModelMgr->add(L"einxin.xrm" , true , 0);
	xGeomLib::xaabb aabb;
	aabb.m_Max = xvec3(20,20,20);
	aabb.m_Min = xvec3(-20, -20, -20);
	frontCamera( aabb );

	m_DefRasterizer= m_pRenderApi->createRasterizerState(L"Default");
	m_pRenderApi->setRasterizerState(m_DefRasterizer);




	m_xuiWinMgr = xui::xuiWindowManager::createManager(m_pRenderApi , m_pEngine->textureManager() );
	m_xuiWinMgr->getXMLManager()->addPath(_RES_ABSPATH_(L"xui/"));
	m_InfoPanel  = XUI_CREATE_DIALOG(xuiDemoInfoPannel,m_xuiWinMgr);



	//设置渲染器的渲染Tech.
	m_pRenderer->setRenderTech(L"Basic");

	m_selVistor     = new xSceneColorSelectVisitor(m_pRenderer , m_pCamera);
	m_renderVisitor = new xSceneRenderVisitor(m_pRenderer , m_pCamera);

	//初始化操作器
	m_Selection.init(this);

	m_pScenePlacementOprator = new xObjPlacement( & this->m_Selection );
	m_pScenePlacementOprator->init(this);
	m_pScenePlacementOprator->SetMoveOperator();


	//初始化灯光
	ILightingEnv* pLightEnv = m_pRenderApi->findLightingState(L"xLightState");
	pLightEnv->enableLight(2);
	xLightDesc* pLight= pLightEnv->lightDesc(0);
	pLight->m_position  = xMathLib::xvec4(1000.0f , 1000.0f , 1000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 25.0f); 
	pLight->m_Ambient   = xMathLib::xvec4(0.1f , 0.1f , 0.1f , 1.0f);

	//第二个灯光
	pLight= pLightEnv->lightDesc(1);
	pLight->m_position  = xMathLib::xvec4(1000.0f , -1000.0f , -500.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.1f , 0.1f , 0.1f , 1.0f);

	pLightEnv->applyChange();

	return ;
}

void     CEvolEnviroment::run()
{
	this->setApplicationState(IApplication::AS_RUN);
	m_pEngine->messageLoop();
}

void     CEvolEnviroment::updateFrame(long passedTime)
{
	return updateFrame(passedTime , m_renderVisitor);
}


void     CEvolEnviroment::updateFrame(long passedTime , ISceneVisitor* pVisitor)
{
	static float angle = 0;
	static int   nFrame = 0;
	static int   tPassed = 0;
	tPassed += passedTime;
	nFrame ++;
	angle += passedTime/1000.0f * 35.0f;
	xSleep(5);
	m_pRenderApi->identityMatrix(MATRIXMODE_World);
	m_pRenderApi->applyCamera(m_pCamera);
	if(m_pRenderApi->renderMode() == eRenderMode::eRenderMode_Select )
		m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.0f,0.0f));
	else 
		m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.3f,1.0f));
	m_pRenderApi->beginScene();
	xMathLib::xmat4 mat;
	//xMathLib::XM_RotateY(mat,angle);

	//绘制模型
	m_pRenderApi->colorSelector()->setRenderObjectID(1,0);
	m_pRenderApi->identityMatrix(MATRIXMODE_World);

	this->m_pRenderer->begin(passedTime);

	//恢复Camera环境
	m_pRenderApi->applyCamera(m_pCamera);

	//m_pModelEffect->draw( m_pModelElement );

	xSceneRenderVisitor rv(m_pRenderer , m_pCamera);
	xSceneUpdateVisitor uv;
	uv.setFrameTime(passedTime);
	if(m_pScene) 
	{
		m_pScene->visit(&uv);
		m_pScene->visit(pVisitor);
	}
	this->m_pRenderer->end(passedTime);


	bool bDrawPlane = m_pEngine->sysValue(L"defualt",L"drawPlane" , false);
	if(bDrawPlane)
	{
		//Plan
		m_pRenderApi->identityMatrix(MATRIXMODE_World);
		xMathLib::XM_Scale(mat,500.0f,500.0f,500.0f);
		m_pRenderApi->multiMatrix(mat.data, MATRIXMODE_World);
		m_pRenderApi->colorSelector()->setRenderObjectID(101,0);
	}

	m_pRenderApi->applyCamera(m_p2DCamera);
	m_pRenderApi->setBlendState( m_pRenderApi->createBlendState(L"AlphaBlend") );
	//===================
	m_pRenderApi->identityMatrix(MATRIXMODE_World);
	m_pRenderApi->setDepthStencilState(m_pStencilState);

	wchar_t texInfo[1024] = {0};
	swprintf(texInfo , L"Web3D演示程序\nFPS=%f \nRenderApi=%s" , nFrame*1000.0f/tPassed ,m_pRenderApi->name() );
    
	m_InfoPanel->show();
	ds_wstring infoText = ds_wstring(texInfo) ;   
	
	infoText = ds_wstring(texInfo) ;
    infoText += m_textInfo.c_str();

	infoText += L"\nPowered By xEvol3D "; 
	m_InfoPanel->setText(infoText.c_str() );
	m_xuiWinMgr->draw();
	//m_pRenderApi->
	//===================
	m_pRenderApi->endScene();
	//m_pRenderApi->endSelectMode(0,0,NULL);
	m_pRenderApi->swapBuffer();         
}

CEvolEnviroment::CEvolEnviroment()
{
	m_pEngine   = NULL;
	m_pRenderApi   = NULL;

	m_pCamera   = NULL;
	m_pStencilState   = NULL;
	m_DefRasterizer   = NULL;
	m_ProcedureTexture   = NULL;
	m_cameraStep =1.0f;

	m_pRenderer    = NULL;
    m_fCameraDist = 3.0f;
    m_pLocker = NULL;
	return ;
}

CEvolEnviroment::~CEvolEnviroment()
{
    XSAFE_RELEASEOBJECT(m_pLocker);
    return ;
}
END_NAMESPACE_XEVOL3D

