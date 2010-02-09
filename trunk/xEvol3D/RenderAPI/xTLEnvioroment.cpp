#include "../xStdPch.h"
#include "xTLEnvioroment.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(ILightingEnv   , IRenderApiObject);
ILightingEnv::ILightingEnv(IRenderApi* pRenderApi)
:IRenderApiObject(pRenderApi)
{

}

ILightingEnv::~ILightingEnv()
{

}

bool IMatrixContext::multiMatrix(const float* _mat , eMatrixMode matMode)
{
	xmat4 matTemp;
	xmat4& mat = *( (xmat4*)_mat ) ;
	switch(matMode)
	{
	case MATRIXMODE_World:
		matTemp = m_Context->m_matWorld;
		XM_Mul(matTemp,mat, m_Context->m_matWorld);
		break;

	case MATRIXMODE_View:
		matTemp = m_Context->m_matView;
		XM_Mul(matTemp,mat, m_Context->m_matView);
		break;

	case MATRIXMODE_Project:
		matTemp = m_Context->m_matProject;
		XM_Mul(matTemp,mat, m_Context->m_matProject);
		break;

	case MATRIXMODE_Texture:
		matTemp = m_Context->m_matTexture;
		XM_Mul(matTemp,mat, m_Context->m_matTexture);
		break;
	}
	this->onMatrixChange(matMode);
	return true;

}

bool IMatrixContext::setMatrix(const float* _mat   , eMatrixMode matMode)
{
	xmat4& mat = *( (xmat4*)_mat ) ;
	switch(matMode)
	{
	case MATRIXMODE_World:
		m_Context->m_matWorld = mat;
		break;

	case MATRIXMODE_View:
		m_Context->m_matView = mat;
		break;

	case MATRIXMODE_Project:
		m_Context->m_matProject = mat;
		break;

	case MATRIXMODE_Texture:
		m_Context->m_matTexture = mat;
		break;
	}
	this->onMatrixChange(matMode);
	return true;
}

bool IMatrixContext::identityMatrix(eMatrixMode matMode)
{
	switch(matMode)
	{
	case MATRIXMODE_World:
		m_Context->m_matWorld.identity();
		break;

	case MATRIXMODE_View:
		m_Context->m_matView.identity();
		break;

	case MATRIXMODE_Project:
		m_Context->m_matProject.identity();
		break;

	case MATRIXMODE_Texture:
		m_Context->m_matTexture.identity();
		break;
	}
	this->onMatrixChange(matMode);
	return true;
}


bool IMatrixContext::getMatrix(float* _mat , eMatrixMode matMode)
{
	xmat4& mat = *( (xmat4*)_mat ) ;
	switch(matMode)
	{
	case MATRIXMODE_World:
		mat = m_Context->m_matWorld;
		break;

	case MATRIXMODE_View:
		mat = m_Context->m_matView;
		break;

	case MATRIXMODE_Project:
		mat = m_Context->m_matProject;
		break;

	case MATRIXMODE_Texture:
		mat = m_Context->m_matTexture;
		break;
	}
	return true;
}

bool IMatrixContext::pushMatrix(eMatrixMode matMode)
{
	switch(matMode)
	{
	case MATRIXMODE_World:
		m_stackWorldMatrix.push_back( m_Context->m_matWorld );
		break;

	case MATRIXMODE_View:
		m_stackViewMatrix.push_back( m_Context->m_matView );
		break;

	case MATRIXMODE_Project:
		m_stackProjectMatrix.push_back( m_Context->m_matProject );
		break;

	case MATRIXMODE_Texture:
		m_stackTextureMatrix.push_back( m_Context->m_matTexture );
		break;
	}
	this->onMatrixChange(matMode);
	return true;
}

bool IMatrixContext::popMatrix(eMatrixMode matMode)
{
	switch(matMode)
	{
	case MATRIXMODE_World:
		{
			size_t iLen = m_stackWorldMatrix.size();
			if(iLen == 0) return false;
			m_Context->m_matWorld = m_stackWorldMatrix[iLen-1];
			m_stackWorldMatrix.pop_back();
		}

		break;

	case MATRIXMODE_View:
		{
			size_t iLen = m_stackViewMatrix.size();
			if(iLen == 0) return false;
			m_Context->m_matView = m_stackViewMatrix[iLen-1];
			m_stackViewMatrix.pop_back();
		}
		break;

	case MATRIXMODE_Project:
		{
			size_t iLen = m_stackProjectMatrix.size();
			if(iLen == 0) return false;
			m_Context->m_matProject = m_stackProjectMatrix[iLen-1];
			m_stackProjectMatrix.pop_back();
		}
		break;

	case MATRIXMODE_Texture:
		{
			size_t iLen = m_stackTextureMatrix.size();
			if(iLen == 0) return false;
			m_Context->m_matTexture = m_stackTextureMatrix[iLen-1];
			m_stackTextureMatrix.pop_back();
		}
		break;
	}
	this->onMatrixChange(matMode);
	return true;
}

void* xTransformContext::data()
{
	return &m_matWorld;
}

int xTransformContext::dataLen()
{
	return sizeof(xmat4) * 4;
}

END_NAMESPACE_XEVOL3D
