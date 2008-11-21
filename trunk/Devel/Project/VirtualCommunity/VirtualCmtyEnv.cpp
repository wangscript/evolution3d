#include "stdafx.h"
#include "VirtualCmtyEnv.h"
namespace VirtualCommunity
{

	xVirtualCmtyEnv::xVirtualCmtyEnv()
	{
		m_bEditorMode = true;
	}

	xVirtualCmtyEnv::~xVirtualCmtyEnv()
	{

	}

	bool xVirtualCmtyEnv::bEditorMode()
	{
		return m_bEditorMode;
	}
	bool  xVirtualCmtyEnv::OnResize(int w , int h)
	{
		m_Evol3D->ResizeRenderWindow(w , h);
		m_RenderApi->onResize(w,h);
		m_3DCamera->aspect = w/float(h);
		m_2DCamera->setPerspective2D(w,h,45.0f,true);
		return true;
	}

	bool xVirtualCmtyEnv::Init(int hWnd)
	{
		m_Evol3D = gcnew xEvol3D::xEvol3DEnv;
		m_Evol3D->Init(m_Evol3D->absPath(L".\\system.xml"),0, hWnd ,m_Evol3D->absPath(L".\\") );
		m_RenderApi = m_Evol3D->createRenderApi();
		m_LogFont  = m_RenderApi->findFont(L"small");
		m_2DCamera = m_RenderApi->createCamera(L"2DCamera");
		m_3DCamera = m_RenderApi->createCamera(L"3DCamera");

		m_3DCamera->EyePos    = gcnew xVector(100.0f, 0.0f , 50.0f , 1.0f);
		m_3DCamera->EyeTarget = gcnew xVector(0.0f  , 0.0f , 0.0f , 1.0f);
		m_3DCamera->EyeUp     = gcnew xVector(0.0f, 0.0f, 1.0f, 1.0f);
		m_3DCamera->fov = 45.0f;
		m_3DCamera->CameraStep = 1.0f;

		m_3DCamera->nearPlan = 0.1f;
		m_3DCamera->farPlan = 10000.0f;
		m_3DCamera->projType = eCameraProject::PT_PROJECT;
		m_3DCamera->aspect = 4.0f/3.0f;
		m_2DCamera->setPerspective2D(400,300,45.0f,true);


		m_TextureManager = gcnew xBaseTextureManager(m_RenderApi , L"VRCTexManager" , false);
		m_TextureManager->addPath( m_Evol3D->absPath(L".\\texture\\"));
		m_TextureManager->addPath( m_Evol3D->absPath(L".\\vrcommunity\\"));
		m_TextureManager->addPath( m_Evol3D->absPath(L".\\vrcommunity\\texure\\"));
		m_GridPlane = m_Evol3D->createHelperDrawable(L"PlaneGrid");
        m_GridPlane->init(m_TextureManager);
		return true;
	}

	bool xVirtualCmtyEnv::Unit()
	{
		return false;
	}

	bool xVirtualCmtyEnv::OnIdle()
	{
		m_RenderApi->begin(0.0f, 0.0f, 0.2f, 1.0f, 1.0f, 0);
		m_RenderApi->beginScene();
		m_RenderApi->applyCamera(m_3DCamera);
        //绘制地表
		xMatrix^ gridPlaneMat = gcnew xMatrix();
		xMathlib::Scale(gridPlaneMat , 200.0f , 200.0f ,200.0f);
		m_RenderApi->pushMatrix(eMatrixMode::MATRIXMODE_World);
		m_RenderApi->WorldMatrix = gridPlaneMat;
		m_GridPlane->begin();
		m_GridPlane->render();
		m_GridPlane->end();
		m_RenderApi->popMatrix(eMatrixMode::MATRIXMODE_World);
   

		//绘制2D Component
		m_RenderApi->identityMatrix(eMatrixMode::MATRIXMODE_World);
		m_RenderApi->applyCamera(m_2DCamera);
		xColorF^ fontCl = gcnew xColorF(1.0f,1.0f,1.0f,1.0f);
		m_LogFont->drawText(L"虚拟社区v1.0" ,10.0f,10.0f,fontCl);
		m_RenderApi->endScene();
		m_RenderApi->swapBuffer();
		return true;
	}
}