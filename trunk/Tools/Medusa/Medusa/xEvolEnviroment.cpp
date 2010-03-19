#include "Stdafx.h"
#include "xEvolEnviroment.h"
#include "xMedusaEditor.h"
#include "Resource.h"
#include "./Editor/EditorEnv.h"
USING_NS_MDED

extern std::wstring  _MEDUSA_RESPATH(const wchar_t* _path);
using namespace XEvol3D;

bool   CEvolEnviroment::onResize()
{
	x2DRect _rect;
	xTextureDesc _desc;
	m_pRenderApi->onResize(0 , 0 );
	m_pRenderApi->getRenderView()->desc(_desc);
	m_pTerrainHeightSelector->resize(_desc.m_width , _desc.m_height );
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
	m_pCamera->dirty();
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

	m_pCamera->m_Eye.m_EyePos    = xvec4(160.0f , -1204.0f , -126.0f );
	m_pCamera->m_Eye.m_EyeTarget = xvec4(160.0f , 796.0f   , -126.0f );
	m_pCamera->m_Eye.m_Up        = xvec4(0.0f   , 0.0f     ,    1.0f );
	m_pCamera->dirty();
}

void     CEvolEnviroment::exit()
{

	xXmlNode* pRootNode     = m_pEngine->sysConfig();
	xXmlNode* pCamerasNode  = pRootNode->findNode(L"Cameras"); 
	if(pCamerasNode == NULL) 
	{
		pCamerasNode = pRootNode->insertNode(L"Cameras");		
	}

	xXmlNode* pCameraNode = pCamerasNode->findNode(L"Default");
	if(pCameraNode == NULL)
	{
		pCameraNode = pCamerasNode->insertNode(L"Default");
	}

	pCameraNode->setValue(L"Step" , m_cameraStep);
	m_pEngine->saveCamera(m_pCamera , pCameraNode );


	GetMedusaEditor()->GetUI()->UnregisteToolbar(m_CameraViewToolbar.m_name);
	m_pRenderApi;
	m_GridPlan->ReleaseObject();   m_GridPlan = NULL;

	m_pCamera->ReleaseObject();    m_pCamera  = NULL;
	m_p2DCamera->ReleaseObject();  m_p2DCamera = NULL;


	//m_pStencilState->ReleaseObject(); m_pStencilState = NULL;
	//m_DefRasterizer->ReleaseObject(); m_DefRasterizer = NULL;


	XSAFE_RELEASEOBJECT(m_pSelView );
	XSAFE_RELEASEOBJECT(m_pRenderer);
	XSAFE_RELEASEOBJECT(m_xuiWinMgr);
	XSAFE_RELEASEOBJECT(m_InfoPanel);

	delete m_pScenePlacementOprator; m_pScenePlacementOprator = NULL;
	XSAFE_RELEASEOBJECT(m_ColorSelVisitor);
	XSAFE_RELEASEOBJECT(m_renderVisitor);
	XSAFE_RELEASEOBJECT(m_TerrainVisitor);

	//地形高度选择器
	XSAFE_RELEASEOBJECT(m_pTerrainHeightSelector);

	if(m_pScene)
	{
		m_pScene->unload();
		m_pScene->ReleaseObject();
	}
	m_hProgram.setNULL();
	m_hFont.setNULL();
	m_pEngine->exit();

	
}

