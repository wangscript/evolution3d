#ifndef __XRENDERER_OBJECT_H__
#define __XRENDERER_OBJECT_H__
#include "../BaseLib/xEvol3DBaseInc.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_   IBaseRenderer ;
class _XEVOL_BASE_API_   IRenderObject : public IBaseObject
{
protected:
	IBaseRenderer* m_pRenderer;
	IRenderObject(IBaseRenderer* pOwner);
	virtual ~IRenderObject();
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IRenderObject);
	virtual void*          handle(){return 0 ; }
	virtual bool           lockRenderObject()  {return true ; }
	virtual bool           unlockRenderObject(){return true ;  }
	virtual IBaseRenderer* renderer(){ return m_pRenderer ; }
	virtual void           setRenderer(IBaseRenderer* pRenderer){  m_pRenderer = pRenderer ; }
};

END_NAMESPACE_XEVOL3D
#endif
