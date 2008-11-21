#include "xDrawElement.h"
#include "xMaterial.h"
BEGIN_NAMESPACE_XEVOL3D


IMPL_BASE_OBJECT_CLASSID(IDrawElement , IRenderObject);
IDrawElement::IDrawElement(IRenderApi* pRdr):IRenderObject(pRdr)
{
	m_pMaterial  = NULL;
	m_pShader    = NULL;
}


void IDrawElement::setMaterial(xMaterial* pMaterial)
{
	m_pMaterial = pMaterial;
}

void IDrawElement::setShader(IGpuProgram* pShader)
{
	m_pShader = pShader;
}


xMaterial* IDrawElement::material()
{
	return m_pMaterial;
}

IGpuProgram* IDrawElement::shader()
{
	if( m_pShader )
		return m_pShader;
	return NULL;
}

END_NAMESPACE_XEVOL3D
