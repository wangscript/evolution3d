#include "xD10MatrixContext.h"
#include "xDirect3D10API.h"
BEGIN_NAMESPACE_XEVOL3D



xD3D10MatrixContext::xD3D10MatrixContext() :
m_WorldConstBinder(this),
m_textureMatBinder(this),
m_projMatBinder(this),
m_viewMatBinder(this)
{

}

xD3D10MatrixContext::~xD3D10MatrixContext()
{

}
bool xD3D10MatrixContext::init(xD3D10RenderApi* pRenderApi)
{
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


	m_matWorld.identity();
	m_matView.identity();
	m_matProject.identity();
	m_matTexture.identity();
	return true;
}

bool xD3D10MatrixContext::multiMatrix(float* _mat , eMatrixMode matMode)
{
	xmat4 matTemp;
	xmat4& mat = *( (xmat4*)_mat ) ;
	switch(matMode)
	{
	case MATRIXMODE_World:
		matTemp = m_matWorld;
		XM_Mul(matTemp,mat,m_matWorld);
		m_WorldConstBinder.setDirty();		
		break;

	case MATRIXMODE_View:
		matTemp = m_matView;
		XM_Mul(matTemp,mat,m_matView);
		m_viewMatBinder.setDirty();		
		break;

	case MATRIXMODE_Project:
		matTemp = m_matProject;
		XM_Mul(matTemp,mat,m_matProject);
		m_projMatBinder.setDirty();		
		break;

	case MATRIXMODE_Texture:
		matTemp = m_matTexture;
		XM_Mul(matTemp,mat,m_matTexture);
		m_textureMatBinder.setDirty();	
		break;
	}
	return true;

}

bool xD3D10MatrixContext::setMatrix(float* _mat   , eMatrixMode matMode)
{
	xmat4& mat = *( (xmat4*)_mat ) ;
	switch(matMode)
	{
	case MATRIXMODE_World:
		m_matWorld = mat;
		m_WorldConstBinder.setDirty();		
		break;

	case MATRIXMODE_View:
		m_matView = mat;
		m_viewMatBinder.setDirty();		
		break;

	case MATRIXMODE_Project:
		m_matProject = mat;
		m_projMatBinder.setDirty();		
		break;

	case MATRIXMODE_Texture:
		m_matTexture = mat;
		m_textureMatBinder.setDirty();	
		break;
	}
	return true;
}

bool xD3D10MatrixContext::identityMatrix(eMatrixMode matMode)
{
	switch(matMode)
	{
	case MATRIXMODE_World:
		m_matWorld.identity();
		m_WorldConstBinder.setDirty();		
		break;

	case MATRIXMODE_View:
		m_matView.identity();
		m_viewMatBinder.setDirty();		
		break;

	case MATRIXMODE_Project:
		m_matProject.identity();
		m_projMatBinder.setDirty();		
		break;

	case MATRIXMODE_Texture:
		m_matTexture.identity();
		m_textureMatBinder.setDirty();	
		break;
	}
	return true;
}

bool xD3D10MatrixContext::getMatrix(float* _mat , eMatrixMode matMode)
{
	xmat4& mat = *( (xmat4*)_mat ) ;
	switch(matMode)
	{
	case MATRIXMODE_World:
		mat = m_matWorld;
		break;

	case MATRIXMODE_View:
		mat = m_matView;
		break;

	case MATRIXMODE_Project:
		mat = m_matProject;
		break;

	case MATRIXMODE_Texture:
		mat = m_matTexture;
		break;
	}
	return true;
}

bool xD3D10MatrixContext::pushMatrix(eMatrixMode matMode)
{
	switch(matMode)
	{
	case MATRIXMODE_World:
		m_stackWorldMatrix.push_back( m_matWorld );
		break;

	case MATRIXMODE_View:
		m_stackViewMatrix.push_back( m_matView );
		break;

	case MATRIXMODE_Project:
		m_stackProjectMatrix.push_back( m_matProject );
		break;

	case MATRIXMODE_Texture:
		m_stackTextureMatrix.push_back( m_matTexture );
		break;
	}
	return true;
}