xvec4   CEvolEnviroment::GetTerrainHeight(xTextureDesc& _desc , int x , int y)
{
	m_pRenderApi->lock();
	IColorSelector* oldColorSelector = m_pRenderApi->colorSelector();
	m_pRenderApi->setColorSelector(m_pTerrainHeightSelector);
	
	xvec4 _ret( 0.0f,0.0f,0.0f,0.0f );
	m_pRenderApi->beginSelectMode();
	m_pRenderApi->getRenderView()->desc(_desc);

	if( x < 0  || x >= _desc.m_width || y < 0  || y >= _desc.m_height )
	{
		m_pRenderApi->setColorSelector(oldColorSelector);
		m_pRenderApi->unlock();		
		return _ret;
	}
	updateFrame(0 , m_TerrainVisitor);
	m_pRenderApi->endSelectMode(x , y , 1, 1 , &_ret );
	m_pRenderApi->setColorSelector(oldColorSelector);
	m_pRenderApi->unlock();

	return _ret;
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

	updateFrame(0 , m_ColorSelVisitor);
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
			bool bDrawPlane = GetEditorEnv()->sysValue(L"defualt",L"drawPlane" , false);
			GetEditorEnv()->setSysValue(L"defualt",L"drawPlane" , !bDrawPlane);
		}
		
		if(msg.Keyboard.nKey == VKEY_DELETE)
		{
			bool bAllowOpScene = GetEditorEnv()->sysValue(L"defualt",L"sceneEditable" , true);
			if(bAllowOpScene && GetSceneSelection()->GetCurSelNode())
			{
				ISceneNode* pNode = GetSceneSelection()->GetCurSelNode();
				ISceneNode* pParentNode = pNode->getParent();
				if(pParentNode == NULL)
				{
					MessageBox(GetActiveWindow() , L"节点没有父节点", L"错误" , MB_OK);
				}
				else
				{
					xInsertNodeCommand* pCmd = xInsertNodeCommand::newInstacne(pParentNode , pNode , false);
					if( pParentNode->removeChild(pNode,true) )
					{
						GetSceneSelection()->setCurSelNode(NULL);
						GetMedusaEditor()->GetUI()->InsertCommand(pCmd);
						IMEdMainUI* pUI = (GetMedusaEditor()->GetUI());
						if (NULL != pUI)
						{
							pUI->fireMEdUIEvent(eMEUI_SceneRemoveNode , (int)m_pScene , (int)pNode );
						}
					}
					else
					{
						delete pCmd;
					}
					
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
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);
		}
		else if(msg.Keyboard.cKeyChar == 'd')
		{
			m_pCamera->shift(m_cameraStep);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);
		}
		else if(msg.Keyboard.cKeyChar == 'w')
		{
			m_pCamera->toward(m_cameraStep);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);
		}
		else if(msg.Keyboard.cKeyChar == 's')
		{
			m_pCamera->toward(-m_cameraStep);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

		}
		else if(msg.Keyboard.nKey == VKEY_UP)
		{
			m_pCamera->pitch(2);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

		}
		else if(msg.Keyboard.nKey == VKEY_DOWN)
		{
			m_pCamera->pitch(-2);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

		}
		if(msg.Keyboard.nKey == VKEY_LEFT)
		{
			m_pCamera->yaw(2);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

		}
		else if(msg.Keyboard.nKey == VKEY_RIGHT)
		{
			m_pCamera->yaw(-2);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

		}
		else if(msg.Keyboard.nKey == VKEY_PRIOR)
		{
			m_pCamera->upDown(m_cameraStep);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

		}
		else if(msg.Keyboard.nKey == VKEY_NEXT)
		{
			m_pCamera->upDown(-m_cameraStep);
			GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

		}
		else 
		{
			return false;
		}
		return true;
	}
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
			m_pScene->ReleaseObject();
		}
		m_pScene = pNewScene;
		m_sceneFile = _Scene;

		m_pScene->setPathManager( m_pEngine->getResPathMgr() );
		IMEdMainUI* pUI = (GetMedusaEditor()->GetUI());
		if (NULL != pUI)
		{
			pUI->fireMEdUIEvent(eMEUI_SceneChanged , (int)m_pScene , 0 );
			pUI->fireMEdUIEvent(eMEUI_SceneLoaded  , (int)m_pScene , 0 );
		}
		return true;
	}
	return false;
}
bool    CEvolEnviroment::init()
{
	m_pScene = NULL;
	wchar_t appname[512] = {0};
	GetModuleFileName(NULL , appname , 512);
	xOperationSys::singleton()->init( appname );
	xStringTable::singleton()->addPath(_MEDUSA_RESPATH(L"Language/").c_str());
	xStringTable::singleton()->addPath(_XEVOL_ABSPATH_(L"./Medusa/Language/") );
	xStringTable::singleton()->setLanguage(NULL);

	m_pEngine = new xEvol3DEngine;
	m_cfgFile = _MEDUSA_RESPATH(L"Configure/system.xml").c_str();
	m_pEngine->initPlatform(this , m_cfgFile.c_str() , _XEVOL_ABSPATH_(L"./") );
	m_pScenePlacementOprator = new xObjPlacement;
	

	m_CameraViewToolbar.m_hDll = AfxGetResourceHandle();
	wcscpy(m_CameraViewToolbar.m_name , L"CameraViewToolBar");
	wcscpy(m_CameraViewToolbar.m_title , L"摄像机操作");
	m_CameraViewToolbar.m_ResID = IDR_CAMERAVIEW;

	m_CameraViewToolbar.m_funcCallback = this;
	m_CameraViewToolbar.ProcessToolbarID(-1);
	GetMedusaEditor()->GetUI()->RegisteToolbar(&m_CameraViewToolbar);


    CMEdUiToolBarInfo testMeshuBarInfo;
    testMeshuBarInfo.m_hDll = AfxGetResourceHandle();
    wcscpy(testMeshuBarInfo.m_name , L"CameraViewToolBar");
    wcscpy(testMeshuBarInfo.m_title , L"摄像机操作");
    testMeshuBarInfo.m_ResID = IDR_MedusaDocumentTYPE;
    testMeshuBarInfo.ProcessMenuID(-1);

	return true;
}
NS_MDED::CMEdUiToolBarInfo::CommandUIStatus  CEvolEnviroment::OnUpdateCommandUI(int ctrlID , int ctrlIdx)
{
	return NS_MDED::CMEdUiToolBarInfo::CUS_ENABLE;
}

