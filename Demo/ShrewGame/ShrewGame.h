// TestBase.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
using namespace XEvol3D;
#include "BattleLayer.h"
class CEvolEnviroment : public IApplication
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

	xSceneSelection*       GetSelection(){return &m_Selection;}
	void                   initRenderer(HWND hRenderWindow);
	bool                   init();
	void                   run();

    bool                   onLogicTick(long passedTime);
    bool                   onRenderTick(long passedTime);
    bool                   onRenderTick(long passedTime, ISceneVisitor* pVisitorr , bool bDrawUI = true);

	void                   drawOtherThing( xMathLib::xmat4 &mat, long passedTime, int nFrame) ;

	const wchar_t*         sysCfgfile();

	xObjPlacement*         scenePlacementOprator() { return m_pScenePlacementOprator ; }
	IRenderApi*            renderApi(){ return m_pRenderApi; }
	IBaseRenderer*         renderer() { return m_pRenderer ; }
	IRenderCamera*         camera(){ return m_pCamera; }
	IRenderCamera*         camera2D(){ return m_p2DCamera; }
	xBaseTextureMgr*       texMgr()    { return m_pEngine->textureManager(); }
	xBaseModelMgr*         modelMgr()  { return m_pEngine->modelMgr(); }
    ISceneGraph*           getCurrentScene();
    xvec2i*  GetSelectData(xTextureDesc& _desc , int x , int y);
    void     ReleaseSelectData(xvec2i* pRTData);

protected:
	xEvol3DEngine*              m_pEngine;
	IRenderApi*                 m_pRenderApi;

	IRenderCamera*              m_pCamera;
	IRenderCamera*              m_p2DCamera;

    IDepthStencilState*         m_pOverlayZState;

	float                       m_cameraStep;

    IBaseRenderer*              m_pRenderer ;
	std::ds_wstring             m_cfgFile;
	xui::xuiWindowManager*      m_xuiWinMgr;
	xui::xuiDemoInfoPannel*     m_InfoPanel;

	xObjPlacement*              m_pScenePlacementOprator;
	xSceneColorSelectVisitor*   m_ColorSelVisitor;
	xSceneRenderVisitor*        m_renderVisitor;
	xSceneSelection             m_Selection;
    

    CBattleLayer*                m_BattleLayer;

};
