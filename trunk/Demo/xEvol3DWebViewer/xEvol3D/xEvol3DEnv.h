// TestBase.cpp : Defines the entry point for the console application.
//
#pragma once
#include "objPlacement.h"
#include "xuiDemoInfoPannel.h"
#include "xEvol3D.h"

using namespace XEvol3D;
BEGIN_NAMESPACE_XEVOL3D
class  CEvolEnviroment : public IApplication
{
public:
	CEvolEnviroment();
    ~CEvolEnviroment();
	bool                   onResize();
	xEvol3DEngine*         engine(){return m_pEngine; }
	void                   topCamera(xGeomLib::xaabb& aabox);
	xvec3                  getModelCeneter(xBaseModel* pModel);
	void                   frontCamera(xGeomLib::xaabb& aabox);
	void                   exit();
	xvec4                  getMouseMoveDir(int dx , int dy , float& movPercent);
	bool                   onMessage(xWindowMsg& msg);
	bool                   createScene(const wchar_t* _SceneType);
	bool                   loadScene(const wchar_t* _Scene);
	xSceneSelection*       GetSelection(){return &m_Selection;}
	void                   initRenderer(HWND hRenderWindow , HWND hParentWnd);
	bool                   init();
	void                   run();
	void                   updateFrame(long passedTime);
	void                   updateFrame(long passedTime , ISceneVisitor* pVisitor);
	const wchar_t*         sysCfgfile();
	ISceneGraph*           scene() { return m_pScene ; }
	const wchar_t*         sceneFile(){ return m_sceneFile.c_str() ; }
	xObjPlacement*         scenePlacementOprator() { return m_pScenePlacementOprator ; }
	IRenderApi*            renderApi(){ return m_pRenderApi; }
	IBaseRenderer*         renderer() { return m_pRenderer ; }
	IRenderCamera*         camera(){ return m_pCamera; }
	IRenderCamera*         camera2D(){ return m_p2DCamera; }
	xBaseTextureMgr*       texMgr()    { return m_pEngine->textureManager(); }
	xBaseModelMgr*         modelMgr()  { return m_pEngine->modelMgr(); }
    void                   lock() { if (m_pLocker) m_pLocker->lock() ; }
    void                   unlock(){ if( m_pLocker ) m_pLocker->unlock() ; }

    void                   setInfoText(const wchar_t* _info) { m_textInfo = _info ; }

public:
	xvec2i*  GetSelectData(xTextureDesc& _desc , int x , int y);
	void     ReleaseSelectData(xvec2i* pRTData);

protected:
	bool __AfterCreateScene( ISceneGraph* pNewScene, const wchar_t* _Scene ) ;

protected:
	xEvol3DEngine*              m_pEngine;
	IRenderApi*                 m_pRenderApi;

	IRenderCamera*              m_pCamera;
	IRenderCamera*              m_p2DCamera;
	HGpuProgram                 m_hProgram; 

	IDepthStencilState*         m_pStencilState;
	IRasterizerState*           m_DefRasterizer;

	IBaseTexture*               m_ProcedureTexture;
	HFontRender                 m_hFont;
	float                       m_cameraStep;

	IBaseRenderer*              m_pRenderer ;
	std::ds_wstring             m_cfgFile;
	xui::xuiWindowManager*      m_xuiWinMgr;
	xui::xuiDemoInfoPannel*     m_InfoPanel;
	ISceneGraph*                m_pScene;
	std::ds_wstring             m_sceneFile;

	xObjPlacement*              m_pScenePlacementOprator;
	xSceneColorSelectVisitor*   m_selVistor;
	xSceneRenderVisitor*        m_renderVisitor;
	xSceneSelection             m_Selection;
    float                       m_fCameraDist;
    

    std::wstring                m_textInfo;
    xThreadLocker*              m_pLocker;
};
END_NAMESPACE_XEVOL3D