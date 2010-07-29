// TestBase.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "RayTracerDemo.h"
#include "working/xWorkingThread.h"
#include "XMathLib/xspline.h"

using namespace XEvol3D;

class CMyJob : public IWorkingJob
{
    int  m_JobID;
    bool m_InWorking;
public:
    IMPL_REFCOUNT_OBJECT_INTERFACE(CMyJob);
    CMyJob(int iJobId)
    {
        m_InWorking = false;
        m_JobID = iJobId;
    }
    bool  isWorking()
    {
        return m_InWorking;
    }

    void  beginWorking()
    {
        m_InWorking = true;
    }
    void  endWorking() 
    {
        m_InWorking = false;
    }
    bool  doWorking()
    {
        XEVOL_LOG(eXL_DEBUG_HIGH ,L"Do a Job Working ID = %d \n" , m_JobID );
        xSleep(1000 + rand()%1000 );
        return true;
    }
};


LRESULT CALLBACK RenderWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code here...
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= RenderWndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= L"RenderWnd";
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
bool CRaytracerPrimaryRenderCallback::preDrawPass(IBaseRenderer* pSender , xRenderPass* pPass )
{
    IDrawElement* pDrawable = pPass->getDrawable();
    xModelElement* pModelElement = dynamic_cast<xModelElement*>(pDrawable);
    if(pModelElement)
    {
        const wchar_t* name = pModelElement->modelName();
        if(name && std::wstring(name).find(L"Light") != std::wstring::npos)
        {
            pSender->renderApi()->setShader(m_hLightShader , eShader_PixelShader);
            pSender->renderApi()->setBlendState(NULL);
        }
        else
        {
            pSender->renderApi()->setShader(m_hObjectShader , eShader_PixelShader);
            pSender->renderApi()->setBlendState(NULL);
        }

    }
    else
    {
        pSender->renderApi()->setShader(m_hObjectShader , eShader_PixelShader);
        pSender->renderApi()->setBlendState(NULL);
    }
    return true;
}

bool CRaytracerPrimaryRenderCallback::postDrawPass(IBaseRenderer* pSender , xRenderPass* pPass )
{
    return true;
}

void CRaytracerPrimaryRenderCallback::init(xEvol3DEngine* pEngine)
{
    //m_hObjectShader = pEngine->renderApi()->shaderManager()->add(L"PrimaryRay(0:simple.texture)" , (unsigned int)eShader_PixelShader , true);
    //IBaseShader* pShader = m_hObjectShader.getResource();

    //m_hLightShader = pEngine->renderApi()->shaderManager()->add(L"RaytracerLight" , (unsigned int)eShader_PixelShader , true);
    //pShader = m_hLightShader.getResource();

    return ;
}

void  CRaytracerPrimaryRenderCallback::uninit()
{
    m_hObjectShader.unload();
    m_hObjectShader.setNULL();

    m_hLightShader.unload();
    m_hLightShader.setNULL();
}

