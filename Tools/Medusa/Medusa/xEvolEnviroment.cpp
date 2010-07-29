#include "Stdafx.h"
#include "xEvolEnviroment.h"
#include "xMedusaEditor.h"
#include "Resource.h"
#include "./Editor/EditorEnv.h"
#include "xCameraOperator.h"
USING_NS_MDED

extern std::wstring  _MEDUSA_RESPATH(const wchar_t* _path);
using namespace XEvol3D;

void setSameViewTarget(IRenderCamera* pCamera , IRenderCamera* pRightCamera)
{
    return ;
    xvec4 rc = pCamera->m_Eye.m_EyeTarget;
    xvec4 lc = pRightCamera->m_Eye.m_EyeTarget;
    xvec4 nc = rc + lc;
    nc = 0.5f * nc;
    pCamera->m_Eye.m_EyeTarget = nc;
    pCamera->setEye(pCamera->m_Eye);
    pRightCamera->m_Eye.m_EyeTarget = nc;
    pRightCamera->setEye(pRightCamera->m_Eye);
}

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

bool   CEvolEnviroment::getRenderWindowRenct(x2DRect& rect)
{
    RECT _rect;
    GetClientRect(m_hRenderWindow , &_rect);
    rect.x = _rect.left;
    rect.y = _rect.top;
    rect.w = _rect.right - _rect.left;
    rect.h = _rect.bottom - _rect.top;
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

    float fCameraStep = m_pCamera->step();
	pCameraNode->setValue(L"Step" , fCameraStep);
	m_pEngine->saveCamera(m_pCamera , pCameraNode );


	GetMedusaEditor()->GetUI()->UnregisteToolbar(m_CameraViewToolbar.m_name);
	m_pRenderApi;
	m_GridPlan->ReleaseObject();   m_GridPlan = NULL;

	m_pCamera->ReleaseObject();    m_pCamera  = NULL;
	m_p2DCamera->ReleaseObject();  m_p2DCamera = NULL;


	//m_pStencilState->ReleaseObject(); m_pStencilState = NULL;
	//m_DefRasterizer->ReleaseObject(); m_DefRasterizer = NULL;

    m_AnaglyphStero.uninit();

    XSAFE_DELETE(m_pDefCameraOperator);
	XSAFE_RELEASEOBJECT(m_pRenderer);
	XSAFE_RELEASEOBJECT(m_xuiWinMgr);
	XSAFE_RELEASEOBJECT(m_InfoPanel);
    XSAFE_RELEASEOBJECT(m_CameraPanel);

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
	m_pEngine->exit();
}


ICameraOperator*  CEvolEnviroment::setCameraOperator(ICameraOperator* pCameraOperator)
{
    ICameraOperator* _old = m_pCameraOperator;

    if(pCameraOperator == NULL) pCameraOperator = m_pDefCameraOperator;

    if(m_pCameraOperator) m_pCameraOperator->onDeActive();
    m_pCameraOperator = pCameraOperator;
    m_pCameraOperator->onActive();
    return _old;
}

ICameraOperator*  CEvolEnviroment::getCameraOperator()
{
    return m_pCameraOperator;
}


IColorSelector* CEvolEnviroment::GetTerrainHeightSelector()
{
    return m_pTerrainHeightSelector;
}

