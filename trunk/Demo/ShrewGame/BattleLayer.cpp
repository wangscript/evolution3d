// TestBase.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "BattleLayer.h"
#include "working/xWorkingThread.h"
#include "XMathLib/xspline.h"

using namespace XEvol3D;
CBattleLayer::CBattleLayer()
{
    m_cameraStep = 4.0f;
    m_pEngine = NULL;
    m_pRenderApi = NULL;
    m_pCamera = NULL;
    m_pRenderer = NULL;
    m_pScene = NULL;
    m_sceneFile = L"";
    m_ParticleSystem = NULL;
    m_pStencilState = NULL;
    m_DefRasterizer = NULL;

}

bool CBattleLayer::uninit()
{
    if(m_pScene)
    {
        m_pScene->unload();
        m_pScene->ReleaseObject();
    }
    return true;
}

bool CBattleLayer::onMessage(xWindowMsg& msg)
{
    if(msg.MsgID == WIN_KEYDOWN)
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
    return false;
}
bool CBattleLayer::init(xEvol3DEngine* pEngine , IRenderCamera* pCamera , IBaseRenderer* pRenderer)
{
    m_pEngine = pEngine;
    m_pRenderApi = pEngine->renderApi();
    m_pRenderer  = pRenderer;
    m_pStencilState = m_pRenderApi->createDepthStencilState(L"Overlay");
    m_pRenderApi->setDepthStencilState(m_pStencilState);

    m_DefRasterizer= m_pRenderApi->createRasterizerState(L"Default");
    m_pRenderApi->setRasterizerState(m_DefRasterizer);

    m_pCamera = pCamera;

    //设置渲染器的渲染Tech.
    m_pRenderer->setRenderTech(L"Basic");

    loadScene(_XEVOL_ABSPATH_(L".\\ShrewGame\\ShrewGameScene.xml"));

    m_ParticleSystem = xParticleSystem::createInstance(m_pRenderer , m_pEngine->textureManager() );
    xXmlDocument doc;
    doc.load( _XEVOL_ABSPATH_(L"./ShrewGame/scene.particle") );
    m_ParticleSystem->load(doc.root( ) );
    m_ParticleSystem->startAll();
    m_ParticleSystem->operator[](0)->stop();
    return true;
}

bool CBattleLayer::onHandle()
{
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


    //设置摄像机的值
    m_pCamera->m_Eye.m_Up        = xMathLib::xvec4(0.0f   , 0.0f    , 1.0f   , 1.0f);
    m_pCamera->m_Eye.m_EyePos    = xMathLib::xvec4(340.0f , -130.0f , 135.0f , 1.0f);
    m_pCamera->m_Eye.m_EyeTarget = xMathLib::xvec4(340.0f ,  256.0f , 90.0f , 1.0f);
    m_pCamera->dirty();
    return true;
}

bool CBattleLayer::onUnhandle()
{
    return true;
}

bool CBattleLayer::updateFrame(long timePassed )
{
    return true;
}

bool CBattleLayer::preHandle()
{
    return true;
}

bool CBattleLayer::preUnhandle()
{
    return true;
}

bool CBattleLayer::postHandle()
{
    return true;
}	

bool CBattleLayer::postUnhandle()
{
    return true;
}

bool CBattleLayer::prevUpdateFrame(long timePassed )
{
    return true;
}

bool CBattleLayer::postUpdateFrame(long timePassed)
{
    return true;
}

const wchar_t* CBattleLayer::name()
{
    return L"CBattleLayer";
}

bool CBattleLayer::onLogicTick(long passedTime)
{
    return true;
}

ISceneGraph* CBattleLayer::scene()
{
    return m_pScene ; 
}

const wchar_t* CBattleLayer::sceneFile()
{ 
    return m_sceneFile.c_str() ; 
}

bool CBattleLayer::createScene(const wchar_t* _SceneType)
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

bool CBattleLayer::loadScene(const wchar_t* _Scene)
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

bool CBattleLayer::__AfterCreateScene( ISceneGraph* pNewScene, const wchar_t* _Scene ) 
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

bool CBattleLayer::onRenderTick(long passedTime, ISceneVisitor* pVisitor)
{
    m_pRenderer->begin(passedTime);
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
    return true;
}



