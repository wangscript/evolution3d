#ifndef __XEVOL3D_PRIMITIVE_ELEMENT_H__
#define __XEVOL3D_PRIMITIVE_ELEMENT_H__
#include "xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D

struct xPrimitiveMesh
{
public:
	  typedef std::vector<IInputBuffer* , dllsafe_alloc<IInputBuffer*> > vInputBuffers;
	  typedef std::vector<HBaseTexture  , dllsafe_alloc<HBaseTexture>  > vHBaseTextures; 
public:     
	  IVertexStream*  m_VertexStream;
	  vInputBuffers   m_IndexBuffers;
	  vHBaseTextures  m_hTextures;
};
typedef std::vector<xPrimitiveMesh , dllsafe_alloc<xPrimitiveMesh> > vPrimitiveMeshs;

class _XEVOL_BASE_API_ xPrimitiveElement : public IDrawElement
{
public:
      xPrimitiveElement(IBaseRenderer* pRenderer , int v);
      virtual ~xPrimitiveElement();    
	  virtual void              center(XMathLib::xvec3&   _center);
	  virtual void              aabb(xGeomLib::xaabb&     _aabb);
	  virtual void              shpere(xGeomLib::xshpere& _shpere);
	  virtual IRenderApi*       renderApi();
	  virtual bool              render(unsigned long passedTime);
	  virtual bool              update(unsigned long passedTime);
	  virtual bool              init(size_t nMesh);
	  virtual xPrimitiveMesh*   getMesh(size_t idx ) ;
	  virtual bool              insertInputBuffer(size_t idx, IInputBuffer* pIndexBuffer);
	  virtual bool              addMesh(IVertexStream* pVertexStream , IInputBuffer* pIndexBuffer);
	  virtual bool              unload();
public:
	DECL_BASE_OBJECT_DLLSAFE(xPrimitiveElement);
protected:
	vPrimitiveMeshs          m_vMeshs;
};


END_NAMESPACE_XEVOL3D
#endif