xvec4   CEvolEnviroment::GetTerrainHeight(xTextureDesc& _desc , int x , int y , bool includeOther)
{
	m_pRenderApi->lock();
	IColorSelector* oldColorSelector = m_pRenderApi->colorSelector();
	m_pRenderApi->setColorSelector(m_pTerrainHeightSelector);
    m_TerrainVisitor->incoudeOther(includeOther);
	
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
xvec2i*  CEvolEnviroment::GetSelectData(xTextureDesc& _desc , const int _x , const int _y , const int _w , const int _h)
{
	int x = _x ; int y = _y ; int w = _w ; int h = _h;
	return GetSelectData2(_desc , x , y , w , h );
}

xvec2i*  CEvolEnviroment::GetSelectData2( xTextureDesc& _desc , int& x , int& y , int& w , int& h)
{
	m_pRenderApi->lock();
	m_pRenderApi->colorSelector()->setRenderObjectID(0xcdef0a74 , 0x12345678 );
	m_pRenderApi->beginSelectMode();

	m_pRenderApi->getRenderView()->desc(_desc);
	xvec2i * pRTData = new xvec2i[_desc.m_width * _desc.m_height];
	if(w == 0 ) {x = 0 ; w = _desc.m_width ; }
	if(h == 0 ) {y = 0 ; h = _desc.m_height; }

	if( x < 0) { w +=x ; x = 0 ; } if(y < 0 ) { h += y ; y = 0; }
	if( x + w >= _desc.m_width )  { w = _desc.m_width  - x - 1; }
	if( y + y >= _desc.m_height ) { h = _desc.m_height - y - 1; }

	if( x < 0  || x >= _desc.m_width || y < 0  || y >= _desc.m_height )
	{
		m_pRenderApi->endSelectMode(0 , 0 , _desc.m_width, _desc.m_height , pRTData );
		m_pRenderApi->unlock();
		delete [] pRTData ;
		return NULL;
	}

	updateFrame(0 , m_ColorSelVisitor);
	m_pRenderApi->endSelectMode(x , y , w , h , pRTData );
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
    return m_pCameraOperator->onMessage(msg);
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
        std::ds_wstring _oldScene = m_sceneFile;
        m_sceneFile = _Scene;
		ISceneGraph* pNewScene = XEvol_CreateScene(_Scene ,arg );
        if(pNewScene == NULL)
        {
            m_sceneFile = _oldScene;
        }
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
    m_hRenderWindow = NULL;
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
    m_CameraViewToolbar.SetType(CMEdUiToolBarInfo::eToolBar);
	GetMedusaEditor()->GetUI()->RegisteToolbar(&m_CameraViewToolbar);


    CMEdUiToolBarInfo testMeshuBarInfo;
    testMeshuBarInfo.m_hDll = AfxGetResourceHandle();
    wcscpy(testMeshuBarInfo.m_name , L"CameraViewToolBar");
    wcscpy(testMeshuBarInfo.m_title , L"摄像机操作");
    testMeshuBarInfo.m_ResID = IDR_MedusaDocumentTYPE;
    testMeshuBarInfo.ProcessMenuID(-1);

    m_TexturePaths = m_pEngine->sysCfgNode(L"ResourceManager")->findNode(L"TextureManager");
    m_ResPaths     = m_pEngine->sysCfgNode(L"ResourceManager")->findNode(L"ResoursPath");
	return true;
}

bool  CEvolEnviroment::OnUpdateCommandUI(int ctrlID , int ctrlIdx , NS_MDED::CMEdUiToolBarInfo::CMEdUiCmdUi* pUi)
{
    pUi->Enable(TRUE);
	return true;
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
    m_hRenderWindow = hRenderWindow;
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
		float fCameraStep = pCameraNode->float_value(L"Step");
        m_pCamera->setStep(fCameraStep);
	}
	
    m_pDefCameraOperator = new xCameraOperator();
    m_pDefCameraOperator->setCamera(m_pCamera);
    m_pCameraOperator = m_pDefCameraOperator;
    m_pCameraOperator->onActive();


	m_p2DCamera = m_pRenderApi->createCamera(L"2DCamera");

	//设置高度选择器
	m_pTerrainHeightSelector = new xTerrainHeightSelector(m_pRenderApi , m_pRenderApi->colorSelector() );
	m_pTerrainHeightSelector->create(NULL);


	onResize();

	m_DefRasterizer= m_pRenderApi->createRasterizerState(L"Default");
	m_pRenderApi->setRasterizerState(m_DefRasterizer);


	m_GridPlan = (IDrawableHelper *)(IDrawableHelper *)xDrawElementFactoryMgr::singleton()->createInstance(L"PlaneGrid" , this->renderer() , 0) ;
	m_GridPlan->init(m_pEngine->textureManager() );


	ILightingEnv* pLightEnv = m_pRenderApi->findLightingState(L"xLightState");
	pLightEnv->enableLight(4);
	xLightDesc* pLight= pLightEnv->lightDesc(0);
	pLight->m_position  = xMathLib::xvec4(100000.0f , 100000.0f , 5000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.36f , 0.36f , 0.36f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 25.0f); 
	pLight->m_Ambient   = xMathLib::xvec4(0.03f , 0.03f , 0.03f , 1.0f);

	//第二个灯光
	pLight= pLightEnv->lightDesc(1);
	pLight->m_position  = xMathLib::xvec4(100000.0f , -100000.0f , 5000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.36f , 0.36f , 0.36f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.03f , 0.03f , 0.03f , 1.0f);

	//第三个灯光
	pLight= pLightEnv->lightDesc(2);
	pLight->m_position  = xMathLib::xvec4(-100000.0f , -100000.0f , 5000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.36f , 0.36f , 0.36f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.03f , 0.03f , 0.03f , 1.0f);

	//第四个灯光
	pLight= pLightEnv->lightDesc(3);
	pLight->m_position  = xMathLib::xvec4(-100000.0f , 100000.0f , 5000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.36f , 0.46f , 0.36f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.26f , 0.26f , 0.26f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.23f , 0.23f , 0.23f , 1.0f);

	pLightEnv->applyChange();
	m_xuiWinMgr = xui::xuiWindowManager::createManager(m_pRenderApi , m_pEngine->textureManager() );
	m_xuiWinMgr->getXMLManager()->addPath(_MEDUSA_RESPATH(L"xui/").c_str());
	m_xuiWinMgr->getXMLManager()->addPath(_XEVOL_ABSPATH_(L"./Medusa/xui/") );
	m_InfoPanel   =  XUI_CREATE_DIALOG(xuiDemoInfoPannel,m_xuiWinMgr);
    m_CameraPanel =  XUI_CREATE_DIALOG(xuiCameraInfoPannel , m_xuiWinMgr ); 


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
    if(m_pRenderApi == NULL)
        return ;
	if(m_nTotalFrame > 20)
	{
		m_timePassed  = 0;
		m_nTotalFrame = 0;
	}
	m_timePassed += passedTime;
	m_nTotalFrame ++;
    if(m_pCameraOperator)
    {
        m_pCameraOperator->updateTick(passedTime/1000.0f);
    }
    if(m_AnaglyphStero.bEnable() == false || m_pRenderApi->renderMode() != eRenderMode_Render )
    {
        _renderFrame(passedTime, pVisitor, m_pCamera);
    }
    else
    {
        //m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.3f,0.0f));
        m_AnaglyphStero.begin(m_pRenderApi , m_pCamera);
        m_AnaglyphStero.render(this , pVisitor , passedTime , m_pCamera);
        m_AnaglyphStero.end();
    }

	//m_pRenderApi->endSelectMode(0,0,NULL);
	m_pRenderApi->swapBuffer();
         
}
void CEvolEnviroment::_renderFrame( long passedTime, ISceneVisitor* pVisitor, IRenderCamera* pCamera) 
{
    m_pRenderApi->identityMatrix(MATRIXMODE_World);
    m_pRenderApi->applyCamera(pCamera);
    if(m_pRenderApi->renderMode() == eRenderMode_Select )
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
        uv.setCamera(m_pCamera);
        m_pScene->visit(&uv );
        m_pScene->visit(pVisitor );
    }
    this->m_pRenderer->end(passedTime);


    bool bDrawPlane = GetEditorEnv()->sysValue(L"defualt", L"drawPlane" , false);
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
        bool bGlobalWireFrame = m_pRenderApi->isGlobalWireFrame();
        m_pRenderApi->setGlobalWireFrame(false);
        m_pRenderApi->applyCamera(m_p2DCamera);
        m_pRenderApi->setBlendState( m_pRenderApi->createBlendState(L"AlphaBlend") );
        //===================
        m_pRenderApi->identityMatrix(MATRIXMODE_World);
        m_pRenderApi->setDepthStencilState(m_pStencilState);
        xvec4     destRect(20,300,400,300);
        xvec4     srcRect(0,0,256,256);
        xColor_4f cl(1.0f,1.0f,1.0f,1.0f);
        wchar_t texInfo[1024] = {0};
        swprintf(texInfo , L"FPS=%f \nRenderApi=%s" , m_nTotalFrame *1000.0f/m_timePassed ,m_pRenderApi->name() );

        if( m_CameraPanel && m_pCameraOperator)
        {
                m_CameraPanel->setText( m_pCameraOperator->cameraInfo() );
                m_CameraPanel->show();
        }

        if( m_InfoPanel   ) m_InfoPanel->show();
        ds_wstring infoText = ds_wstring(texInfo) + L"\nMedusa 3D Studio "; 
        m_InfoPanel->setText(infoText.c_str() );
        m_xuiWinMgr->draw();
        m_pRenderApi->setGlobalWireFrame(bGlobalWireFrame);
    }

    //m_pRenderApi->
    //===================
    m_pRenderApi->endScene();
}
std::ds_wstring CEvolEnviroment::sceneDir()
{
    std::ds_wstring scenePath = xFileSystem::singleton()->getPathName(m_sceneFile.c_str() );
    return scenePath;
}

