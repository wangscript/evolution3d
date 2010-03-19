#include "EditorRenderPreview.h"
using namespace XEvol3D;

bool   xEditorPreviewApp::onResize()
{
	x2DRect _rect;
	xTextureDesc _desc;
	m_pRenderApi->onResize(0 , 0 );
	m_pRenderApi->getRenderView()->desc(_desc);
	m_pCamera->setAspect( float(_desc.m_width)/(_desc.m_height) );
	return true;
}

void     xEditorPreviewApp::topCamera(xGeomLib::xaabb& aabox)
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

xvec3   xEditorPreviewApp::getModelCeneter(xBaseModel* pModel)
{
	xGeomLib::xaabb& aabox = pModel->boundBox();
	xvec3 cen = 0.5f * (aabox.m_Max + aabox.m_Min);
	return cen;
}

void     xEditorPreviewApp::frontCamera(xGeomLib::xaabb& aabox)
{
	xvec3 cen = 0.5f * (aabox.m_Max + aabox.m_Min);
	xvec3 len =        (aabox.m_Max - aabox.m_Min);

	m_pCamera->m_Eye.m_EyePos    = xvec4(cen.x + 10 * len.x ,cen.y , cen.z );
	m_pCamera->m_Eye.m_EyeTarget = xvec4(cen.x         ,cen.y , cen.z);
	m_pCamera->m_Eye.m_Up        = xvec4(0.0f,0.0f,1.0f);
	m_cameraStep = len.x/10.0f;
	m_pCamera->setFarPlan(len.x * 15.0f);
	m_pCamera->setNearPlan(0.1f);
}

