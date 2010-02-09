#include "../BaseLib/xEvol3DAPI.h"
#include "../BaseLib/xEvol3DBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D




class _XEVOL_BASE_API_ xPhysicsObject : public IBaseObject
{

};



class _XEVOL_BASE_API_ xPhysics : public IBaseObject
{
public:
	virtual bool createObject() = 0;
};

END_NAMESPACE_XEVOL3D