CEvolEnviroment::CEvolEnviroment()
{
    m_TexturePaths =  NULL;
    m_ResPaths     =  NULL;

	m_pEngine      = NULL;
	m_pRenderApi   = NULL;
	m_GridPlan   = NULL;

	m_pCamera   = NULL;
	m_pStencilState   = NULL;
	m_DefRasterizer   = NULL;

	m_pRenderer    = NULL;

	m_ColorSelVisitor= NULL;
	m_renderVisitor= NULL;
	m_TerrainVisitor= NULL;
	m_pTerrainHeightSelector = NULL;

    m_timePassed   = 0;
    m_nTotalFrame  = 0;
    m_pCameraOperator = NULL;
    m_pDefCameraOperator = NULL;
	return ;
}


//Stero

CAnaglyphStero::CAnaglyphStero()
{
    m_pRenderApi  = NULL;
    m_pRightCamera  = NULL;
    m_LVBackBuffer  = NULL;
    m_RVBackBuffer  = NULL;
    m_pStencilState = NULL;
    m_CameraDist    = 0.0f;
    m_bEnable       = false;

}

float CAnaglyphStero::maxDist()
{
    return 10.0f;
}

void CAnaglyphStero::enable(bool bFlag)
{
    m_bEnable = bFlag;
}

