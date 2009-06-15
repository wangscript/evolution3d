#ifndef _KID_RENDERER_RENDER_TECH_
#define _KID_RENDERER_MATERIAL_TECH_
#include "../xRenderer.h"
#include "../xMaterial.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xKidRenderer;
class _XEVOL_BASE_API_ xKidBasicMaterial : public xMaterial
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xKidBasicMaterial);
public:
	xKidBasicMaterial(IRenderApi* pRendereApi);
	virtual ~xKidBasicMaterial();
};

END_NAMESPACE_XEVOL3D
#endif
