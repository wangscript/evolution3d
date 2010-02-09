// TestBase.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <BaseLib/xXmlDocument.h>
#include <BaseLib/xStringTable.h>
#include <BaseLib/xLogger.h>
#include "Image/xImage.h"
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
#include "xScene/xSceneSimpleVistor.h"
#include "objPlacement.h"
#include "xuiDemoInfoPannel.h"
#include "xEvol3D.h"

using namespace XEvol3D;

struct xRayTraceNodeData
{
    xvec4 m_Pos;
    xvec4 m_Tan;
    xvec4 m_Reflect;
    xvec4 m_Normal;
    int   m_Level;
};

class GetColor
{
public:
    float operator ()(float v)
    {
        if(v > 1.0f )
            return 1.0f;
        if(v < 0.0f )
            return 0.0f;
        return v;
    }
};

#define  MAX_TRACE_LEVEL 1
enum
{
    COL_VIEW     = 0,
    POS_VIEW     ,
    NOR_VIEW     ,
    REFLECT_VIEW ,
    TANGENT_VIEW ,

    MAX_RT_VIEW  ,
};

class CRaytracerPrimaryRenderCallback : public IRendererEventCallback
{
public:
	bool preDrawPass(IBaseRenderer* pSender , xRenderPass* pPass );
	bool postDrawPass(IBaseRenderer* pSender , xRenderPass* pPass );
	void init(xEvol3DEngine* pEngine);
	void uninit();
protected:
	HBaseShader m_hLightShader;
    HBaseShader m_hObjectShader;
};
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
	bool                   createScene(const wchar_t* _SceneType);
	bool                   loadScene(const wchar_t* _Scene);

	xSceneSelection*       GetSelection(){return &m_Selection;}
	void                   initRenderer(HWND hRenderWindow);
	bool                   init();
	void                   run();
	void                   updateFrame(long passedTime);
	void                   updateFrame(long passedTime , ISceneVisitor* pVisitorr , bool bDrawUI = true);
	void                   drawOtherThing( xMathLib::xmat4 &mat, long passedTime, int nFrame) ;

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

	void                   DoTrace();
    xvec4                  CaclBuffer( HWND hRenderWindow, IRenderView* pRenderView , int level) ;
    xvec4                  TraceRay(xRayTraceNodeData& data); 
public:
	xvec2i*  GetSelectData(xTextureDesc& _desc , int x , int y);
	void     ReleaseSelectData(xvec2i* pRTData);

protected:
	bool __AfterCreateScene( ISceneGraph* pNewScene, const wchar_t* _Scene ) ;

protected:
	xEvol3DEngine*              m_pEngine;
	IRenderApi*                 m_pRenderApi;
	IDrawableHelper*            m_GridPlan;
	IRenderView*                m_PrimaryBuffer;//第一条光线的Buffer;
	std::vector<IRenderView*>   m_TraceView;

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
	xSceneRenderVisitor*        m_renderVisitor;
	xSceneSelection             m_Selection;
	CRaytracerPrimaryRenderCallback m_PrimRayCb;
};