bool CAnaglyphStero::bEnable()
{
    return m_bEnable;
}

void CAnaglyphStero::uninit()
{
    m_hSteroProgram.setNULL();
    m_hLVProgram.setNULL();
    m_hRVProgram.setNULL();

    XSAFE_RELEASEOBJECT(m_LVBackBuffer);
    XSAFE_RELEASEOBJECT(m_RVBackBuffer);
    XSAFE_RELEASEOBJECT(m_pRightCamera);
}

void CAnaglyphStero::begin(IRenderApi* pRenderApi , IRenderCamera* pMainCamera )
{
    int w = 0 ;
    int h = 0 ;
    pRenderApi->getRenderViewSize(w , h );
    if(m_pRightCamera == NULL)
    {
        m_pRenderApi = pRenderApi;
        m_pRightCamera = m_pRenderApi->createCamera(L"RightCamera");
        m_pRightCamera->copyFrom(pMainCamera);
        m_pRightCamera->shift( 5.0f );
        m_hLVProgram = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel(0:simple.texture,SteroL)" , NULL);;
        m_hRVProgram = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel(0:simple.texture,SteroR)" , NULL);;

        m_hSteroProgram = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"Stero2D.pixel" , NULL);; 
        m_pStencilState = m_pRenderApi->createDepthStencilState(L"Overlay");
    }

    xTextureDesc _desc;
    _desc.m_width  = 0;
    _desc.m_height = 0;
    if(m_LVBackBuffer) 
        m_LVBackBuffer->desc(_desc);

    if(_desc.m_width != w || _desc.m_height != h || m_LVBackBuffer == NULL)
    {
        XSAFE_RELEASEOBJECT(m_LVBackBuffer);
        XSAFE_RELEASEOBJECT(m_RVBackBuffer);

        m_LVBackBuffer = m_pRenderApi->createRenderView(w , h , true);
        m_LVBackBuffer->createRenderTarget(1 , PIXELFORMAT_R8G8B8A8 , true , true);

        m_RVBackBuffer = m_pRenderApi->createRenderView(w , h , true);
        m_RVBackBuffer->createRenderTarget(1 , PIXELFORMAT_R8G8B8A8 , true , true);

    }


    m_pRightCamera->copyFrom(pMainCamera);
    m_pRightCamera->shift(m_CameraDist);
    setSameViewTarget(pMainCamera , m_pRightCamera);
}

