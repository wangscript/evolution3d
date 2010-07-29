#include "../xStdPch.h"
#include "xDrawElement.h"
#include "xRenderer.h"
#include "xMaterial.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(IRenderPassArg  ,  IBaseObject);

IMPL_BASE_OBJECT_DLLSAFE(IDrawElement ,  IRenderApiObject);
IMPL_BASE_OBJECT_DLLSAFE(xRenderPass  ,  IRenderApiObject);

IMPL_OBJECT_FACTORY_MGR(IDrawElement ,IDrawElementCreator , xDrawElementFactoryMgr );
IMPL_OBJECT_FACTORY_MGR(xRenderPass  ,xRenderPassCreator  , xRenderPassFactoryMgr );

bool IRenderPassArg::setMatrix(const xMathLib::xmat4& _mat)
{
	m_trans = _mat;
	return true;
}

const xMathLib::xmat4& IRenderPassArg::getMatrix()
{
	return m_trans;
}

IRenderPassArg::IRenderPassArg()
{

}

IRenderPassArg::~IRenderPassArg()
{

}


IDrawElement::IDrawElement(IBaseRenderer* pRenderer)
:IRenderObject(pRenderer)
{

}

bool IDrawElement::isVisible(xGeomLib::xCamera* pCamera , const xMathLib::xmat4& _mat)
{
	bool bVisible = true;
	if(pCamera)
	{
		xGeomLib::xaabb _aabb;
		this->aabb(_aabb);
		xMathLib::xvec3 _min = _aabb.m_Min * _mat;
		xMathLib::xvec3 _max = _aabb.m_Max * _mat;
		xMathLib::xvec3 cen = 0.5f* (_min + _max);
		xMathLib::xvec3 len =  ( _max - _min );
		float l = len.len() * 0.5f;
		bVisible = pCamera->frustum()->visible(cen , l);
	}
	return bVisible;
}

IRenderApi* IDrawElement::renderApi()
{
	if(m_pRenderer == NULL)
		return NULL;
	return m_pRenderer->renderApi();
}
//==========================================
xRenderPass::xRenderPass(IBaseRenderer* pRenderer)
:IRenderObject(pRenderer)
{
	m_RefCount   = 1;
	m_Material   = NULL;
	m_Drawable   = NULL;
	m_ConstTable = NULL;
}

xRenderPass::~xRenderPass()
{
    return ;
}

bool  xRenderPass::render(unsigned long passedTime)
{
   if(m_Drawable == NULL)
	   return false;
  
   IRendererEventCallback* pCallback = m_pRenderer->getEventCallBack();
   const xmat4& _trans = m_RenderPassArg->getMatrix();
   //m_pRenderApi->pushMatrix(eMatrixMode::MATRIXMODE_World);
   IRenderApi* pRenderApi = m_pRenderer->renderApi();
   pRenderApi->setMatrix(_trans.data , MATRIXMODE_World);
   //有材质
   if(m_Material)
   {
	    //应用材质
	   m_Material->begin(m_ConstTable);

	   //绘制物体
	   
	   if(pCallback == NULL)
	   {
		   m_Material->render(m_Drawable,passedTime);
	   }
	   else
	   {
		   pCallback->preDrawPass (m_pRenderer , this);
		   m_Material->render(m_Drawable,passedTime);
		   pCallback->postDrawPass(m_pRenderer , this);

	   }
	   //材质使用结束
	   m_Material->end(m_ConstTable);
   }
   else
   {
	   //绘制物体
	   m_Drawable->render(passedTime);
   }
   return true;
}


IMPL_BASE_OBJECT_DLLSAFE(xSerializeDrawElement ,  IDrawElement);
xSerializeDrawElement::xSerializeDrawElement(IBaseRenderer* pRenderer, int arg)
:IDrawElement(pRenderer)
{

}

xSerializeDrawElement::~xSerializeDrawElement()
{

}

END_NAMESPACE_XEVOL3D