HWND GetRenderWindow(HINSTANCE hInstance, int nCmdShow , int w , int h)
{
    static HWND hWnd = NULL;
    if(hWnd != NULL)
        return hWnd;

    MyRegisterClass(hInstance);
    RECT _rect = {120 , 120 , w + 120, h + 120};
    ::AdjustWindowRect(&_rect , WS_OVERLAPPEDWINDOW , false);
    hWnd = CreateWindow(L"RenderWnd", L"RenderView", WS_OVERLAPPEDWINDOW, _rect.left, _rect.top, _rect.right - _rect.left, _rect.bottom - _rect.top , NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}

xvec4 CEvolEnviroment::TraceRay(xRayTraceNodeData& data)
{
    if(data.m_Pos.w < 0.0f)
        return xvec4(0.0f, 0.0f , 0.0f ,0.0f);

    IRenderCamera* pCamera = m_pRenderApi->createCamera(L"temp");
    pCamera->m_Eye.m_EyePos = data.m_Pos;
    pCamera->m_Eye.m_EyeTarget = data.m_Pos + 10.0f * data.m_Normal;
    pCamera->m_Eye.m_Up = data.m_Tan;
    pCamera->setAspect(1.0f);
    pCamera->setFarPlan(10000.0f);
    pCamera->setNearPlan(1.0f);
    pCamera->setFOV(170.0f);
    pCamera->setProjectType(XGeomLib::xCamera::PROJECT_T::PT_PROJECT);
    pCamera->dirty();

    IRenderCamera* pOldCamera = m_pCamera;
    m_pCamera = pCamera;
    IRendererEventCallback* pOldCb = m_pRenderer->getEventCallBack();
    m_pRenderer->setEventcallback(&m_PrimRayCb);
    xvec4 ret = CaclBuffer(NULL , m_TraceView[data.m_Level] , data.m_Level +1 );
    m_pRenderer->setEventcallback(pOldCb);
    m_pCamera = pOldCamera;
    //return ret;
    return ret;//0.5f * (data.m_Normal + xvec4(1.0f, 1.0f , 1.0f ,1.0f));
}

xvec4  CEvolEnviroment::CaclBuffer( HWND hRenderWindow, IRenderView* pRenderView , int level) 
{
    xTextureDesc _rvDesc;
    pRenderView->desc(_rvDesc);
    m_pRenderApi->pushRenderView(pRenderView);
    for(int i = 2 ; i < MAX_RT_VIEW  ; i ++)
    {
        pRenderView->clearRenderTarget(xColor_4f(0.0f,0.0f,0.0f,0.0f) , i);
    }
    //Position buffer清空
    pRenderView->clearRenderTarget(xColor_4f(0.0f,0.0f,0.0f,-1.0f) , 1);


    updateFrame(0 , m_renderVisitor , false);

    m_pRenderApi->popRenderView();






    xvec4 ret(1.0f,1.0f,1.0f,0.0f);
    if(level == 0)
    {
        xvec4* buf_Position = new xvec4[_rvDesc.m_width * _rvDesc.m_height];
        xvec4* buf_Normal   = new xvec4[_rvDesc.m_width * _rvDesc.m_height];
        xvec4* buf_Reflect  = new xvec4[_rvDesc.m_width * _rvDesc.m_height];
        xvec4* buf_Tangent  = new xvec4[_rvDesc.m_width * _rvDesc.m_height];
        ((IRenderTarget*)pRenderView->renderTarget(POS_VIEW)    )->grabRenderTagetData( (void*)buf_Position ,0 , 0 , _rvDesc.m_width  , _rvDesc.m_height);
        ((IRenderTarget*)pRenderView->renderTarget(NOR_VIEW)    )->grabRenderTagetData( (void*)buf_Normal   ,0 , 0 , _rvDesc.m_width  , _rvDesc.m_height);
        ((IRenderTarget*)pRenderView->renderTarget(REFLECT_VIEW))->grabRenderTagetData( (void*)buf_Reflect  ,0 , 0 , _rvDesc.m_width  , _rvDesc.m_height);
        ((IRenderTarget*)pRenderView->renderTarget(TANGENT_VIEW))->grabRenderTagetData( (void*)buf_Tangent  ,0 , 0 , _rvDesc.m_width  , _rvDesc.m_height);

        HDC hDC = ::GetDC(hRenderWindow);
        GetColor _GetColor;
        for(int y = 0 ; y < _rvDesc.m_height ; y ++)
        {
            for(int x = 0 ; x < _rvDesc.m_width ; x ++)
            {
                xRayTraceNodeData data;
                data.m_Level = level;
                data.m_Pos     = buf_Position[x + y * _rvDesc.m_width] ;
                data.m_Normal  = buf_Normal  [x + y * _rvDesc.m_width] ;
                data.m_Reflect = buf_Reflect [x + y * _rvDesc.m_width] ;
                data.m_Tan     = buf_Tangent [x + y * _rvDesc.m_width] ;

                xvec4 lum = TraceRay(data);
                float r = _GetColor(lum.x );
                float g = _GetColor(lum.y );
                float b = _GetColor(lum.z );
                DWORD cl = RGB(r * 255 , g * 255 , b * 255 );
                SetPixel(hDC , x , y , cl);
            }
        }
        ::ReleaseDC(hRenderWindow , hDC);
        delete[]buf_Position;
        delete[]buf_Normal;  
        delete[]buf_Reflect ;
        delete[]buf_Tangent ;
    }
    else
    {
        //计算每个点的颜色
        xvec4* buf_Color  = new xvec4[_rvDesc.m_width * _rvDesc.m_height];
        ret = xvec4(0.0f,0.0f,0.0f,0.0f);
        float weight = 0.0f;
        ((IRenderTarget*)pRenderView->renderTarget(COL_VIEW)    )->grabRenderTagetData((void*)buf_Color , 0 , 0 , _rvDesc.m_width  , _rvDesc.m_height );
        GetColor _GetColor;
        for(int y = 0 ; y < _rvDesc.m_height ; y ++)
        {
            for(int x = 0 ; x < _rvDesc.m_width ; x ++)
            {
                xvec4 pixel = buf_Color[x + y * _rvDesc.m_width] ;
                ret = pixel + ret;
                weight += 1.0f;
            }
        }
        ret = 640.0f/weight * ret ;
        delete [] buf_Color;
    }
    return ret;
}

void CEvolEnviroment::DoTrace()
{
    xTextureDesc _rvDesc;
    m_pRenderApi->getRenderView()->desc( _rvDesc );
    if(m_PrimaryBuffer != NULL)
    {
        xTextureDesc _pbDes ;
        m_PrimaryBuffer->desc(_pbDes);
        if(_rvDesc.m_width != _pbDes.m_width || _rvDesc.m_height != _pbDes.m_height)
        {
            XSAFE_RELEASEOBJECT(m_PrimaryBuffer);
            for(size_t i = 0 ; i < m_TraceView.size() ; i ++)
            {
                XSAFE_RELEASEOBJECT(m_TraceView[i]);
            }
            m_TraceView.clear();
        }
    }


    if(m_PrimaryBuffer == NULL)
    {
        m_PrimaryBuffer = m_pRenderApi->createRenderView(_rvDesc.m_width , _rvDesc.m_height , false);
        m_PrimaryBuffer->createRenderTarget(MAX_RT_VIEW , PIXELFORMAT_R32G32B32A32F , true , false);
        for(int i  = 0 ; i < MAX_TRACE_LEVEL ; i ++)
        {
            IRenderView* pTracerView = m_pRenderApi->createRenderView(64 , 64 , true);
            pTracerView->createRenderTarget(MAX_RT_VIEW , PIXELFORMAT_R32G32B32A32F , true , false);
            m_TraceView.push_back(pTracerView);
        }
    }

    HWND hRenderWindow = GetRenderWindow(::GetModuleHandle(NULL) , SW_SHOW , _rvDesc.m_width  , _rvDesc.m_height );
    IRendererEventCallback* pOldCb = m_pRenderer->getEventCallBack();
    m_pRenderer->setEventcallback(&m_PrimRayCb);
    CaclBuffer(hRenderWindow, m_PrimaryBuffer , 0);
    m_pRenderer->setEventcallback(pOldCb);
    return ;
}

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

    m_pCamera->m_Eye.m_EyePos    = xvec4(cen.x + 10 * len.x ,cen.y , cen.z );
    m_pCamera->m_Eye.m_EyeTarget = xvec4(cen.x         ,cen.y , cen.z);
    m_pCamera->m_Eye.m_Up        = xvec4(0.0f,0.0f,1.0f);
    m_cameraStep = len.x/10.0f;
    m_pCamera->setFarPlan(len.x * 150.0f);
    m_pCamera->setNearPlan(0.1f);
}

