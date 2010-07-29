// TestBase.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ShrewGame.h"
#include "working/xWorkingThread.h"
#include "XMathLib/xspline.h"

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

    m_pCamera->m_Eye.m_EyePos    = xvec4(cen.x + 10 * len.x ,cen.y , cen.z );
    m_pCamera->m_Eye.m_EyeTarget = xvec4(cen.x         ,cen.y , cen.z);
    m_pCamera->m_Eye.m_Up        = xvec4(0.0f,0.0f,1.0f);
    m_cameraStep = len.x/10.0f;
    m_pCamera->setFarPlan(len.x * 150.0f);
    m_pCamera->setNearPlan(0.1f);
}

void     CEvolEnviroment::exit()
{
    m_BattleLayer->uninit();
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

    onRenderTick(0 , m_ColorSelVisitor);
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
    if(IApplication::onMessage(msg))
        return true;

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
    m_pScenePlacementOprator->onMessage(msg);
    return false;
}

const wchar_t*     CEvolEnviroment::sysCfgfile()
{
    return m_cfgFile.c_str() ; 
}



bool    CEvolEnviroment::init()
{
    wchar_t appname[512] = {0};
    GetModuleFileName(NULL , appname , 512);
    xOperationSys::singleton()->init( appname );
    xStringTable::singleton()->addPath(_XEVOL_ABSPATH_(L"Language/"));
    xStringTable::singleton()->setLanguage(NULL);

    m_pEngine = new xEvol3DEngine;
    m_cfgFile = _XEVOL_ABSPATH_(L"./ShrewGame/ShrewGame.xml");
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

    


    //m_Model = m_pModelMgr->add(L"einxin.xrm" , true , 0);
    xGeomLib::xaabb aabb;
    aabb.m_Max = xvec3(20,20,20);
    aabb.m_Min = xvec3(-20, -20, -20);
    frontCamera( aabb );


    m_xuiWinMgr = xui::xuiWindowManager::createManager(m_pRenderApi , m_pEngine->textureManager() );
    m_xuiWinMgr->getXMLManager()->addPath(_XEVOL_ABSPATH_(L"./ShrewGame/xui/"));
    m_InfoPanel  = XUI_CREATE_DIALOG(xuiDemoInfoPannel,m_xuiWinMgr);


    m_pOverlayZState = m_pRenderApi->createDepthStencilState(L"Overlay");

    //设置渲染器的渲染Tech.
    m_pRenderer->setRenderTech(L"Basic");

    m_ColorSelVisitor     = new xSceneColorSelectVisitor(m_pRenderer , m_pCamera);
    m_renderVisitor = new xSceneRenderVisitor(m_pRenderer , m_pCamera);

    //初始化操作器
    m_Selection.init(this);

    m_pScenePlacementOprator = new xObjPlacement( & this->m_Selection );
    m_pScenePlacementOprator->init(this);
    m_pScenePlacementOprator->SetMoveOperator();


    m_BattleLayer = new CBattleLayer;
    m_BattleLayer->init( m_pEngine , m_pCamera , m_pRenderer );



    return ;
}

void     CEvolEnviroment::run()
{
    handleAppLayer(m_BattleLayer);
    this->setApplicationState(IApplication::AS_RUN);
    m_pEngine->messageLoop();
}


void     CEvolEnviroment::drawOtherThing( xMathLib::xmat4 &mat, long passedTime, int nFrame) 
{
    static int   tPassed = 0;
    tPassed += passedTime;

    m_pRenderApi->applyCamera(m_p2DCamera);
    m_pRenderApi->setBlendState( m_pRenderApi->createBlendState(L"AlphaBlend") );
    //===================
    m_pRenderApi->identityMatrix(MATRIXMODE_World);
    m_pRenderApi->setDepthStencilState(m_pOverlayZState);
    xvec4     destRect(20,300,400,300);
    xvec4     srcRect(0,0,256,256);
    xColor_4f cl(1.0f,1.0f,1.0f,1.0f);

    wchar_t texInfo[1024] = {0};
    swprintf(texInfo , L"地鼠物语测试程序\nFPS=%f \nRenderApi=%s" , nFrame*1000.0f/tPassed ,m_pRenderApi->name() );

    m_InfoPanel->show();
    ds_wstring infoText = ds_wstring(texInfo) + L"\nEvolution3D "; 
    m_InfoPanel->setText(infoText.c_str() );
    m_xuiWinMgr->draw();
}


bool CEvolEnviroment::onLogicTick(long passedTime)
{
    return IApplication::onLogicTick(passedTime);
}

bool CEvolEnviroment::onRenderTick(long passedTime)
{
    return onRenderTick(passedTime , m_renderVisitor);
}
bool CEvolEnviroment::onRenderTick(long passedTime, ISceneVisitor* pVisitor , bool bDrawUI)
{
    static float angle = 0;
    static int   nFrame = 0;


    nFrame ++;
    angle += passedTime/1000.0f * 35.0f;
    //xSleep(15);
    m_pRenderApi->identityMatrix(MATRIXMODE_World);
    m_pRenderApi->applyCamera(m_pCamera);
    if(m_pRenderApi->renderMode() == eRenderMode_Select )
        m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.0f,0.0f));
    else 
        m_pRenderApi->begin(xColor_4f(0.0f,0.3f,0.3f,0.0f));

    m_pRenderApi->beginScene();
    xMathLib::xmat4 mat;
    //xMathLib::XM_RotateY(mat,angle);

    //绘制模型
    m_pRenderApi->colorSelector()->setRenderObjectID(1,0);
    m_pRenderApi->identityMatrix(MATRIXMODE_World);

    
    //m_pModelEffect->draw( m_pModelElement );

    if(m_pCurAppLayer)
    {
        ((CShrewGameLayer*)m_pCurAppLayer)->onRenderTick(passedTime , pVisitor);
    }

    

    if(bDrawUI ) drawOtherThing(mat, passedTime, nFrame);

    //===================
    m_pRenderApi->endScene();
    m_pRenderApi->swapBuffer();         
    return true;
}

ISceneGraph* CEvolEnviroment::getCurrentScene()
{
    if(m_pCurAppLayer)
    {
        return ((CShrewGameLayer*)m_pCurAppLayer)->scene();
    }
    return NULL;
}

CEvolEnviroment::CEvolEnviroment()
{
    m_pEngine   = NULL;
    m_pRenderApi   = NULL;

    m_pCamera   = NULL;
    m_pOverlayZState   = NULL;
    m_cameraStep =1.0f;
    m_pRenderer    = NULL;

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
    app.run();
    app.exit();
    IBaseObject::DumpObjectList();
    return 0;
}

