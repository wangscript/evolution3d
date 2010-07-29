#include "../xStdPch.h"
#include "xMatrixContext.h"
#include "RenderAPI/xRenderAPI.h"
#include "RenderAPI/xRenderApiImp.h"
#include "RenderAPI/xMemBuffer.h"
BEGIN_NAMESPACE_XEVOL3D


xRenderApiMatContext::xRenderApiMatContext(IRenderApi* pRenderApi)
{
	m_name         = L"TransformBuffer";
	m_pReflection  = NULL;
	m_pConstBuffer = NULL;
    m_pCamera      = NULL;

	//注册变量绑定器
    m_pWorldConstBinder  = NULL;
    m_pViewMatBinder     = NULL;
    m_pProjMatBinder     = NULL;
    m_pTextureMatBinder  = NULL;
    
    m_pCameraUpBinder   = NULL;
    m_pCameraPosBinder  = NULL;
    m_pCameraDirBinder  = NULL;
    m_pCameraArgBinder  = NULL;

}

xRenderApiMatContext::~xRenderApiMatContext()
{
    delete m_pWorldConstBinder;
    //ViewMatrix
    delete m_pViewMatBinder;
    //ProjectMatrix
    delete m_pProjMatBinder;
    //Texture Matrix
    delete m_pTextureMatBinder;

    delete m_pCameraUpBinder ;
    delete m_pCameraPosBinder;

    delete m_pCameraDirBinder;
    delete m_pCameraArgBinder;
}

bool xRenderApiMatContext::install()
{
	return m_pRenderApi->setInputBuffer( m_name.c_str() ,NULL);

}

bool xRenderApiMatContext::uninstall()
{
	return m_pRenderApi->setInputBuffer(m_name.c_str() ,NULL);
}

bool xRenderApiMatContext::init(IRenderApi* pRenderApi)
{
	m_pRenderApi = pRenderApi;

	int iBufLen = sizeof(xTransformContext) + sizeof(xCameraEyeInfo);
	m_pConstBuffer = pRenderApi->createConstBuffer( iBufLen );
	IInputBuffer* pInputBuffer = (IInputBuffer*)m_pConstBuffer;
	pRenderApi->setInputBuffer(m_name.c_str() , m_pConstBuffer);
	m_pReflection = (IInputBufferReflection *)pInputBuffer->createReflection(true);
	m_Context     = (xTransformContext*)m_pReflection->systemBuffer();
	m_pCameraInfo = (xCameraEyeInfo*) ( (char*)m_pReflection->systemBuffer() + sizeof(xTransformContext)  ) ;
	m_pReflection->setDirty(true);

	m_Context->m_matWorld.identity();
	m_Context->m_matView.identity();
	m_Context->m_matProject.identity();
	m_Context->m_matTexture.identity();

	//初始化CameraInfo;
	m_pCameraInfo->m_EyeDir = xvec4(0.0f , 1.0f     , 0.0f );
	m_pCameraInfo->m_EyePos = xvec4(0.0f , -1000.0f , 0.0f );
	m_pCameraInfo->m_Up     = xvec4(0.0f , 0.0f     , 1.0f );
	m_pCameraInfo->m_fNear  = 0.1f;
	m_pCameraInfo->m_fFar   = 10000.0f;
	m_pCameraInfo->m_fFov   = 45.0f;
	m_pCameraInfo->m_fAspect=16.0f/10.0f;


    //注册矩阵和相机位置的绑定器
    m_pCameraUpBinder   = new xConstantValueDataBinder(m_pCamera->m_Eye.m_Up.v     , sizeof(xvec4) );
    m_pCameraPosBinder  = new xConstantValueDataBinder(m_pCamera->m_Eye.m_EyePos.v , sizeof(xvec4) );
    m_pCameraDirBinder  = new xConstantValueDataBinder(m_pCameraInfo->m_EyeDir.v   , sizeof(xvec4) );
    m_pCameraArgBinder  = new xConstantValueDataBinder(m_pCameraInfo->m_CameraArg  , sizeof(xvec4) );

    pRenderApi->registeShaderConstBinder(L"CameraUp"      , m_pCameraUpBinder );
    pRenderApi->registeShaderConstBinder(L"CameraPos"     , m_pCameraPosBinder);
    pRenderApi->registeShaderConstBinder(L"cameraUp"      , m_pCameraUpBinder );
    pRenderApi->registeShaderConstBinder(L"cameraPos"     , m_pCameraPosBinder);
    pRenderApi->registeShaderConstBinder(eSPS_Camera_Up   , m_pCameraUpBinder);
    pRenderApi->registeShaderConstBinder(eSPS_Camera_Pos  , m_pCameraPosBinder);

    pRenderApi->registeShaderConstBinder(L"CameraDir"     , m_pCameraDirBinder);
    pRenderApi->registeShaderConstBinder(L"CameraArg"     , m_pCameraArgBinder);
    pRenderApi->registeShaderConstBinder(L"cameraDir"     , m_pCameraDirBinder);
    pRenderApi->registeShaderConstBinder(L"cameraArg"     , m_pCameraArgBinder);
    pRenderApi->registeShaderConstBinder(eSPS_Camera_Dir  , m_pCameraDirBinder);
    pRenderApi->registeShaderConstBinder(eSPS_Camera_Arg  , m_pCameraArgBinder);



    m_pWorldConstBinder  = new xConstantValueDataBinder(transContext()->m_matWorld.data , sizeof(xmat4) );
    m_pViewMatBinder     = new xConstantValueDataBinder(transContext()->m_matView.data , sizeof(xmat4) );
    m_pProjMatBinder     = new xConstantValueDataBinder(transContext()->m_matProject.data , sizeof(xmat4) );
    m_pTextureMatBinder  = new xConstantValueDataBinder(transContext()->m_matTexture.data , sizeof(xmat4) );
    
    //注册变量绑定器
    //WorldMatrix
    pRenderApi->registeShaderConstBinder(L"matWorld"       , m_pWorldConstBinder);
    pRenderApi->registeShaderConstBinder(L"WorldMatrix"    , m_pWorldConstBinder);
    pRenderApi->registeShaderConstBinder(eSPS_WorldMatrix  , m_pWorldConstBinder);


    //ViewMatrix
    pRenderApi->registeShaderConstBinder(L"matView"       , m_pViewMatBinder);
    pRenderApi->registeShaderConstBinder(L"ViewMatrix"    , m_pViewMatBinder);
    pRenderApi->registeShaderConstBinder(eSPS_ViewMatrix  , m_pViewMatBinder);

    //ProjectMatrix
    pRenderApi->registeShaderConstBinder(L"matProj"        , m_pProjMatBinder);
    pRenderApi->registeShaderConstBinder(L"matProject"     , m_pProjMatBinder);
    pRenderApi->registeShaderConstBinder(L"ProjMatrix"     , m_pProjMatBinder);
    pRenderApi->registeShaderConstBinder(L"ProjectMatrix"  , m_pProjMatBinder);
    pRenderApi->registeShaderConstBinder(eSPS_ProjMatrix   , m_pProjMatBinder);

    //Texture Matrix
    pRenderApi->registeShaderConstBinder(L"matTexture"      , m_pTextureMatBinder);
    pRenderApi->registeShaderConstBinder(L"TextureMatrix"   , m_pTextureMatBinder);
    pRenderApi->registeShaderConstBinder(eSPS_TextureMatrix , m_pTextureMatBinder);

	return true;
}