bool  CEvolEnviroment::OnCommand(int ctrlID , int ctrlIdx)
{
	switch(ctrlID)
	{
	case ID_BTN_TOPCAMERA:
		{
			xGeomLib::xaabb aabb;
			aabb.m_Max = xvec3(100,100,100);
			aabb.m_Min = xvec3(-100, -100, -100);
			topCamera( aabb );
		}
		break;
	case ID_BTN_FRONTCAMERA:
		{
			xGeomLib::xaabb aabb;
			aabb.m_Max = xvec3(80,80,80);
			aabb.m_Min = xvec3(-80, -80, -80);
			frontCamera( aabb );
		}
		break;
	}
	return true;
}
void     CEvolEnviroment::initRenderer(HWND hRenderWindow)
{
	m_pEngine->initRenderer(L"Medusa" , (void*)hRenderWindow );
	m_pRenderApi = m_pEngine->renderApi();
	//===============

	m_pRenderer = xRendererManager::singleton()->createRenderer(L"KidRenderer",m_pRenderApi);
	m_pRenderer->loadConfig( sysCfgfile() ,NULL,0);


	m_pRenderApi = m_pRenderApi;


	//初始化摄像机
	m_pCamera = m_pRenderApi->createCamera(L"Default");
	xGeomLib::xaabb aabb;
	aabb.m_Max = xvec3(20,20,20);
	aabb.m_Min = xvec3(-20, -20, -20);
	frontCamera( aabb );
	m_pCamera->setProjectType(xGeomLib::xCamera::PT_PROJECT);
	//加载摄像机配置
	xXmlNode* pCamerasNode  = m_pEngine->sysConfig();
	pCamerasNode = pCamerasNode->findNode(L"Cameras");
	if(pCamerasNode && pCamerasNode->bool_value(L"enable"))
	{
		xXmlNode* pCameraNode = pCamerasNode->findNode(L"Default");
		m_pEngine->loadCamera(m_pCamera , pCameraNode);
		m_cameraStep = pCameraNode->float_value(L"Step");
	}
	

	m_p2DCamera = m_pRenderApi->createCamera(L"2DCamera");

	//设置高度选择器
	m_pTerrainHeightSelector = new xTerrainHeightSelector(m_pRenderApi , m_pRenderApi->colorSelector() );
	m_pTerrainHeightSelector->create(NULL);


	onResize();

	m_pStencilState = m_pRenderApi->createDepthStencilState(L"Overlay");

	m_hProgram = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel<0:simple.texture,simple.fakehdr>" , NULL);;

	m_hFont    = m_pRenderApi->findFont(L"small");


	//m_Model = m_pModelMgr->add(L"einxin.xrm" , true , 0);


	m_DefRasterizer= m_pRenderApi->createRasterizerState(L"Default");
	m_pRenderApi->setRasterizerState(m_DefRasterizer);



	
	m_GridPlan = (IDrawableHelper *)(IDrawableHelper *)xDrawElementFactoryMgr::singleton()->createInstance(L"PlaneGrid" , this->renderer() , 0) ;
	m_GridPlan->init(m_pEngine->textureManager() );


	m_pSelView = m_pRenderApi->createRenderView(800 , 600 , true);
	m_pSelView->createRenderTarget(1 , PIXELFORMAT_R8G8B8A8 , true , true);

	ILightingEnv* pLightEnv = m_pRenderApi->findLightingState(L"xLightState");
	pLightEnv->enableLight(4);
	xLightDesc* pLight= pLightEnv->lightDesc(0);
	pLight->m_position  = xMathLib::xvec4(100000.0f , 100000.0f , 5000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 25.0f); 
	pLight->m_Ambient   = xMathLib::xvec4(0.03f , 0.03f , 0.03f , 1.0f);

	//第二个灯光
	pLight= pLightEnv->lightDesc(1);
	pLight->m_position  = xMathLib::xvec4(100000.0f , -100000.0f , -5000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.03f , 0.03f , 0.03f , 1.0f);

	//第三个灯光
	pLight= pLightEnv->lightDesc(2);
	pLight->m_position  = xMathLib::xvec4(-100000.0f , -100000.0f , -5000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.03f , 0.03f , 0.03f , 1.0f);

	//第四个灯光
	pLight= pLightEnv->lightDesc(3);
	pLight->m_position  = xMathLib::xvec4(-100000.0f , 100000.0f , 5000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.03f , 0.03f , 0.03f , 1.0f);

	pLightEnv->applyChange();
	m_xuiWinMgr = xui::xuiWindowManager::createManager(m_pRenderApi , m_pEngine->textureManager() );
	m_xuiWinMgr->getXMLManager()->addPath(_MEDUSA_RESPATH(L"xui/").c_str());
	m_xuiWinMgr->getXMLManager()->addPath(_XEVOL_ABSPATH_(L"./Medusa/xui/") );
	m_InfoPanel  = XUI_CREATE_DIALOG(xuiDemoInfoPannel,m_xuiWinMgr);



	//设置渲染器的渲染Tech.
	m_pRenderer->setRenderTech(L"Basic");

	m_ColorSelVisitor     = new xSceneColorSelectVisitor(m_pRenderer , m_pCamera);
	m_renderVisitor = new xSceneRenderVisitor(m_pRenderer , m_pCamera);
	m_TerrainVisitor = new xTerrainHeightSelVisitor(m_pRenderer , m_pCamera);



	//初始化操作器
	m_pScenePlacementOprator->init(this);
	m_Selection.init(this);
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
		m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.3f,0.0f));
	m_pRenderApi->beginScene();
	xMathLib::xmat4 mat;
	//xMathLib::XM_RotateY(mat,angle);

	//绘制模型
	m_pRenderApi->colorSelector()->setRenderObjectID(1,0);
	m_pRenderApi->identityMatrix(MATRIXMODE_World);

	this->m_pRenderer->begin(passedTime);
	//m_pModelEffect->draw( m_pModelElement );

	xSceneRenderVisitor rv(m_pRenderer , m_pCamera);
	xSceneUpdateVisitor uv;
	uv.setFrameTime(passedTime);
	if(m_pScene) 
	{
		m_pScene->visit(&uv );
	    m_pScene->visit(pVisitor );
	}
	this->m_pRenderer->end(passedTime);


	bool bDrawPlane = GetEditorEnv()->sysValue(L"defualt",L"drawPlane" , false);
	if(bDrawPlane)
	{
		//Plan
		m_pRenderApi->identityMatrix(MATRIXMODE_World);
		xMathLib::XM_Scale(mat,500.0f,500.0f,500.0f);
		m_pRenderApi->multiMatrix(mat.data, MATRIXMODE_World);
		m_pRenderApi->colorSelector()->setRenderObjectID(101,0);
		m_GridPlan->begin();
		m_GridPlan->render(passedTime);
	}

	if(m_pRenderApi->renderMode() == eRenderMode_Render)
	{
		m_pRenderApi->applyCamera(m_p2DCamera);
		m_pRenderApi->setBlendState( m_pRenderApi->createBlendState(L"AlphaBlend") );
		//===================
		m_pRenderApi->identityMatrix(MATRIXMODE_World);
		m_pRenderApi->setDepthStencilState(m_pStencilState);
		xvec4     destRect(20,300,400,300);
		xvec4     srcRect(0,0,256,256);
		xColor_4f cl(1.0f,1.0f,1.0f,1.0f);
		IGpuProgram* pGpuProgram = m_hProgram.getResource()  ;
		wchar_t texInfo[1024] = {0};
		swprintf(texInfo , L"FPS=%f \nRenderApi=%s" , nFrame*1000.0f/tPassed ,m_pRenderApi->name() );

		m_InfoPanel->show();
		ds_wstring infoText = ds_wstring(texInfo) + L"\nMudus 3D Studio "; 
		m_InfoPanel->setText(infoText.c_str() );
		m_xuiWinMgr->draw();
	}

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
	m_GridPlan   = NULL;

	m_pCamera   = NULL;
	m_pStencilState   = NULL;
	m_DefRasterizer   = NULL;

    m_cameraStep =40.0f;

	m_pSelView   = NULL;
	m_pRenderer    = NULL;

	m_ColorSelVisitor= NULL;
	m_renderVisitor= NULL;
	m_TerrainVisitor= NULL;
	m_pTerrainHeightSelector = NULL;
	return ;
}