void     xEditorPreviewApp::exit()
{
	m_hProgram.setNULL();
	m_hFont.setNULL();
	m_Model.setNULL();
	m_pModelMgr->clear();
	delete m_pModelMgr;
	m_pEngine->exit();
}
xvec4  xEditorPreviewApp::getMouseMoveDir(int x , int y , float& movPercent)
{
	float dx = x - m_lastDragPos.x;
	float dy = y - m_lastDragPos.y;
	xTextureDesc _desc;
	m_pRenderApi->getRenderView()->desc(_desc);

	movPercent = sqrt((float)dx * dx  + dy * dy)/sqrt((float)_desc.m_width * _desc.m_width + _desc.m_height * _desc.m_height );
	xvec4 up , left , dir;
	m_pCamera->getLUD(left , up , dir);
	xvec4 _distDir = m_ModelPos - m_pCamera->m_Eye.m_EyePos;
	_distDir.w = 0.0f;
	float _dist = _distDir.len();
	float tanFov2 = XM_SinD(m_pCamera->getFOV()/2.0f ) / XM_CosD( m_pCamera->getFOV()/2.0f );

	float _screen_dist = _desc.m_height /2.0f / tanFov2; 
	float _maxH = tanFov2 * _dist * 2.0f;
	float _maxW = m_pCamera->getAspect() * _maxH ;


	float facX =  dx / float(_desc.m_width)   * _maxW;
	float facY = -dy / float(_desc.m_height)  * _maxH;

	xvec4 movDir = facY * up + facX * left;

	float s = _dist / _screen_dist;
	xvec4 movDir2 = dx * s * left  - dy  * s * up;
	return movDir2;
}
bool   xEditorPreviewApp::onMessage(xWindowMsg& msg)
{
	if(m_xuiWinMgr == NULL || m_pRenderApi == NULL)
		return true;

	m_xuiWinMgr->dispatchMsg(msg);
	if(msg.MsgID == WIN_LMOUSE_DOWN)
	{
		m_pRenderApi->lock();
		m_pRenderApi->colorSelector()->setRenderObjectID(0xcdef0a74 , 0x12345678 );
		m_pRenderApi->beginSelectMode();
		xTextureDesc _desc;
		m_pRenderApi->getRenderView()->desc(_desc);
		xvec2i * pRTData = new xvec2i[_desc.m_width * _desc.m_height];
		int x = msg.Mouse.x;
		int y = msg.Mouse.y;
		if( x < 0  || x >= _desc.m_width ||
			y < 0  || y >= _desc.m_height )
		{
			m_pRenderApi->endSelectMode(0 , 0 , _desc.m_width, _desc.m_height , pRTData );
			m_pRenderApi->unlock();
			return true;
		}

		updateFrame(0);
		m_pRenderApi->endSelectMode(0 , 0 , _desc.m_width, _desc.m_height , pRTData );
		m_pRenderApi->unlock();

		xvec2i& val = pRTData[ y * _desc.m_width + x];

		if(val.x == 1)
		{
			m_bInDrag     = true;
			m_lastDragPos = xvec2i(x , y);
		}
		else
		{
			for(int _i = -3 ; _i < 3 ; _i ++)
			{
				for(int _j = -3 ; _j < 3 ; _j ++)
				{
					int _x = x + _i;
					int _y = y + _j;
					if(_x >=0 && _x < _desc.m_width && _y >= 0 && _y < _desc.m_height)
					{
						xvec2i& val = pRTData[ _y * _desc.m_width + _x];
						if(val.x == 100)
						{ 
							m_RotateAxis = val.y;
							m_lastDragPos = xvec2i(x , y);
							break;
						}
					}		
				}
			}

		}

		return true;
		//IRenderTarget* pRenderTarget = (IRenderTarget*)m_pSelView->renderTarget(0);
		//pRenderTarget->grabRenderTagetData(0 , 0 , 800 , 600 , pRTData);
		//xFreeImage image;
		//image.data((const char*)pRTData,800 , 600 , PIXELFORMAT_R8G8B8A8);
		//image.flipRGBA(4);
		//image.save(_XEVOL_ABSPATH_(L"rendertarget.tga"));
	}
	if(msg.MsgID == WIN_MOUSE_MOVE)
	{
		int x = msg.Mouse.x;
		int y = msg.Mouse.y;
		if(m_bInDrag)
		{
			float movPercent;
			xvec4 movDir = getMouseMoveDir(x , y , movPercent);
			m_ModelPos = m_ModelPos + movDir;
		}
		else if(m_RotateAxis >= 0 )
		{
			float movPercent;
			xvec4 xAxis = xvec4(1.0f,0.0f,0.0f,1.0f);
			xvec4 yAxis = xvec4(0.0f,1.0f,0.0f,1.0f);
			xvec4 zAxis = xvec4(0.0f,0.0f,1.0f,1.0f);
			xvec4 movDir = getMouseMoveDir(x , y , movPercent);
			movDir.normalize();

			if(m_RotateAxis == 0)//XY
			{
				xvec4 RotAxis = zAxis;
				xvec4 rotateDir = movDir.cp(RotAxis);
				xvec4 cpAxis = yAxis + xAxis;
				float factor = rotateDir.dp3(cpAxis);
				float angle = factor * 90.0f * movPercent;
				xmat4 rotMat ;
				XM_RotateZ(rotMat , angle);
				xmat4 mat = m_ModelRotate * rotMat;
				m_ModelRotate = mat;
			}
			if(m_RotateAxis == 1)//YZ
			{
				xvec4 RotAxis = xAxis;
				xvec4 rotateDir = movDir.cp(RotAxis);
				xvec4 cpAxis = yAxis + zAxis;
				float factor = rotateDir.dp3(cpAxis);
				float angle = factor * 90.0f * movPercent;
				xmat4 rotMat ;
				XM_RotateX(rotMat , angle);
				xmat4 mat = m_ModelRotate * rotMat;
				m_ModelRotate = mat;
			}
			if(m_RotateAxis == 2)//XZ
			{
				xvec4 RotAxis = yAxis;
				xvec4 rotateDir = movDir.cp(RotAxis);
				xvec4 cpAxis = xAxis + zAxis;
				float factor = rotateDir.dp3(cpAxis);
				float angle = factor * 90.0f * movPercent * 4.0f;
				xmat4 rotMat ;
				XM_RotateY(rotMat , angle);
				xmat4 mat = m_ModelRotate * rotMat;
				m_ModelRotate = mat;
			}

		}
		m_lastDragPos = xvec2i(x , y);
		return true;
	}
	if(msg.MsgID == WIN_LMOUSE_RELEASE)
	{
		int x = msg.Mouse.x;
		int y = msg.Mouse.y;
		m_bInDrag = false;
		m_RotateAxis = -1;
		m_lastDragPos = xvec2i(x , y);
		return true;
	}
	if(msg.MsgID == WIN_RESIZE)
	{
		onResize();
		return true;
	}
	else if(msg.MsgID == WIN_KEYUP)
	{
		if(msg.Keyboard.cKeyChar >= '1' && msg.Keyboard.cKeyChar <= '9')
		{
			m_Model->setAction( msg.Keyboard.cKeyChar - '1' );
		}
		return true;
	}
	else if(msg.MsgID == WIN_KEYDOWN)
	{
		if(msg.Keyboard.cKeyChar == 'f' )
		{
			frontCamera(m_Model->boundBox() );
		}
		else if(msg.Keyboard.cKeyChar == 't' )
		{
			topCamera(m_Model->boundBox() );
		}
		else if(msg.Keyboard.cKeyChar == 'a' )
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

void     xEditorPreviewApp::init(HWND hRenderWindow)
{
	m_ModelRotate.identity();
	m_ModelPos    = xvec4(0.0f,0.0f,0.0f,1.0f);
	m_lastDragPos = xvec2i(0,0);
	m_bInDrag     = false;
	m_RotateAxis  = -1;

	m_pEngine = new xEvol3DEngine;
	m_pEngine->init(this , (void*)hRenderWindow , _XEVOL_ABSPATH_(L"./system.xml") , L"BaseDemo" , NULL , 0);
	m_pRenderApi = m_pEngine->renderApi();
	//===============

	m_pRenderer = xRendererManager::singleton()->createRenderer(L"KidRenderer",m_pRenderApi);
	m_pRenderer->loadConfig(_XEVOL_ABSPATH_(L"./system.xml"),NULL,0);



	wchar_t* ch_ = L"啊";
	//xDistanceFontGenerator fontGen(32 , 4096 , ch_[0] , ch_[0] + 2);
	//fontGen.generate( _XEVOL_ABSPATH_(L".\\font\\STXINGKA.TTF") ,_XEVOL_ABSPATH_(L".\\font\\test") );
	//fontGen.generate( _XEVOL_ABSPATH_(L".\\font\\msyh.TTF") ,_XEVOL_ABSPATH_(L".\\font\\test") );
	//m_FontGen.load(m_pRenderApi , _XEVOL_ABSPATH_(L"./font/font.tga"));
	//m_FontGen.proccess();

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

	onResize();

	m_ProcedureTexture = m_pRenderApi->createLockableTexture(256,256,PIXELFORMAT_R32G32B32A32F,false );
	xTextureLockArea _lock;
	m_ProcedureTexture->lock(eLock_WriteDiscard , _lock);
	m_ProcedureTexture->unlock(_lock);
	m_pStencilState = m_pRenderApi->createDepthStencilState(L"Overlay");

	m_hProgram = m_pRenderApi->gpuProgramManager()->load(L"simple2D.vertex" , L"simple2D.pixel<0:simple.texture,simple.fakehdr>" , NULL);;

	m_hFont    = m_pRenderApi->findFont(L"small" , true);

	m_pTexMgr = m_pRenderApi->createTextureManager(L"DefTexMgr", true);
	m_pTexMgr->addPath( _XEVOL_ABSPATH_(L"./texture/") );

	m_pModelMgr= xBaseModelMgr::createInstance(m_pRenderApi , m_pTexMgr , L"ModelMgr");
	m_pModelMgr->addPath(_XEVOL_ABSPATH_(L"./model/") );
	m_Model = m_pModelMgr->add(L"einxin.xrm" , true , 0);
	frontCamera(m_Model->boundBox() );

	//判断有没有骨架，如果是骨骼动画，则要加SkinAni的Node
	/*
	xGpuProgNameParser _parser;
	_parser.setShaderName( eShader_VertexShader , L"simpleMesh.vertex");
	if(m_Model->skeleton())
	{
	_parser.addShaderNode( eShader_VertexShader , L"SkinAni" , (size_t)0);
	}
	_parser.setShaderName( eShader_PixelShader , L"simpleMesh.pixel<0:simple.texture,simple.Lighting;>" );
	xGpuProgramName _modelShaderName;
	_parser.toName(_modelShaderName);
	HGpuProgram hModelShader = m_pRenderApi->gpuProgramManager()->load(_modelShaderName);
	m_pModelShader = hModelShader.getResource();
	*/

	m_DefRasterizer= m_pRenderApi->createRasterizerState(L"Default");
	m_pRenderApi->setRasterizerState(m_DefRasterizer);

	m_ArcBall = (IDrawableHelper *)xPluginMgr::singleton()->createObject(L"ArcBall",NULL);
	m_ArcBall->init(m_pRenderApi , m_pTexMgr);

	m_GridPlan = (IDrawableHelper *)xPluginMgr::singleton()->createObject(L"PlaneGrid",NULL);
	m_GridPlan->init(m_pRenderApi , m_pTexMgr);


	m_pSelView = m_pRenderApi->createRenderView(800 , 600 , true);
	m_pSelView->createRenderTarget(1 , PIXELFORMAT_R8G8B8A8 , true , true);

	ILightingEnv* pLightEnv = m_pRenderApi->findLightingState(L"xLightState");
	pLightEnv->enableLight(2);
	xLightDesc* pLight= pLightEnv->lightDesc(0);
	pLight->m_position  = xMathLib::xvec4(1000.0f , 1000.0f , 1000.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 25.0f); 
	pLight->m_Ambient   = xMathLib::xvec4(0.1f , 0.1f , 0.1f , 1.0f);

	//第二个灯光
	pLight= pLightEnv->lightDesc(1);
	pLight->m_position  = xMathLib::xvec4(1000.0f , -1000.0f , -500.0f , 1.0f);
	pLight->m_Diffuse   = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 1.0f);  
	pLight->m_Speculer  = xMathLib::xvec4(0.6f , 0.6f , 0.6f , 25.0f);  
	pLight->m_Ambient   = xMathLib::xvec4(0.1f , 0.1f , 0.1f , 1.0f);


	pLightEnv->applyChange();
	m_xuiWinMgr = xui::xuiWindowManager::createManager(m_pRenderApi , m_pTexMgr);
	m_xuiWinMgr->getXMLManager()->addPath(_XEVOL_ABSPATH_(L"./xui/"));

	m_pModelPass = this->m_pRenderer->createPass(L"model");
	xModelElement* pModelElement = new xModelElement(m_pRenderApi , 0);
	m_pModelPass->setDrawable(pModelElement);
	pModelElement->setModel(m_Model);

	m_pRenderer->setRenderTech(L"Basic");

	xMaterial* pMaterial = m_pRenderer->createMaterial(L"SimpleSkinBlend");
	m_pModelPass->setMaterial(pMaterial);
	return ;
}

void     xEditorPreviewApp::run()
{
	this->setApplicationState(IApplication::AS_RUN);
	m_pEngine->messageLoop();
}

void     xEditorPreviewApp::updateFrame(long passedTime)
{
	static float angle = 0;
	static int   nFrame = 0;
	static int   tPassed = 0;
	tPassed += passedTime;
	nFrame ++;
	angle += passedTime/1000.0f * 35.0f;
	xSleep(5);
	m_pRenderApi->identityMatrix(MATRIXMODE_World);
	m_pRenderApi->applyCamera(m_pCamera);
	if(m_pRenderApi->renderMode() == eRenderMode::eRenderMode_Select )
		m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.0f,0.0f));
	else 
		m_pRenderApi->begin(xColor_4f(0.0f,0.0f,0.3f,1.0f));
	m_pRenderApi->beginScene();
	xMathLib::xmat4 mat;
	//xMathLib::XM_RotateY(mat,angle);

	//绘制模型
	m_pRenderApi->colorSelector()->setRenderObjectID(1,0);
	m_pRenderApi->identityMatrix(MATRIXMODE_World);

	XM_Transform(mat , m_ModelPos.x ,m_ModelPos.y , m_ModelPos.z );
	m_pRenderApi->multiMatrix(m_ModelRotate.data , MATRIXMODE_World);
	m_pRenderApi->multiMatrix(mat.data           , MATRIXMODE_World);


	//=============================
	m_Model->updateFrame(passedTime);
	//m_Model->draw();
	//==============================
	this->m_pRenderer->begin(passedTime);
	this->m_pRenderer->drawPass(xStringHash(L"defualt") , m_pModelPass);
	this->m_pRenderer->end(passedTime);

	//绘制AcrBall
	xvec3 _center =  getModelCeneter(m_Model.getResource() );

	m_pRenderApi->identityMatrix(MATRIXMODE_World);
	xMathLib::XM_Scale(mat,30.0f,30.0f,30.0f);
	m_pRenderApi->multiMatrix(mat.data, MATRIXMODE_World);
	xMathLib::XM_Transform(mat , _center.x , _center.y , _center.z );
	m_pRenderApi->multiMatrix(mat.data, MATRIXMODE_World);
	XM_Transform(mat , m_ModelPos.x ,m_ModelPos.y , m_ModelPos.z );
	m_pRenderApi->multiMatrix(mat.data, MATRIXMODE_World);		

	m_pRenderApi->colorSelector()->setRenderObjectID(100,0);
	m_ArcBall->begin();
	m_ArcBall->render(passedTime);


	//Plan
	m_pRenderApi->identityMatrix(MATRIXMODE_World);
	xMathLib::XM_Scale(mat,500.0f,500.0f,500.0f);
	m_pRenderApi->multiMatrix(mat.data, MATRIXMODE_World);
	m_pRenderApi->colorSelector()->setRenderObjectID(101,0);
	m_GridPlan->begin();
	m_GridPlan->render(passedTime);

	//===================
	m_pRenderApi->identityMatrix(MATRIXMODE_World);
	m_pRenderApi->setDepthStencilState(m_pStencilState);
	xvec4     destRect(20,300,400,300);
	xvec4     srcRect(0,0,256,256);
	xColor_4f cl(1.0f,1.0f,1.0f,1.0f);
	IGpuProgram* pGpuProgram = m_hProgram.getResource()  ;
	wchar_t texInfo[1024] = {0};
	swprintf(texInfo , L"FPS=%f \nRenderApi=%s" , nFrame*1000.0f/tPassed ,m_pRenderApi->name() );

	m_hFont->drawText(texInfo,10,10,xColor_4f(1.0f,1.0f,1.0f,1.0f));
	m_hFont->drawText(L"Shader Composer ",10,50,xColor_4f(1.0f,1.0f,1.0f,1.0f));
	m_xuiWinMgr->draw();
	//m_pRenderApi->
	//===================
	m_pRenderApi->endScene();
	//m_pRenderApi->endSelectMode(0,0,NULL);
	m_pRenderApi->swapBuffer();         
}

xEditorPreviewApp::xEditorPreviewApp()
{
	m_pEngine   = NULL;
	m_pRenderApi   = NULL;
	m_ArcBall   = NULL;
	m_GridPlan   = NULL;

	m_pCamera   = NULL;
	m_pStencilState   = NULL;
	m_DefRasterizer   = NULL;
	m_ProcedureTexture   = NULL;
	m_pTexMgr   = NULL;
	m_pModelMgr   = NULL;
	m_cameraStep =1.0f;

	m_pSelView   = NULL;
	m_bInDrag = false;
	m_RotateAxis = 0;

	m_pRenderer    = NULL;
	m_pModelPass   = NULL;
}