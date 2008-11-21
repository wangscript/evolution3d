#ifndef __XEVOL3D_DRAABLEOBJECT_H__
#define __XEVOL3D_DRAABLEOBJECT_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../XMathLib/XGeomLib.h"
#include "../XMathLib/XGVolume.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_  IDrawableObject : public IBaseObject
{
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IDrawableObject);
	virtual ~IDrawableObject(){};       
	//used for culling
	virtual void           center(XMathLib::xvec3&   _center) = 0;
	virtual void           aabb(xGeomLib::xaabb&     _aabb) = 0;
	virtual void           shpere(xGeomLib::xshpere& _shpere) = 0;
	virtual IRenderApi*    renderApi() = 0;
	virtual void           render(unsigned long passedTime) = 0;
	virtual bool           update(unsigned long passedTime) = 0 ;
};

END_NAMESPACE_XEVOL3D
#endif
