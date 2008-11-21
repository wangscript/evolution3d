#ifndef __XEVOL3D_DRAABLE_HELPER_H__
#define __XEVOL3D_DRAABLE_HELPER_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../XMathLib/XGeomLib.h"
#include "../XMathLib/XGVolume.h"
#include "xDrawableObject.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_  IDrawableHelper : public IDrawableObject
{
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IDrawableHelper);
	virtual ~IDrawableHelper(){};       
	//used for culling
	virtual bool  init(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr) = 0;
	virtual bool  begin() = 0;
	virtual bool  end() = 0;
	virtual bool  update(unsigned long passedTime){return true ; }

};

END_NAMESPACE_XEVOL3D
#endif