bool xD3D10MatrixContext::popMatrix(eMatrixMode matMode)
{
	switch(matMode)
	{
	case MATRIXMODE_World:
		{
		    size_t iLen = m_stackWorldMatrix.size();
			if(iLen == 0) return false;
			m_matWorld = m_stackWorldMatrix[iLen-1];
			m_stackWorldMatrix.pop_back();
			m_WorldConstBinder.setDirty();
		}

		break;

	case MATRIXMODE_View:
		{
			size_t iLen = m_stackViewMatrix.size();
			if(iLen == 0) return false;
			m_matView = m_stackViewMatrix[iLen-1];
			m_stackViewMatrix.pop_back();
			m_viewMatBinder.setDirty();
		}
		break;

	case MATRIXMODE_Project:
		{
			size_t iLen = m_stackProjectMatrix.size();
			if(iLen == 0) return false;
			m_matProject = m_stackProjectMatrix[iLen-1];
			m_stackProjectMatrix.pop_back();
			m_projMatBinder.setDirty();
		}
		break;

	case MATRIXMODE_Texture:
		{
			size_t iLen = m_stackTextureMatrix.size();
			if(iLen == 0) return false;
			m_matTexture = m_stackTextureMatrix[iLen-1];
			m_stackTextureMatrix.pop_back();
			m_textureMatBinder.setDirty();
		}
		break;
	}
	return true;
}

IRenderCamera* xD3D10MatrixContext::createCamera(const wchar_t* cameraName)
{
      return new xD3D10Camera();
}

bool xD3D10MatrixContext::applyCamera()
{
	if(m_pCamera == NULL )
	{
		return false;
	}
	//D3DXMatrixPerspectiveFovRH( (D3DXMATRIX*)m_matProject.data , m_pCamera->getFOV(), m_pCamera->getAspect() , m_pCamera->getNearPlan(), m_pCamera->getFarPlan() );
	//D3DXMatrixLookAtRH((D3DXMATRIX*)m_matView.data , (D3DXVECTOR3*)m_pCamera->m_Eye.m_EyePos.v ,   (D3DXVECTOR3*)m_pCamera->m_Eye.m_EyeTarget.v ,  (D3DXVECTOR3*)m_pCamera->m_Eye.m_Up.v );
	m_pCamera->toProjectMatrix( m_matProject );
	m_pCamera->toViewMatrix( m_matView );
	return true;
}

bool xD3D10MatrixContext::applyCamera(IRenderCamera* pCamera)
{
	if(pCamera == NULL )
	{
		return false;
	}
	m_pCamera = (xD3D10Camera *)pCamera;
	//D3DXMatrixPerspectiveFovRH( (D3DXMATRIX*)m_matProject.data , m_pCamera->getFOV(), m_pCamera->getAspect() , m_pCamera->getNearPlan(), m_pCamera->getFarPlan() );
	//D3DXMatrixLookAtRH((D3DXMATRIX*)m_matView.data , (D3DXVECTOR3*)m_pCamera->m_Eye.m_EyePos.v ,   (D3DXVECTOR3*)m_pCamera->m_Eye.m_EyeTarget.v ,  (D3DXVECTOR3*)m_pCamera->m_Eye.m_Up.v );
	m_pCamera->toProjectMatrix( m_matProject );
	m_pCamera->toViewMatrix( m_matView );

	return true;
}

IRenderCamera* xD3D10MatrixContext::getCamera()
{
	return m_pCamera;
}

//自动设置世界矩阵
bool xWorldMatConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData(matContext->m_matWorld.data , sizeof(xmat4) );
    return true;
}

//自动设置观察矩阵
bool xViewMatConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData(matContext->m_matView.data , sizeof(xmat4) );
	return true;
}

//自动设置投影矩阵
bool xProjectMatConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData(matContext->m_matProject.data , sizeof(xmat4) );
	return true;
}

//自动设置纹理矩阵
bool xTextureMatConstBinder::updateConstant(IShaderConstantReflection* pConst)
{
	pConst->setData(matContext->m_matTexture.data , sizeof(xmat4) );
	return true;
}

END_NAMESPACE_XEVOL3D