void CAnaglyphStero::end()
{

}

void CAnaglyphStero::render(CEvolEnviroment* pEnv , ISceneVisitor* pVisitor, long passedTime , IRenderCamera* pMainCamera )
{

    m_pRenderApi->pushRenderView(m_RVBackBuffer);
    pEnv->_renderFrame(passedTime          , pVisitor , m_pRightCamera );
    m_pRenderApi->popRenderView();

    m_pRenderApi->pushRenderView(m_LVBackBuffer);
    pEnv->_renderFrame(0 , pVisitor , pMainCamera );
    m_pRenderApi->popRenderView();



    //===================
    bool bGlobalWireFrame = m_pRenderApi->isGlobalWireFrame();
    m_pRenderApi->setGlobalWireFrame(false);
    m_pRenderApi->applyCamera( pEnv->camera2D() );
    m_pRenderApi->identityMatrix(MATRIXMODE_World);
    m_pRenderApi->setDepthStencilState(m_pStencilState);
    int w = 0; int h = 0;
    m_pRenderApi->getRenderViewSize(w , h );
    xvec4     destRect(0,0,w,h);
    xvec4     srcRect(0,0,256,256);
    xColor_4f cl(1.0f,1.0f,1.0f,1.0f);

    IRenderTarget* pRTLeft  = dynamic_cast<IRenderTarget*>(m_LVBackBuffer->renderTarget(0));
    IRenderTarget* pRTRight = dynamic_cast<IRenderTarget*>(m_RVBackBuffer->renderTarget(0));
    IGpuProgram*   pGpuProgram = m_hSteroProgram.getResource();

    m_pRenderApi->setBlendState( m_pRenderApi->createBlendState(L"AlphaBlend") );
    m_pRenderApi->setGpuProgram( pGpuProgram );

    m_pRenderApi->setTexture(Texture_Diffuse       , pRTLeft->toTexture() );
    m_pRenderApi->setTexture(L"RightEyeTexture"    , pRTRight->toTexture() );
    m_pRenderApi->drawRectf( pRTLeft->toTexture()  , &destRect.x, cl);

    //Unbind the texture;
    m_pRenderApi->setShaderResource(eShader_PixelShader , 0 , NULL);
    m_pRenderApi->setShaderResource(eShader_PixelShader , 1 , NULL);
    m_pRenderApi->setGlobalWireFrame(bGlobalWireFrame);
}

float CAnaglyphStero::dist()
{
    return m_CameraDist;
}

void  CAnaglyphStero::setDist(float _dist)
{
    m_CameraDist = _dist;
}