void     CEvolEnviroment::exit()
{
    if(m_pScene)
    {
        m_pScene->unload();
        m_pScene->ReleaseObject();
    }
    m_PrimRayCb.uninit();
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
        if(msg.Keyboard.nKey == VKEY_F6)
        {
            DoTrace();
        }
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
    wchar_t appname[512] = {0};
    GetModuleFileName(NULL , appname , 512);
    xOperationSys::singleton()->init( appname );
    xStringTable::singleton()->addPath(_XEVOL_ABSPATH_(L"Language/"));
    xStringTable::singleton()->setLanguage(NULL);

    m_pEngine = new xEvol3DEngine;
    m_cfgFile = _XEVOL_ABSPATH_(L"./Raytracer/raytracer.xml");
    m_pEngine->initPlatform(this , m_cfgFile.c_str() , _XEVOL_ABSPATH_(L"./") );

    return true;
}


void     CEvolEnviroment::initRenderer(HWND hRenderWindow)
{
    m_pEngine->initRenderer(L"BaseDemo" , (void*)hRenderWindow );
    m_pRenderApi = m_pEngine->renderApi();
    //===============

    m_pRenderer = xRendererManager::singleton()->createRenderer(L"KidRenderer",m_pRenderApi);
    m_pRenderer->loadConfig( sysCfgfile() ,NULL,0);


    //光线追踪器用到得
    m_pEngine->textureManager()->addPath( _XEVOL_ABSPATH_(L"./Raytracer/texture/") );
    m_pEngine->modelMgr()->addPath( _XEVOL_ABSPATH_(L"./Raytracer/model/") );


    wchar_t* ch_ = L"啊";
    //xDistanceFontGenerator fontGen(32 , 4096 , ch_[0] , ch_[0] + 2);
    //fontGen.generate( _XEVOL_ABSPATH_(L".\\font\\STXINGKA.TTF") ,_XEVOL_ABSPATH_(L".\\font\\test") );
    //fontGen.generate( _XEVOL_ABSPATH_(L".\\font\\msyh.TTF") ,_XEVOL_ABSPATH_(L".\\font\\test") );
    //m_FontGen.load(m_pRenderApi , _XEVOL_ABSPATH_(L"./font/font.tga"));
    //m_FontGen.proccess();

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




    m_GridPlan = (IDrawableHelper *)(IDrawableHelper *)xDrawElementFactoryMgr::singleton()->createInstance(L"PlaneGrid" , this->renderer() , 0) ;
    m_GridPlan->init(m_pEngine->textureManager() );


    ILightingEnv* pLightEnv = m_pRenderApi->findLightingState(L"xLightState");
    pLightEnv->enableLight(2);
    xLightDesc* pLight= pLightEnv->lightDesc(0);
    pLight->m_position  = xMathLib::xvec4(1000.0f , 1000.0f , 1000.0f , 1.0f);
    pLight->m_Diffuse   = xMathLib::xvec4(0.6f , 0.0f , 0.0f , 1.0f);  
    pLight->m_Speculer  = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 25.0f); 
    pLight->m_Ambient   = xMathLib::xvec4(0.3f , 0.0f , 0.8f , 1.0f);

    //第二个灯光
    pLight= pLightEnv->lightDesc(1);
    pLight->m_position  = xMathLib::xvec4(1000.0f , -1000.0f , -500.0f , 1.0f);
    pLight->m_Diffuse   = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 1.0f);  
    pLight->m_Speculer  = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 25.0f);  
    pLight->m_Ambient   = xMathLib::xvec4(0.1f , 0.1f , 0.1f , 1.0f);

    pLightEnv->applyChange();
    m_xuiWinMgr = xui::xuiWindowManager::createManager(m_pRenderApi , m_pEngine->textureManager() );
    m_xuiWinMgr->getXMLManager()->addPath(_XEVOL_ABSPATH_(L"xui/"));
    m_InfoPanel  = XUI_CREATE_DIALOG(xuiDemoInfoPannel,m_xuiWinMgr);



    //设置渲染器的渲染Tech.
    m_pRenderer->setRenderTech(L"Basic");

    m_ColorSelVisitor     = new xSceneColorSelectVisitor(m_pRenderer , m_pCamera);
    m_renderVisitor = new xSceneRenderVisitor(m_pRenderer , m_pCamera);

    //初始化操作器
    m_Selection.init(this);

    m_pScenePlacementOprator = new xObjPlacement( & this->m_Selection );
    m_pScenePlacementOprator->init(this);
    m_pScenePlacementOprator->SetMoveOperator();


    xImage* pImg = xImageLib::load(IMAGE_UNKNOWN, _XEVOL_ABSPATH_(L"./texture/test.dds") , (const int8*)NULL , (int)0 , PIXELFORMAT_R8G8B8A8 );


    m_PrimRayCb.init(m_pEngine);


    m_ParticleSystem = xParticleSystem::createInstance(m_pRenderer , m_pEngine->textureManager() );
    //m_pEmitter = m_ParticleSystem->createEmitter(L"xSphereEmitter");
    //m_pEmitter->init(L"xQuadParticlePool");
	//m_pEmitter->m_EmitterInfo.m_VelocityRange.setRange(xMathLib::xvec3(-20,-20,60.f) , xMathLib::xvec3(20.0f,20.0f,90.0f));
	//m_pEmitter->m_EmitterInfo.m_AcellRange.setRange(xMathLib::xvec3(0.0f , 0.0f , -9.80f) , xMathLib::xvec3(0.0f,0.0f , -9.80f));
	//m_pEmitter->m_EmitterInfo.m_EmitterFrequecy.setRange(10,50);
	//m_pEmitter->m_EmitterInfo.m_LifeRange.setRange(15.0f , 25.0f);

	//m_pBoxEmitter = m_ParticleSystem->createEmitter(L"xBoxEmitter");
	//m_pBoxEmitter->init(L"xQuadParticlePool");
	//m_pBoxEmitter->m_EmitterInfo.m_VelocityRange.setRange(xMathLib::xvec3(-2,-2,-5.f) , xMathLib::xvec3(2.0f,2.0f,-3.0f));
	//m_pBoxEmitter->m_EmitterInfo.m_AcellRange.setRange(xMathLib::xvec3(0.0f , 0.0f , -3.80f) , xMathLib::xvec3(0.0f,0.0f , -1.3f));
	//m_pBoxEmitter->m_EmitterInfo.m_EmitterFrequecy.setRange(1,15);
	//m_pBoxEmitter->m_EmitterInfo.m_LifeRange.setRange(15.0f , 18.0f);
	//m_pBoxEmitter->m_Info.m_PosArg.m_Value = xMathLib::xvec3(0.0f ,0.0f , 200.0f);

    xXmlDocument doc;
	doc.load( _XEVOL_ABSPATH_(L"./test.particle") );
	m_ParticleSystem->load(doc.root( ) );
	m_ParticleSystem->startAll();
	m_ParticleSystem->operator[](0)->stop();

    //doc.insertNode(L"emitters");
    //xXmlNode* pNode = doc.root()->insertNode(L"emitter");
    //m_pEmitter->save(pNode);
    //m_pEmitter->stop();

	//doc.root()->insertNode(L"emitter");
	//m_pBoxEmitter->save(pNode);
	//m_pBoxEmitter->start();
    //doc.save( _XEVOL_ABSPATH_(L"./testparticle.xml"));
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

