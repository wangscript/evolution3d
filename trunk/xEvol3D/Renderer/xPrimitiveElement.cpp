#include "../xStdPch.h"
#include "xPrimitiveElement.h"
#include "xRenderer.h"
#include "xMaterial.h"
#include "../xModel/xCoreMesh.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xPrimitiveElement ,  IDrawElement);
IMPL_OBJECT_FACTORY(xPrimitiveElement , IDrawElement , xPrimitiveElementFactory , IDrawElementCreator , xDrawElementFactoryMgr , L"PrimitiveElement", L"Primitive Element" , IBaseRenderer* , int);

xPrimitiveElement::xPrimitiveElement(IBaseRenderer* pRenderer , int v):IDrawElement(pRenderer)
{

}

xPrimitiveElement::~xPrimitiveElement()
{
     unload();
}

void xPrimitiveElement::center(XMathLib::xvec3&   _center)
{

}
void xPrimitiveElement::aabb(xGeomLib::xaabb&     _aabb)
{

}

void xPrimitiveElement::shpere(xGeomLib::xshpere& _shpere)
{

}

bool xPrimitiveElement::init(size_t nMesh)
{
      m_vMeshs.reserve(nMesh);
	  return true;
}

xPrimitiveMesh* xPrimitiveElement::getMesh(size_t idx )
{
     if(m_vMeshs.size() > idx)
		 return &m_vMeshs[idx];
	 return NULL;
}

bool xPrimitiveElement::insertInputBuffer(size_t idx, IInputBuffer* pIndexBuffer)
{
     xPrimitiveMesh* pMesh = getMesh(idx);
	 if(pMesh == NULL)
		 return false;
	 pMesh->m_IndexBuffers.push_back(pIndexBuffer);
	 pIndexBuffer->AddRef();
	 return true;
}

bool xPrimitiveElement::addMesh(IVertexStream* pVertexStream , IInputBuffer* pIndexBuffer)
{
     xPrimitiveMesh mesh;
	 m_vMeshs.push_back(mesh);
	 xPrimitiveMesh& _mesh = m_vMeshs[m_vMeshs.size() - 1];
	 _mesh.m_VertexStream = pVertexStream;
	 _mesh.m_IndexBuffers.push_back( pIndexBuffer );
	 pVertexStream->AddRef();
	 pIndexBuffer->AddRef();
	 return true;
}

bool xPrimitiveElement::unload()
{
	size_t nMesh = m_vMeshs.size() ;
    for(size_t i = 0 ; i < nMesh ; i ++)
	{
		xPrimitiveMesh& _mesh = m_vMeshs[ i ];
		size_t nIndexBuffers = _mesh.m_IndexBuffers.size() ;
        for(size_t j = 0 ; j < nIndexBuffers ; j ++)
		{
			_mesh.m_IndexBuffers[j]->ReleaseObject();
		}
		_mesh.m_VertexStream->ReleaseObject();
	}
	m_vMeshs.clear();
	return true;
}

IRenderApi* xPrimitiveElement::renderApi()
{
	if(m_pRenderer == NULL)
		return NULL;
	return m_pRenderer->renderApi();
}

bool xPrimitiveElement::render(unsigned long passedTime)
{
	if(renderApi() == NULL)
		return false;
	size_t nMeshs = m_vMeshs.size() ;
	for(size_t i = 0 ; i < nMeshs ; i ++)
	{
		xPrimitiveMesh& _mesh = m_vMeshs[i];
		renderApi()->setVertexStream( _mesh.m_VertexStream );
		size_t nTextures = _mesh.m_hTextures.size() ;
		for(size_t iTex = 0 ; iTex < nTextures ; iTex ++)
		{
			IBaseTexture* pTexture = _mesh.m_hTextures[iTex].getResource();
			renderApi()->setTexture( (int)iTex ,  pTexture );
		}

		size_t nIB = _mesh.m_IndexBuffers.size() ;
		for(size_t iIB = 0 ; iIB < nIB ; iIB ++)
		{
			IInputBuffer* pIdxBuffer = _mesh.m_IndexBuffers[iIB];
			renderApi()->draw(pIdxBuffer , pIdxBuffer->nElement() );
		}

	}
	return true;
}

bool xPrimitiveElement::update(unsigned long passedTime)
{
	return true;
}

END_NAMESPACE_XEVOL3D