void xRenderApiMatContext::onMatrixChange(eMatrixMode matMode)
{
	switch(matMode)
	{
	case MATRIXMODE_World:
		m_pWorldConstBinder->setDirty();		
		break;

	case MATRIXMODE_View:
		m_pViewMatBinder->setDirty();		
		break;

	case MATRIXMODE_Project:
		m_pProjMatBinder->setDirty();		
		break;

	case MATRIXMODE_Texture:
		m_pTextureMatBinder->setDirty();	
		break;
	}
	m_pReflection->setDirty(true);
}


IMPL_REFCOUNT_OBJECT_FUNCTION(xBasicRenderCamera);
IRenderCamera* xRenderApiMatContext::createCamera(const wchar_t* cameraName)
{
      return new xBasicRenderCamera(this);
}

bool xRenderApiMatContext::applyCamera()
{
	if(m_pCamera == NULL )
	{
		return false;
	}
	//D3DXMatrixPerspectiveFovRH( (D3DXMATRIX*)m_matProject.data , m_pCamera->getFOV(), m_pCamera->getAspect() , m_pCamera->getNearPlan(), m_pCamera->getFarPlan() );
	//D3DXMatrixLookAtRH((D3DXMATRIX*)m_matView.data , (D3DXVECTOR3*)m_pCamera->m_Eye.m_EyePos.v ,   (D3DXVECTOR3*)m_pCamera->m_Eye.m_EyeTarget.v ,  (D3DXVECTOR3*)m_pCamera->m_Eye.m_Up.v );
	m_pCamera->toProjectMatrix( m_Context->m_matProject );
	m_pCamera->toViewMatrix( m_Context->m_matView );
	onMatrixChange(MATRIXMODE_Project);
	onMatrixChange(MATRIXMODE_View);
	onCameraChange(m_pCamera);
	return true;
}

bool xRenderApiMatContext::onCameraChange(IRenderCamera* pCamera)
{
    if(m_pCamera != pCamera)
		return false;
	m_pCameraInfo->m_Up     = pCamera->m_Eye.m_Up;
	m_pCameraInfo->m_EyePos = pCamera->m_Eye.m_EyePos;
	m_pCameraInfo->m_EyeDir = pCamera->m_Eye.m_EyeTarget - pCamera->m_Eye.m_EyePos;
	m_pCameraInfo->m_fNear  = pCamera->getNearPlan();
	m_pCameraInfo->m_fFar   = pCamera->getFarPlan();
	m_pCameraInfo->m_fFov   = pCamera->getFOV();
	m_pCameraInfo->m_fAspect= pCamera->getAspect();
	m_pCameraInfo->m_Up.normalize();
	m_pCameraInfo->m_EyeDir.normalize();
	m_pReflection->setDirty(true);


	//更新状态
	m_pCameraUpBinder->setDirty();	 
	m_pCameraPosBinder->setDirty();	
	m_pCameraDirBinder->setDirty();
	m_pCameraArgBinder->setDirty();
	return true;

}

bool xRenderApiMatContext::applyCamera(IRenderCamera* pCamera)
{
	if(pCamera == NULL )
	{
		return false;
	}
	m_pCamera = (xBasicRenderCamera *)pCamera;
	return applyCamera();
}

IRenderCamera* xRenderApiMatContext::getCamera()
{
	return m_pCamera;
}

END_NAMESPACE_XEVOL3D
