// TestBase.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include "ShrewGameLayer.h"

using namespace XEvol3D;


class CBattleLayer : public CShrewGameLayer
{
public:
    CBattleLayer();
    bool uninit();
    bool init(xEvol3DEngine* pEngine , IRenderCamera* pCamera , IBaseRenderer* pRenderer);
    bool onHandle();
    bool onUnhandle();
    bool updateFrame(long timePassed );
    bool preHandle();
    bool preUnhandle();
    bool postHandle();
    bool postUnhandle(); 
    bool prevUpdateFrame(long timePassed = 0);
    bool postUpdateFrame(long timePassed = 0);    
    bool onLogicTick(long passedTime);
    bool createScene(const wchar_t* _SceneType);
    bool loadScene(const wchar_t* _Scene);
    bool onRenderTick(long passedTime, ISceneVisitor* pVisitor);

    const wchar_t* name();
    ISceneGraph*   scene();
    const wchar_t* sceneFile();
    bool           onMessage(xWindowMsg& msg);
protected:
    bool __AfterCreateScene( ISceneGraph* pNewScene, const wchar_t* _Scene ) ;
protected:
    xEvol3DEngine*              m_pEngine;
    IRenderApi*                 m_pRenderApi;
    IRenderCamera*              m_pCamera;
    float                       m_cameraStep;
    IBaseRenderer*              m_pRenderer ;
    ISceneGraph*                m_pScene;
    std::ds_wstring             m_sceneFile;
    xParticleSystem*            m_ParticleSystem;
    IDepthStencilState*         m_pStencilState;
    IRasterizerState*           m_DefRasterizer;
};
