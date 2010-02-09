// TestBase.cpp : Defines the entry point for the console application.
//
#include <BaseLib/xXmlDocument.h>
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

#include "xEvol3D.h"
using namespace XEvol3D;

class xEditorPreviewApp : public IApplication
{
	xEvol3DEngine*      m_pEngine;
	IRenderApi*         m_pRenderApi;
	IDrawableHelper*    m_ArcBall;
	IDrawableHelper*    m_GridPlan;

	IRenderCamera*      m_pCamera;
	HGpuProgram         m_hProgram; 
	IDepthStencilState* m_pStencilState;
	IRasterizerState*   m_DefRasterizer;
	IBaseTexture*       m_ProcedureTexture;
	HFontRender         m_hFont;
	HBaseModel          m_Model;
	xBaseTextureMgr*    m_pTexMgr;
	xBaseModelMgr*      m_pModelMgr;
	float               m_cameraStep;

	IRenderView*        m_pSelView;
	bool                m_bInDrag;
	xvec2i              m_lastDragPos;
	xvec4               m_ModelPos;
	xmat4               m_ModelRotate;
	int                 m_RotateAxis;

	IBaseRenderer*      m_pRenderer ;
	xRenderPass*        m_pModelPass;

	xui::xuiWindowManager*   m_xuiWinMgr;
public:
	xEditorPreviewApp();
	bool   onResize();
	void   topCamera(xGeomLib::xaabb& aabox);
	xvec3  getModelCeneter(xBaseModel* pModel);
	void   frontCamera(xGeomLib::xaabb& aabox);
	void   exit();
	xvec4  getMouseMoveDir(int x , int y , float& movPercent);
	bool   onMessage(xWindowMsg& msg);
	void   init(HWND hRenderWindow);
	void   run();
	void   updateFrame(long passedTime);
};

