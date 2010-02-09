#include "../xStdPch.h"
#include "xMatrixContext.h"
#include "RenderAPI/xRenderAPI.h"
#include "RenderAPI/xMemBuffer.h"
BEGIN_NAMESPACE_XEVOL3D


xRenderApiMatContext::xRenderApiMatContext(IRenderApi* pRenderApi) :
m_WorldConstBinder(this),
m_textureMatBinder(this),
m_projMatBinder(this),
m_viewMatBinder(this),
m_CameraUpBinder(this),
m_CameraPosBinder(this),
m_CameraDirBinder(this),
m_CameraArgBinder(this)
{
	m_name         = L"TransformBuffer";
	m_pReflection  = NULL;
	m_pConstBuffer = NULL;

	//注册变量绑定器
	//WorldMatrix
	pRenderApi->registeShaderConstBinder(L"matWorld"       , &m_WorldConstBinder);
	pRenderApi->registeShaderConstBinder(L"WorldMatrix"    , &m_WorldConstBinder);
	pRenderApi->registeShaderConstBinder(eSPS_WorldMatrix  , &m_WorldConstBinder);


	//ViewMatrix
	pRenderApi->registeShaderConstBinder(L"matView"       , &m_viewMatBinder);
	pRenderApi->registeShaderConstBinder(L"ViewMatrix"    , &m_viewMatBinder);
	pRenderApi->registeShaderConstBinder(eSPS_ViewMatrix  , &m_viewMatBinder);

	//ProjectMatrix
	pRenderApi->registeShaderConstBinder(L"matProj"        , &m_projMatBinder);
	pRenderApi->registeShaderConstBinder(L"matProject"     , &m_projMatBinder);
	pRenderApi->registeShaderConstBinder(L"ProjMatrix"     , &m_projMatBinder);
	pRenderApi->registeShaderConstBinder(L"ProjectMatrix"  , &m_projMatBinder);
	pRenderApi->registeShaderConstBinder(eSPS_ProjMatrix   , &m_viewMatBinder);

	//Texture Matrix
	pRenderApi->registeShaderConstBinder(L"matTexture"      , &m_textureMatBinder);
	pRenderApi->registeShaderConstBinder(L"TextureMatrix"   , &m_textureMatBinder);
	pRenderApi->registeShaderConstBinder(eSPS_TextureMatrix , &m_textureMatBinder);


	pRenderApi->registeShaderConstBinder(L"CameraUp"      , &m_CameraUpBinder );
	pRenderApi->registeShaderConstBinder(L"CameraPos"     , &m_CameraPosBinder);
	pRenderApi->registeShaderConstBinder(L"CameraDir"     , &m_CameraDirBinder);
	pRenderApi->registeShaderConstBinder(L"CameraArg"     , &m_CameraArgBinder);

	pRenderApi->registeShaderConstBinder(L"cameraUp"      , &m_CameraUpBinder );
	pRenderApi->registeShaderConstBinder(L"cameraPos"     , &m_CameraPosBinder);
	pRenderApi->registeShaderConstBinder(L"cameraDir"     , &m_CameraDirBinder);
	pRenderApi->registeShaderConstBinder(L"cameraArg"     , &m_CameraArgBinder);

	pRenderApi->registeShaderConstBinder(eSPS_Camera_Up   , &m_CameraUpBinder);
	pRenderApi->registeShaderConstBinder(eSPS_Camera_Pos  , &m_CameraPosBinder);
	pRenderApi->registeShaderConstBinder(eSPS_Camera_Dir  , &m_CameraDirBinder);
	pRenderApi->registeShaderConstBinder(eSPS_Camera_Arg  , &m_CameraArgBinder);
}

xRenderApiMatContext::~xRenderApiMatContext()
{

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
	return true;
}

void xRenderApiMatContext::onMatrixChange(eMatrixMode matMode)
{
	switch(matMode)
	{
	case MATRIXMODE_World:
		m_WorldConstBinder.setDirty();		
		break;

	case MATRIXMODE_View:
		m_viewMatBinder.setDirty();		
		break;

	case MATRIXMODE_Project:
		m_projMatBinder.setDirty();		
		break;

	case MATRIXMODE_Texture:
		m_textureMatBinder.setDirty();	
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
	m_CameraUpBinder.setDirty();	 
	m_CameraPosBinder.setDirty();	
	m_CameraDirBinder.setDirty();
	m_CameraArgBinder.setDirty();
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

//自动设置世界矩阵
bool xWorldMatConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData(matContext->transContext()->m_matWorld.data , sizeof(xmat4) );
    return true;
}

//自动设置观察矩阵
bool xViewMatConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData(matContext->transContext()->m_matView.data , sizeof(xmat4) );
	return true;
}

//自动设置投影矩阵
bool xProjectMatConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData(matContext->transContext()->m_matProject.data , sizeof(xmat4) );
	return true;
}

//自动设置纹理矩阵
bool xTextureMatConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData(matContext->transContext()->m_matTexture.data , sizeof(xmat4) );
	return true;
}

//自动设置Camera的属性
bool xCameraUpConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	xvec4 up = matContext->m_pCamera->m_Eye.m_Up;
	up.normalize();
	pConst->setData( &up , sizeof(xvec4) );
	return true;
}

bool xCameraPosConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData( &matContext->m_pCamera->m_Eye.m_EyePos , sizeof(xvec4) );
	return true;
}

bool xCameraDirConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	xvec4 dir = matContext->m_pCamera->m_Eye.m_EyeTarget - matContext->m_pCamera->m_Eye.m_EyePos;
	dir.normalize();
	pConst->setData( &dir , sizeof(xvec4) );
	return true;
}

bool xCameraArgConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	xvec4 arg(matContext->m_pCamera->getNearPlan() , matContext->m_pCamera->getFarPlan() , matContext->m_pCamera->getFOV() , matContext->m_pCamera->getAspect() ); 
	pConst->setData( &arg , sizeof(xvec4) );
	return true;
}
END_NAMESPACE_XEVOL3D