void     CEvolEnviroment::drawOtherThing( xMathLib::xmat4 &mat, long passedTime, int nFrame) 
{
    static int   tPassed = 0;
    tPassed += passedTime;
    bool bDrawPlane = m_pEngine->sysValue(L"defualt",L"drawPlane" , false);
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
    swprintf(texInfo , L"光线追踪测试程序\nFPS=%f \nRenderApi=%s" , nFrame*1000.0f/tPassed ,m_pRenderApi->name() );

    m_InfoPanel->show();
    ds_wstring infoText = ds_wstring(texInfo) + L"\nMudus 3D Studio "; 
    m_InfoPanel->setText(infoText.c_str() );
    m_xuiWinMgr->draw();
}

void     CEvolEnviroment::updateFrame(long passedTime , ISceneVisitor* pVisitor , bool bDrawUI)
{
    static float angle = 0;
    static int   nFrame = 0;

   
    nFrame ++;
    angle += passedTime/1000.0f * 35.0f;
    //xSleep(15);
    m_pRenderApi->identityMatrix(MATRIXMODE_World);
    m_pRenderApi->applyCamera(m_pCamera);
	if(m_pRenderApi->renderMode() == eRenderMode::eRenderMode_Select )
		m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.0f,0.0f));
	else 
		m_pRenderApi->begin(xColor_4f(0.0f,0.3f,0.3f,0.0f));

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
        m_pScene->visit(&uv);
        m_pScene->visit(pVisitor);
    }
    this->m_pRenderer->end(passedTime);

    m_pRenderApi->setBlendState(NULL);
    m_pRenderApi->setRasterizerState(NULL);
    m_pRenderApi->identityMatrix(MATRIXMODE_World);
	m_ParticleSystem->update( passedTime/1000.0f);
	m_ParticleSystem->render( passedTime/1000.0f);
	//m_pEmitter->update( passedTime/1000.0f);
	//m_pBoxEmitter->update( passedTime/1000.0f );
	//m_pEmitter->render(passedTime / 1000.0f);
	//m_pBoxEmitter->render(passedTime / 1000.0f);
    if(bDrawUI ) drawOtherThing(mat, passedTime, nFrame);

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
    m_ProcedureTexture   = NULL;
    m_cameraStep =1.0f;

    m_pRenderer    = NULL;
    m_PrimaryBuffer  = NULL;

    return ;
}



int _tmain(int argc, _TCHAR* argv[])
{
    CEvolEnviroment app;
    xOperationSys::singleton()->init(argv[0]);


    xMathLib::xquat q1( xMathLib::xvec4(1.0,0.0f,0.0f,1.0) , -90.0f);
    xMathLib::xquat q2( xMathLib::xvec4(0.0,0.0f,1.0f,1.0) , 90.0f);
    xMathLib::xquat q3 = q2 * q1;

    xMathLib::xvec3 axis;
    float angle;
    q3.toAxis( axis , angle);

    xGeomLib::xBezier<xvec3> myBezier(2);
    myBezier[0] = xvec3(1.0f , 1.0f , 1.0f);
    myBezier[1] = xvec3(1.0f , 4.0f , 2.0f);

    xvec3 v1 = myBezier(0.0f);
    xvec3 v2 = myBezier(1.0f);

    app.init();
    app.initRenderer(NULL);
    //app.loadScene(_XEVOL_ABSPATH_(L".\\WorldComposer\\scene\\demoScene.xml"));
    app.loadScene(_XEVOL_ABSPATH_(L".\\raytracerScene.xml"));
    app.run();
    app.exit();
    IBaseObject::DumpObjectList();
    return 0;
}

