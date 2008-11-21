#include "xGeomElementObject.h"
#include "xMaterial.h"
#define new XNEW_OP

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(x2DQuadElement , IDrawElement)
//=========Draw Element of the xCoreMesh===========================
x2DQuadElement::x2DQuadElement(IRenderApi* pApi)
:IDrawElement(pApi)
{
	m_Rectf.x = 0; 
	m_Rectf.y = 0;
	m_Rectf.w = 0; 
	m_Rectf.h = 0; 
	
	m_fColor.r = 1.0f ; 
	m_fColor.g = 1.0f ;  
	m_fColor.b = 1.0f ; 
	m_fColor.a = 1.0f ;
}

x2DQuadElement::~x2DQuadElement()
{

}

void x2DQuadElement::center(XMathLib::xvec3&   _center)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
}
void x2DQuadElement::aabb(xGeomLib::xaabb&     _aabb)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
}
void x2DQuadElement::shpere(xGeomLib::xshpere& _shpere)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
}

IDrawElement* x2DQuadElement::clone()
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	return NULL;
}

void x2DQuadElement::setupTransform()
{
	m_pRenderApi->setMatrix(m_matPos.data,MATRIXMODE_World);
}
void x2DQuadElement::setTexture(IBaseTexture* hTexture , int stage)
{
   m_hMainTexture = hTexture;
}

void x2DQuadElement::setRect(float x , float y , float w , float h)
{
	m_Rectf.x = x;
	m_Rectf.y = y;
	m_Rectf.w = w;
	m_Rectf.h = h;
}

void x2DQuadElement::setFullScreenRect()
{
	xvec4 _rct;
	//m_pRenderApi->getViewPort(_rct);
	XEVOL_WARNNING_NOT_IMPLEMENT;
	x2DRect_f rect;
	m_Rectf.x = _rct.x;
	m_Rectf.y = _rct.y;
	m_Rectf.w = _rct.z;
	m_Rectf.h = _rct.w;
}

void x2DQuadElement::render()
{
	xMaterial* pMaterial = material();
    IGpuProgram* pShader = shader();
	m_pRenderApi->setGpuProgram(pShader);
	
	//draw primitive
	//m_pRenderApi->enter2DMode();
	//m_pRenderApi->drawRect(m_hMainTexture.getResource() , m_Rectf,m_fColor);
	//m_pRenderApi->leave2DMode();

	XEVOL_WARNNING_NOT_IMPLEMENT;
	m_pRenderApi->setGpuProgram(NULL);

	return ;
}

bool x2DQuadElement::prepare()
{
	m_pRenderApi->getMatrix(m_matPos ,MATRIXMODE_World);
	return true;
}

bool x2DQuadElement::unprepare()
{
	return true;
}

END_NAMESPACE_XEVOL3D
