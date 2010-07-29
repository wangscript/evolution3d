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

class MEDUSA_API ICameraOperator
{
public:
    virtual bool            onMessage(xWindowMsg& _msg) = 0;
    virtual const wchar_t*  cameraInfo() = 0;
    virtual bool            onActive() = 0;
    virtual bool            onDeActive() = 0;
    virtual void            setCamera( IRenderCamera* pCamera ) = 0;
    virtual const wchar_t*  name() = 0;
    virtual void            updateTick(float _tPassedTime) = 0;
};


class MEDUSA_API CEvolEnviroment;

class MEDUSA_API CAnaglyphStero
{
public:
    CAnaglyphStero();
    void enable(bool bFlag);
    bool bEnable();
    void uninit();
    void begin(IRenderApi* pRenderApi , IRenderCamera* pMainCamera );
    void render(CEvolEnviroment* pEnv , ISceneVisitor* pVisitor, long passedTime , IRenderCamera* pMainCamera );
    void end();
    float dist();
    void  setDist(float _dist);
    float maxDist();
protected:
    IRenderApi*                 m_pRenderApi;
    IDepthStencilState*         m_pStencilState;
    IRenderCamera*              m_pRightCamera;
    IRenderView*                m_LVBackBuffer;
    IRenderView*                m_RVBackBuffer;
    HGpuProgram                 m_hSteroProgram;
    HGpuProgram                 m_hLVProgram;
    HGpuProgram                 m_hRVProgram;
    bool                        m_bEnable;
    float                       m_CameraDist;
};

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
    bool                   getRenderWindowRenct(x2DRect& _rect);
	xSceneSelection*       GetSelection(){return &m_Selection;}
	void                   initRenderer(HWND hRenderWindow);
	bool                   init();
	void                   run();
	void                   updateFrame(long passedTime);
	void                   updateFrame(long passedTime , ISceneVisitor* pVisitor);

    

    const wchar_t*         sysCfgfile();
	ISceneGraph*           scene() { return m_pScene ; }
	const wchar_t*         sceneFile(){ return m_sceneFile.c_str() ; }
    std::ds_wstring        sceneDir();
	xObjPlacement*         scenePlacementOprator() { return m_pScenePlacementOprator ; }
	IRenderApi*            renderApi() { return m_pRenderApi; }
	IBaseRenderer*         renderer()  { return m_pRenderer ; }
	IRenderCamera*         camera()    { return m_pCamera; }
	IRenderCamera*         camera2D()  { return m_p2DCamera; }
	xBaseTextureMgr*       texMgr()    { return m_pEngine->textureManager(); }
	xBaseModelMgr*         modelMgr()  { return m_pEngine->modelMgr(); }
    xXmlNode*              GetTexturePaths(){ return m_TexturePaths;}
    xXmlNode*              GetResPaths()  { return m_ResPaths  ;}

	bool                   OnUpdateCommandUI(int ctrlID , int ctrlIdx, NS_MDED::CMEdUiToolBarInfo::CMEdUiCmdUi* pUi);
	bool                   OnCommand(int ctrlID , int ctrlIdx);
    ICameraOperator*       setCameraOperator(ICameraOperator* pCameraOperator);
    ICameraOperator*       getCameraOperator();
public:
	xvec2i*         GetSelectData2(xTextureDesc& _desc , int& x , int& y , int& w , int& h);
	xvec2i*         GetSelectData(xTextureDesc& _desc , const int x , const int y , const int w , const int h);
	xvec4           GetTerrainHeight(xTextureDesc& _desc , int x , int y , bool includeOther = false);
    IColorSelector* GetTerrainHeightSelector();
	void            ReleaseSelectData(xvec2i* pRTData);
    CAnaglyphStero* GetAnaglyphStero(){ return &m_AnaglyphStero ; }
    void             _renderFrame( long passedTime, ISceneVisitor* pVisitor ,IRenderCamera* pCamera);
protected:
	bool             __AfterCreateScene( ISceneGraph* pNewScene, const wchar_t* _Scene ) ;
    

protected:
	xEvol3DEngine*              m_pEngine;
	IRenderApi*                 m_pRenderApi;
	IDrawableHelper*            m_GridPlan;

	IRenderCamera*              m_pCamera;
	IRenderCamera*              m_p2DCamera;

	IDepthStencilState*         m_pStencilState;
	IRasterizerState*           m_DefRasterizer;

	IBaseRenderer*              m_pRenderer ;
	std::ds_wstring             m_cfgFile;
	xui::xuiWindowManager*      m_xuiWinMgr;
	xui::xuiDemoInfoPannel*     m_InfoPanel;
    xui::xuiCameraInfoPannel*   m_CameraPanel; 
	ISceneGraph*                m_pScene;
	std::ds_wstring             m_sceneFile;

	xObjPlacement*              m_pScenePlacementOprator;
	xSceneColorSelectVisitor*   m_ColorSelVisitor;
	xTerrainHeightSelector*     m_pTerrainHeightSelector;
	xTerrainHeightSelVisitor*   m_TerrainVisitor;
	xSceneRenderVisitor*        m_renderVisitor;
	NS_MDED::CMEdUiToolBarInfo  m_CameraViewToolbar;
	xSceneSelection             m_Selection;
    xXmlNode*                   m_TexturePaths;
    xXmlNode*                   m_ResPaths;
    HWND                        m_hRenderWindow;

    ICameraOperator*            m_pCameraOperator;
    ICameraOperator*            m_pDefCameraOperator;

    uint32                      m_nTotalFrame;
    uint32                      m_timePassed;
    CAnaglyphStero              m_AnaglyphStero;
};

