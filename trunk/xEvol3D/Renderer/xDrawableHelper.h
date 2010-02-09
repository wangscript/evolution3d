#ifndef __XEVOL3D_DRAABLE_HELPER_H__
#define __XEVOL3D_DRAABLE_HELPER_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../XMathLib/xGeomLib.h"
#include "../XMathLib/xvolume.h"
#include "xDrawElement.h"
#include "xRenderer.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_  IDrawableHelper : public IDrawElement
{
public:
	DECL_BASE_OBJECT_DLLSAFE(IDrawableHelper);
    IDrawableHelper(IBaseRenderer* pRenderer);
	virtual ~IDrawableHelper(){};       
	//used for culling
	virtual bool  init(xBaseTextureMgr* pTexMgr) = 0;
	virtual bool  begin() = 0;
	virtual bool  end() = 0;
	virtual bool  update(unsigned long passedTime){return true ; }

};

END_NAMESPACE_XEVOL3D
#endif
