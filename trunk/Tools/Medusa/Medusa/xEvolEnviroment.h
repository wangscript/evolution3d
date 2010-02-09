// TestBase.cpp : Defines the entry point for the console application.
//
#pragma once
#include "./Operator/objPlacement.h"
#include "xEvol3D.h"
#include <BaseLib/xXmlDocument.h>
#include <BaseLib/xStringTable.h>
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
#include <RenderAPI/xTLEnvioroment.h>
#include <Renderer/xRenderer.h>
#include <Renderer/xModelElement.h>
#include "xui/xuiWindowManager.h"
#include <tchar.h>
#include <XMathLib/xMathlib.h>
#include <Image/xILImage.h>
#include <RenderAPI/xBaseTexture.h>
#include "xModel/xBaseModel.h"
#include "Renderer/xDrawableHelper.h"
#include "Image/xFreeImage.h"
#include "xScene/xSceneGraph.h"
#include "xui/xuiDemoInfoPannel.h"
#include "xScene/xSceneSimpleVistor.h"
#include "Operator/xTerrainHeightQuery.h"
using namespace XEvol3D;

class MEDUSA_API CEvolEnviroment : public IApplication  , public NS_MDED::CMEdUiToolBarInfo::MEdUIToolbarCallback
{
public:
	CEvolEnviroment();
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
	void                   initRenderer(HWND hRenderWindow);
	bool                   init();
	void                   run();
	void                   updateFrame(long passedTime);
	void                   updateFrame(long passedTime , ISceneVisitor* pVisitor);
	const wchar_t*         sysCfgfile();
	ISceneGraph*           scene() { return m_pScene ; }
	const wchar_t*         sceneFile(){ return m_sceneFile.c_str() ; }
	xObjPlacement*         scenePlacementOprator() { return m_pScenePlacementOprator ; }
	IRenderApi*            renderApi() { return m_pRenderApi; }
	IBaseRenderer*         renderer()  { return m_pRenderer ; }
	IRenderCamera*         camera()    { return m_pCamera; }
	IRenderCamera*         camera2D()  { return m_p2DCamera; }
	xBaseTextureMgr*       texMgr()    { return m_pEngine->textureManager(); }
	xBaseModelMgr*         modelMgr()  { return m_pEngine->modelMgr(); }

	NS_MDED::CMEdUiToolBarInfo::CommandUIStatus OnUpdateCommandUI(int ctrlID , int ctrlIdx);
	bool                                        OnCommand(int ctrlID , int ctrlIdx);
public:
	xvec2i*  GetSelectData(xTextureDesc& _desc , int x , int y);
	xvec4    GetTerrainHeight(xTextureDesc& _desc , int x , int y);
	void     ReleaseSelectData(xvec2i* pRTData);

protected:
	bool __AfterCreateScene( ISceneGraph* pNewScene, const wchar_t* _Scene ) ;

protected:
	xEvol3DEngine*              m_pEngine;
	IRenderApi*                 m_pRenderApi;
	IDrawableHelper*            m_GridPlan;

	IRenderCamera*              m_pCamera;
	IRenderCamera*              m_p2DCamera;
	HGpuProgram                 m_hProgram; 

	IDepthStencilState*         m_pStencilState;
	IRasterizerState*           m_DefRasterizer;

	IBaseTexture*               m_ProcedureTexture;
	HFontRender                 m_hFont;
	float                       m_cameraStep;
	IRenderView*                m_pSelView;

	IBaseRenderer*              m_pRenderer ;
	std::ds_wstring             m_cfgFile;
	xui::xuiWindowManager*      m_xuiWinMgr;
	xui::xuiDemoInfoPannel*     m_InfoPanel;
	ISceneGraph*                m_pScene;
	std::ds_wstring             m_sceneFile;

	xObjPlacement*              m_pScenePlacementOprator;
	xSceneColorSelectVisitor*   m_ColorSelVisitor;
	xTerrainHeightSelector*     m_pTerrainHeightSelector;
	xTerrainHeightSelVisitor*   m_TerrainVisitor;
	xSceneRenderVisitor*        m_renderVisitor;
	NS_MDED::CMEdUiToolBarInfo  m_CameraViewToolbar;
	xSceneSelection             m_Selection;
};

